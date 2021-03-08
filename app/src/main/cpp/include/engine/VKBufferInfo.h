//
// Created by glumes on 2021/2/24.
//

#ifndef VULKANCAMERA_VKBUFFERINFO_H
#define VULKANCAMERA_VKBUFFERINFO_H

#include <vulkan_wrapper.h>
#include <vector>
#include <string>
#include <Log.h>
#include <VKDefine.h>

class VKDeviceInfo;

class VKBufferInfo {

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



    int createRGBUniformBuffer(VKDeviceInfo* deviceInfo);

    int createVertexBuffer(VKDeviceInfo* deviceInfo);


    int createShowVertexBuffer(VKDeviceInfo* deviceInfo);

    int createIndexBuffer(VKDeviceInfo* deviceInfo);

    int createUniformBuffers(VKDeviceInfo* deviceInfo);

    int updateUniformBuffers(size_t m_width,size_t m_height,int m_rotation,size_t m_backingWidth,size_t m_backingHeight);

    UniformBufferObject m_ubo;

    RGBBufferObject m_rgb;

private:

    bool mapMemoryTypeToIndex(VKDeviceInfo* deviceInfo,uint32_t typeBits, VkFlags requirements_mask, uint32_t* typeIndex);

    void createBuffer(VKDeviceInfo * deviceInfo,VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                      VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    void copyBuffer(VKDeviceInfo * deviceInfo,VkBuffer srcBuffer,VkBuffer dstBuffer,VkDeviceSize size);
};




#endif //VULKANCAMERA_VKBUFFERINFO_H
