//
// Created by glumes on 2021/2/28.
//

#ifndef VULKANCAMERA_VKOFFSCREEN_H
#define VULKANCAMERA_VKOFFSCREEN_H

#include <vulkan_wrapper.h>

#include <vector>

class VKDeviceManager;

class VKSwapChainManager;

class VKBufferManager;

class VKTextureManager;

class VKOffScreen {

public:
    struct FrameBufferAttachment {
        VkImage image;
        VkDeviceMemory mem;
        VkImageView view;
    };

    struct OffscreenPass {
        int32_t width, height;
        std::vector<VkFramebuffer> frameBuffer;
        std::vector<FrameBufferAttachment> color;
        std::vector<VkSampler> sampler;
        std::vector<VkDescriptorImageInfo> descriptor;
        VkRenderPass renderPass;
    } offscreenPass;


    struct VulkanGfxPipelineInfo {
        VkDescriptorSetLayout dscLayout_;
        VkDescriptorPool descPool_;
        VkDescriptorSet descSet_;
        VkPipelineLayout layout_;
        VkPipelineCache cache_;
        VkPipeline pipeline_;
    };

    VulkanGfxPipelineInfo offscreenPipeline;

    void createOffscreen(VKDeviceManager*deviceInfo, VKSwapChainManager *swapChainInfo);

    int updateOffScreenDescriptorSet(VKDeviceManager *deviceInfo, VKBufferManager *bufferInfo, VKTextureManager *textureInfo);


    VkResult allocateMemoryTypeFromProperties(VKDeviceManager *deviceInfo, uint32_t typeBits,
                                              VkFlags requirements_mask,
                                              uint32_t *typeIndex);

    int num = 2;
};


#endif //VULKANCAMERA_VKOFFSCREEN_H
