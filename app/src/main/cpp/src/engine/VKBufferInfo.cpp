//
// Created by glumes on 2021/2/24.
//

#include <VKDeviceInfo.h>
#include "VKBufferInfo.h"
#include <VKDefine.h>
#include <CommonUtils.h>

void VKBufferInfo::createBuffer(VKDeviceInfo * deviceInfo,VkDeviceSize size, VkBufferUsageFlags usage,
                                VkMemoryPropertyFlags properties, VkBuffer &buffer,
                                VkDeviceMemory &bufferMemory) {
    VkBufferCreateInfo bufferInfo {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .size = size,
            .usage = usage,
            .flags = 0,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = &deviceInfo->queueFamilyIndex,
    };

    CALL_VK(vkCreateBuffer(deviceInfo->device, &bufferInfo, nullptr, &buffer));


    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(deviceInfo->device, buffer, &memReq);

    VkMemoryAllocateInfo allocInfo{
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = memReq.size,
            .memoryTypeIndex = 0,  // Memory type assigned in the next step
    };

    // Assign the proper memory type for that buffer
    mapMemoryTypeToIndex(deviceInfo,memReq.memoryTypeBits, properties, &allocInfo.memoryTypeIndex);
    // Allocate memory for the buffer
    CALL_VK(vkAllocateMemory(deviceInfo->device, &allocInfo, nullptr, &bufferMemory));
    CALL_VK(vkBindBufferMemory(deviceInfo->device, buffer, bufferMemory, 0));
}

void VKBufferInfo::copyBuffer(VKDeviceInfo *deviceInfo, VkBuffer srcBuffer, VkBuffer dstBuffer,
                              VkDeviceSize size) {

    VkCommandPoolCreateInfo cmdPoolCreateInfo {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = deviceInfo->queueFamilyIndex,
    };

    VkCommandPool cmdPool;
    CALL_VK(vkCreateCommandPool(deviceInfo->device, &cmdPoolCreateInfo, nullptr, &cmdPool));

    VkCommandBuffer cmdBuffer;
    const VkCommandBufferAllocateInfo cmd = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = cmdPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
    };

    CALL_VK(vkAllocateCommandBuffers(deviceInfo->device, &cmd, &cmdBuffer));
    VkCommandBufferBeginInfo cmdBufferInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            .pInheritanceInfo = nullptr};
    CALL_VK(vkBeginCommandBuffer(cmdBuffer, &cmdBufferInfo));

    VkBufferCopy copyRegion = {
            .srcOffset = 0,
            .dstOffset = 0,
            .size = size
    };
    vkCmdCopyBuffer(cmdBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    CALL_VK(vkEndCommandBuffer(cmdBuffer));
    VkFenceCreateInfo fenceInfo = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
    };
    VkFence fence;
    CALL_VK(vkCreateFence(deviceInfo->device, &fenceInfo, nullptr, &fence));

    VkSubmitInfo submitInfo = {
            .pNext = nullptr,
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = &cmdBuffer,
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
    };
    CALL_VK(vkQueueSubmit(deviceInfo->queue, 1, &submitInfo, fence) != VK_SUCCESS);
    CALL_VK(vkWaitForFences(deviceInfo->device, 1, &fence, VK_TRUE, 100000000) != VK_SUCCESS);
    vkDestroyFence(deviceInfo->device, fence, nullptr);

    vkFreeCommandBuffers(deviceInfo->device, cmdPool, 1, &cmdBuffer);
    vkDestroyCommandPool(deviceInfo->device, cmdPool, nullptr);
}


int VKBufferInfo::createVertexBuffer(VKDeviceInfo *deviceInfo) {
    const Vertex vertices[4] {
            { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f } },
            { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } },
            { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
            { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } }
    };

    VkBuffer  stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VkDeviceSize bufferSize = sizeof(vertices);

    createBuffer(deviceInfo,bufferSize,VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer,stagingBufferMemory);
    void* data = nullptr;
    CALL_VK(vkMapMemory(deviceInfo->device,stagingBufferMemory,0,bufferSize,0,&data));
    memcpy(data,vertices,bufferSize);
    vkUnmapMemory(deviceInfo->device,stagingBufferMemory);

    createBuffer(deviceInfo,bufferSize,VK_BUFFER_USAGE_TRANSFER_DST_BIT|VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,vertexBuffer,vertexBufferMemory);

    copyBuffer(deviceInfo,stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(deviceInfo->device, stagingBuffer, nullptr);
    vkFreeMemory(deviceInfo->device, stagingBufferMemory, nullptr);

    return 0;
}

