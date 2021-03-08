//
// Created by glumes on 2021/2/23.
//

#include <VKDeviceManager.h>
#include <VKSwapChainManager.h>
#include <VKRender.h>

VKSwapChainManager::VKSwapChainManager() {

}

VKSwapChainManager::~VKSwapChainManager() {

}

int VKSwapChainManager::createSwapChain(VKDeviceManager* info) {
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(info->physicalDevice, info->surface,
                                              &surfaceCapabilities);

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(info->physicalDevice, info->surface,
                                         &formatCount, nullptr);

    std::unique_ptr<VkSurfaceFormatKHR[]> formats = std::make_unique<VkSurfaceFormatKHR[]>(formatCount);

    vkGetPhysicalDeviceSurfaceFormatsKHR(info->physicalDevice, info->surface,
                                         &formatCount, formats.get());

    uint32_t chosenFormat;
    for (chosenFormat = 0; chosenFormat < formatCount; chosenFormat++) {
        if (formats[chosenFormat].format == VK_FORMAT_R8G8B8A8_UNORM) break;
    }
    assert(chosenFormat < formatCount);

    displaySize = surfaceCapabilities.currentExtent;
    displayFormat = formats[chosenFormat].format;

    // Create a swap chain (here we choose the minimum available number of surface in the chain)
    VkSwapchainCreateInfoKHR swapchainCreateInfo {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .surface = info->surface,
            .minImageCount = surfaceCapabilities.minImageCount,
            .imageFormat = formats[chosenFormat].format,
            .imageColorSpace = formats[chosenFormat].colorSpace,
            .imageExtent = surfaceCapabilities.currentExtent,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
            .imageArrayLayers = 1,
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = &info->queueFamilyIndex,
            .presentMode = VK_PRESENT_MODE_FIFO_KHR,
            .oldSwapchain = VK_NULL_HANDLE,
            .clipped = VK_FALSE,
    };

    CALL_VK(vkCreateSwapchainKHR(info->device, &swapchainCreateInfo, nullptr,
                    &swapchain));

    // Get the length of the created swap chain
    CALL_VK(vkGetSwapchainImagesKHR(info->device, swapchain,
                                    &swapchainLength, nullptr));

    return 0;
}

int VKSwapChainManager::createFrameBuffer(VKDeviceManager *deviceInfo, VKRender * renderInfo, VkImageView depthView) {

    // query display attachment to swapchain
    uint32_t swapchainImagesCount = 0;
    CALL_VK(vkGetSwapchainImagesKHR(deviceInfo->device, swapchain,
                                    &swapchainImagesCount, nullptr));
    displayImages = std::make_unique<VkImage[]>(swapchainImagesCount);
    CALL_VK(vkGetSwapchainImagesKHR(deviceInfo->device, swapchain,
                                    &swapchainImagesCount,
                                    displayImages.get()));

    // create image view for each swapchain image
    displayViews = std::make_unique<VkImageView[]>(swapchainImagesCount);
    for (uint32_t i = 0; i < swapchainImagesCount; i++) {
        VkImageViewCreateInfo viewCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext = nullptr,
                .image = displayImages[i],
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = displayFormat,
                .components = {
                        .r = VK_COMPONENT_SWIZZLE_R,
                        .g = VK_COMPONENT_SWIZZLE_G,
                        .b = VK_COMPONENT_SWIZZLE_B,
                        .a = VK_COMPONENT_SWIZZLE_A,
                },
                .subresourceRange = {
                        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                        .baseMipLevel = 0,
                        .levelCount = 1,
                        .baseArrayLayer = 0,
                        .layerCount = 1,
                },
                .flags = 0,
        };
        CALL_VK(vkCreateImageView(deviceInfo->device, &viewCreateInfo, nullptr, &displayViews[i]));
    }

    // create a framebuffer from each swapchain image
    framebuffers = std::make_unique<VkFramebuffer[]>(swapchainLength);
    for (uint32_t i = 0; i < swapchainLength; i++) {
        VkImageView attachments[2] = {
                displayViews[i],
                depthView,
        };
        VkFramebufferCreateInfo fbCreateInfo{
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext = nullptr,
                .renderPass = renderInfo->renderPass,
                .layers = 1,
                .attachmentCount = 1,  // 2 if using depth
                .pAttachments = attachments,
                .width = static_cast<uint32_t>(displaySize.width),
                .height = static_cast<uint32_t>(displaySize.height),
        };
        fbCreateInfo.attachmentCount = (depthView == VK_NULL_HANDLE ? 1 : 2);

        CALL_VK(vkCreateFramebuffer(deviceInfo->device, &fbCreateInfo, nullptr, &framebuffers[i]));
    }
    return 0;
}
