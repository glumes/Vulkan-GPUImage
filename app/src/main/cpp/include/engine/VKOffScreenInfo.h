//
// Created by glumes on 2021/2/28.
//

#ifndef VULKANCAMERA_VKOFFSCREENINFO_H
#define VULKANCAMERA_VKOFFSCREENINFO_H

#include <vulkan_wrapper.h>

#include <vector>

class VKDeviceInfo;

class VKSwapChainInfo;

class VKBufferInfo;

class VKPipelineInfo;

class VKTextureInfo;

class VKOffScreenInfo {

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

    void createOffscreen(VKDeviceInfo*deviceInfo,VKSwapChainInfo *swapChainInfo);

    int updateOffScreenDescriptorSet(VKDeviceInfo *deviceInfo,VKBufferInfo *bufferInfo,VKTextureInfo *textureInfo);


    VkResult allocateMemoryTypeFromProperties(VKDeviceInfo *deviceInfo, uint32_t typeBits,
                                              VkFlags requirements_mask,
                                              uint32_t *typeIndex);

    int num = 2;
};


#endif //VULKANCAMERA_VKOFFSCREENINFO_H