bool VKBufferInfo::mapMemoryTypeToIndex(VKDeviceInfo* deviceInfo,uint32_t typeBits, VkFlags requirements_mask,
                                        uint32_t *typeIndex) {
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(deviceInfo->physicalDevice, &memoryProperties);
    // Search memtypes to find first index with those properties
    for (uint32_t i = 0; i < 32; i++) {
        if ((typeBits & 1) == 1) {
            // Type is available, does it match user properties?
            if ((memoryProperties.memoryTypes[i].propertyFlags & requirements_mask) ==
                requirements_mask) {
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }
    return false;
}

int VKBufferInfo::createIndexBuffer(VKDeviceInfo *deviceInfo) {
    const uint16_t indices[6] {
            0, 1, 2, 2, 3, 0
    };

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    m_indexCount = sizeof(indices) / sizeof(indices[0]);
    VkDeviceSize bufferSize = sizeof(indices);

    createBuffer(deviceInfo,bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer, stagingBufferMemory);

    void* data = nullptr;
    CALL_VK(vkMapMemory(deviceInfo->device, stagingBufferMemory, 0, bufferSize, 0, &data));
    memcpy(data, indices, bufferSize);
    vkUnmapMemory(deviceInfo->device, stagingBufferMemory);

    createBuffer(deviceInfo,bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 indexBuffer, indexBufferMemory);

    copyBuffer(deviceInfo,stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(deviceInfo->device, stagingBuffer, nullptr);
    vkFreeMemory(deviceInfo->device, stagingBufferMemory, nullptr);
    return 0;
}

int VKBufferInfo::createUniformBuffers(VKDeviceInfo *deviceInfo) {

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    VkDeviceSize bufferSize = sizeof(m_ubo);

    createBuffer(deviceInfo,bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer, stagingBufferMemory);

    void* data = nullptr;
    CALL_VK(vkMapMemory(deviceInfo->device, stagingBufferMemory, 0, bufferSize, 0, &data));
    memcpy(data, &m_ubo, bufferSize);
    vkUnmapMemory(deviceInfo->device, stagingBufferMemory);

    createBuffer(deviceInfo,bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 uboBuffer, uboBufferMemory);

    copyBuffer(deviceInfo,stagingBuffer, uboBuffer, bufferSize);

    vkDestroyBuffer(deviceInfo->device, stagingBuffer, nullptr);
    vkFreeMemory(deviceInfo->device, stagingBufferMemory, nullptr);
    return 0;
}

int VKBufferInfo::updateUniformBuffers(size_t m_width, size_t m_height, int m_rotation,
                                       size_t m_backingWidth, size_t m_backingHeight) {

    float targetAspectRatio = (float)m_width / (float)m_height;

    mat4f_load_ortho(-1.0f, 1.0f, -targetAspectRatio, targetAspectRatio, -1.0f, 1.0f, m_ubo.projection);

    mat4f_load_rotation_z(m_rotation + 180, m_ubo.rotation);

    float scaleFactor = aspect_ratio_correction(false, m_backingWidth, m_backingHeight, m_width, m_height);

    mat4f_load_scale(scaleFactor, scaleFactor, 1.0f, m_ubo.scale);

    return 0;
}

int VKBufferInfo::createShowVertexBuffer(VKDeviceInfo *deviceInfo) {

    const float vertexData[] = {
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
    };

    VkBufferCreateInfo bufferInfo {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .size = sizeof(vertexData),
            .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            .flags = 0,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = &deviceInfo->queueFamilyIndex,
    };

    CALL_VK(vkCreateBuffer(deviceInfo->device, &bufferInfo, nullptr,
                           &showVertexBuffer));

    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(deviceInfo->device, showVertexBuffer, &memReq);

    VkMemoryAllocateInfo allocInfo{
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = memReq.size,
            .memoryTypeIndex = 0,  // Memory type assigned in the next step
    };

    // Assign the proper memory type for that buffer
    mapMemoryTypeToIndex(deviceInfo,memReq.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &allocInfo.memoryTypeIndex);

    CALL_VK(vkAllocateMemory(deviceInfo->device, &allocInfo, nullptr, &showBufferMemory));


    void* data;
    CALL_VK(vkMapMemory(deviceInfo->device, showBufferMemory, 0, allocInfo.allocationSize,
                        0, &data));
    memcpy(data, vertexData, sizeof(vertexData));
    vkUnmapMemory(deviceInfo->device, showBufferMemory);

    CALL_VK(
            vkBindBufferMemory(deviceInfo->device, showVertexBuffer, showBufferMemory, 0));
    return 0;
}

int VKBufferInfo::createRGBUniformBuffer(VKDeviceInfo *deviceInfo) {

    m_rgb.red = 1.0;
    m_rgb.green = 0.0;
    m_rgb.blue = 1.0;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    VkDeviceSize bufferSize = sizeof(m_rgb);

    createBuffer(deviceInfo,bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 stagingBuffer, stagingBufferMemory);

    void* data = nullptr;
    CALL_VK(vkMapMemory(deviceInfo->device, stagingBufferMemory, 0, bufferSize, 0, &data));
    memcpy(data, &m_rgb, bufferSize);
    vkUnmapMemory(deviceInfo->device, stagingBufferMemory);

    createBuffer(deviceInfo,bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                 rgbBuffer, rgbBufferMemory);

    copyBuffer(deviceInfo,stagingBuffer, rgbBuffer, bufferSize);

    vkDestroyBuffer(deviceInfo->device, stagingBuffer, nullptr);
    vkFreeMemory(deviceInfo->device, stagingBufferMemory, nullptr);

    return 0;
}


