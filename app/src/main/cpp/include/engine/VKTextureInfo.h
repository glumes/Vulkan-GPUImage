//
// Created by glumes on 2021/2/24.
//

#ifndef VULKANCAMERA_VKTEXTUREINFO_H
#define VULKANCAMERA_VKTEXTUREINFO_H

#include <vulkan_wrapper.h>
#include <cassert>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

class VKDeviceInfo;

class VKRenderInfo;

class VKBufferInfo;

class VKTextureInfo {

public:

    VKTextureInfo() : texType{tTexY, tTexU, tTexV} {

    }

    static const VkFormat kTexFmt = VK_FORMAT_R8G8B8A8_UNORM;

    struct VulkanTexture {
        VkSampler sampler;
        VkImage image;
        VkImageLayout imageLayout;
        VkSubresourceLayout layout;
        VkDeviceMemory mem;
        VkImageView view;
        size_t width;
        size_t height;
        void *mapped;
    };

    typedef struct texture_object {
        VkSampler sampler;
        VkImage image;
        VkImageLayout imageLayout;
        VkDeviceMemory mem;
        VkImageView view;
        int32_t tex_width;
        int32_t tex_height;
    } texture_object;

    struct texture_object testTextures[1];


    VkResult LoadTextureFromFile(VKDeviceInfo * deviceInfo, AAssetManager* manager,const char* filePath,
                                 struct texture_object* tex_obj,
                                 VkImageUsageFlags usage, VkFlags required_props);
    static const uint32_t kTextureCount = 3;

    void createTexture(VKDeviceInfo *deviceInfo, uint8_t *m_pBuffer, size_t m_width, size_t m_height);

    void createImgTexture(VKDeviceInfo *deviceInfo,AAssetManager* manager);

    static const VkFormat kTextureFormat = VK_FORMAT_R8_UNORM;

    struct VulkanTexture textures[kTextureCount];

    enum TextureType {
        tTexY, tTexU, tTexV
    };

    const TextureType texType[kTextureCount];

    VkResult loadTexture(VKDeviceInfo *deviceInfo, uint8_t *buffer, TextureType type, size_t width,
                         size_t height,
                         VulkanTexture *texture, VkImageUsageFlags usage, VkFlags required_props);

    void setImageLayout(VkCommandBuffer cmdBuffer,
                        VkImage image,
                        VkImageLayout oldImageLayout,
                        VkImageLayout newImageLayout,
                        VkPipelineStageFlags srcStages,
                        VkPipelineStageFlags destStages);

    size_t getBufferOffset(VulkanTexture *texture, TextureType type, size_t width, size_t height);

    void copyTextureData(VulkanTexture *texture, uint8_t *data);


    VkResult allocateMemoryTypeFromProperties(VKDeviceInfo *deviceInfo, uint32_t typeBits,
                                              VkFlags requirements_mask,
                                              uint32_t *typeIndex);

    void deleteTextures(VKDeviceInfo *deviceInfo);

    void updateTextures(VKDeviceInfo *deviceInfo, uint8_t *buffer, size_t width, size_t height);
};



#endif //VULKANCAMERA_VKTEXTUREINFO_H
