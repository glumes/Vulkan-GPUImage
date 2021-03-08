//
// Created by glumes on 2021/2/23.
//

#ifndef VULKANCAMERA_VKSWAPCHAININFO_H
#define VULKANCAMERA_VKSWAPCHAININFO_H

#include <vulkan_wrapper.h>
#include <vector>
#include <string>
#include <Log.h>

class VKDeviceInfo;
class VKRenderInfo;

class VKSwapChainInfo {

public:
    VKSwapChainInfo();

    ~VKSwapChainInfo();

    int createSwapChain(VKDeviceInfo* info);

    int createFrameBuffer(VKDeviceInfo *deviceInfo, VKRenderInfo * renderInfo, VkImageView depthView =  VK_NULL_HANDLE);


//private:

    VkSwapchainKHR swapchain;
    uint32_t swapchainLength;

    VkExtent2D displaySize;
    VkFormat displayFormat;

    // array of frame buffers and views
    std::unique_ptr<VkFramebuffer[]> framebuffers;
    std::unique_ptr<VkImage[]> displayImages;
    std::unique_ptr<VkImageView[]> displayViews;
};


#endif //VULKANCAMERA_VKSWAPCHAININFO_H
