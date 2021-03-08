//
// Created by glumes on 2021/2/23.
//

#ifndef VULKANCAMERA_VKSWAPCHAINMANAGER_H
#define VULKANCAMERA_VKSWAPCHAINMANAGER_H

#include <vulkan_wrapper.h>
#include <vector>
#include <string>
#include <Log.h>

class VKDeviceManager;
class VKRender;

class VKSwapChainManager {

public:
    VKSwapChainManager();

    ~VKSwapChainManager();

    int createSwapChain(VKDeviceManager* info);

    int createFrameBuffer(VKDeviceManager *deviceInfo, VKRender * renderInfo, VkImageView depthView =  VK_NULL_HANDLE);



    VkSwapchainKHR swapchain;
    uint32_t swapchainLength;

    VkExtent2D displaySize;
    VkFormat displayFormat;

    // array of frame buffers and views
    std::unique_ptr<VkFramebuffer[]> framebuffers;
    std::unique_ptr<VkImage[]> displayImages;
    std::unique_ptr<VkImageView[]> displayViews;
};


#endif //VULKANCAMERA_VKSWAPCHAINMANAGER_H
