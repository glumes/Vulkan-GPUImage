//
// Created by glumes on 2021/2/24.
//

#ifndef VULKANCAMERA_VKBUFFERMANAGER_H
#define VULKANCAMERA_VKBUFFERMANAGER_H

#include <vulkan_wrapper.h>
#include <vector>
#include <string>
#include <Log.h>
#include <VKDefine.h>

class VKDeviceManager;

class VKBufferManager {

public:

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    VkBuffer uboBuffer;
    VkDeviceMemory uboBufferMemory;


    VkBuffer rgbBuffer;
    VkDeviceMemory rgbBufferMemory;


    uint32_t m_indexCount;


    VkBuffer showVertexBuffer;
    VkDeviceMemory showBufferMemory;



    int createRGBUniformBuffer(VKDeviceManager* deviceInfo);

    int createVertexBuffer(VKDeviceManager* deviceInfo);


    int createShowVertexBuffer(VKDeviceManager* deviceInfo);

    int createIndexBuffer(VKDeviceManager* deviceInfo);

    int createUniformBuffers(VKDeviceManager* deviceInfo);

    int updateUniformBuffers(size_t m_width,size_t m_height,int m_rotation,size_t m_backingWidth,size_t m_backingHeight);

    UniformBufferObject m_ubo;

    RGBBufferObject m_rgb;

private:

    bool mapMemoryTypeToIndex(VKDeviceManager* deviceInfo, uint32_t typeBits, VkFlags requirements_mask, uint32_t* typeIndex);

    void createBuffer(VKDeviceManager * deviceInfo, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                      VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    void copyBuffer(VKDeviceManager * deviceInfo, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};




#endif //VULKANCAMERA_VKBUFFERMANAGER_H
