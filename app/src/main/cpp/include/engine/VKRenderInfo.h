//
// Created by glumes on 2021/2/23.
//

#ifndef VULKANCAMERA_VKRENDERINFO_H
#define VULKANCAMERA_VKRENDERINFO_H

#include <vulkan_wrapper.h>
#include <vector>
#include <string>
#include <Log.h>

class VKDeviceInfo;

class VKSwapChainInfo;

class VKBufferInfo;

class VKPipelineInfo;

class VKOffScreenInfo;

class VulkanFilter;

class OffScreenFilter;

class EffectFilter;

class VKRenderInfo {

public:
    VKRenderInfo();

    ~VKRenderInfo();


    int createRenderPass(VKDeviceInfo *deviceInfo, VKSwapChainInfo *swapChainInfo);

    int createCommandPool(VKDeviceInfo *deviceInfo, VKSwapChainInfo *swapChainInfo,
                          VKBufferInfo *bufferInfo,
                          VKOffScreenInfo *vkOffScreenInfo,
                          VulkanFilter * filter,OffScreenFilter* offScreenFilter,VulkanFilter* effectFilter);

    int deleteCommandPool(VKDeviceInfo *deviceInfo);

    void setImageLayout(VkCommandBuffer cmdBuffer,
                        VkImage image,
                        VkImageLayout oldImageLayout,
                        VkImageLayout newImageLayout,
                        VkPipelineStageFlags srcStages,
                        VkPipelineStageFlags destStages);


    VkRenderPass renderPass;
    VkCommandPool cmdPool;
    std::unique_ptr<VkCommandBuffer[]> cmdBuffer;
    uint32_t cmdBufferLen;
    VkSemaphore semaphore;
    VkFence fence;

};


#endif //VULKANCAMERA_VKRENDERINFO_H
