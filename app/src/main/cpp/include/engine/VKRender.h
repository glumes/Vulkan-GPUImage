//
// Created by glumes on 2021/2/23.
//

#ifndef VULKANCAMERA_VKRENDER_H
#define VULKANCAMERA_VKRENDER_H

#include <vulkan_wrapper.h>
#include <vector>
#include <string>
#include <Log.h>

class VKDeviceManager;

class VKSwapChainManager;

class VKBufferManager;


class VKOffScreen;

class VulkanFilter;

class OffScreenFilter;

class EffectFilter;

class VKRender {

public:
    VKRender();

    ~VKRender();


    int createRenderPass(VKDeviceManager *deviceInfo, VKSwapChainManager *swapChainInfo);

    int createCommandPool(VKDeviceManager *deviceInfo, VKSwapChainManager *swapChainInfo,
                          VKBufferManager *bufferInfo,
                          VKOffScreen *vkOffScreenInfo,
                          VulkanFilter * filter, OffScreenFilter* offScreenFilter, VulkanFilter* effectFilter);

    int deleteCommandPool(VKDeviceManager *deviceInfo);

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


#endif //VULKANCAMERA_VKRENDER_H
