//
// Created by glumes on 2021/2/23.
//

#include <VKEngineRenderer.h>
#include <Log.h>
#include <VKDeviceInfo.h>
#include <VKSwapChainInfo.h>
#include <VKRenderInfo.h>
#include <VKBufferInfo.h>
#include <VKShaders.h>
#include <VKTextureInfo.h>
#include <VKOffScreenInfo.h>

#include <VulkanFilter.h>
#include <OffScreenFilter.h>
#include <EffectFilter.h>
#include <MirrorFilter.h>

VKEngineRenderer::VKEngineRenderer() {
    vkRenderInfo = new VKRenderInfo;
    vkBufferInfo = new VKBufferInfo;
    vkSwapChainInfo = new VKSwapChainInfo;
    vkDeviceInfo = new VKDeviceInfo;
    vkTextureInfo = new VKTextureInfo;
    vkOffScreenInfo = new VKOffScreenInfo;

    vkDeviceInfo->initialized = false;

    m_pBuffer = nullptr;
    m_length = 0;

    vulkanFilter = new VulkanFilter;
    offscreenFilter = new OffScreenFilter;
    effectFilter = new EffectFilter;
}

VKEngineRenderer::~VKEngineRenderer() {
    vkDeviceInfo->initialized = false;

    deleteCommandPool();
}

void VKEngineRenderer::init(ANativeWindow *window, size_t width, size_t height,AAssetManager* manager) {
    m_backingWidth = width;
    m_backingHeight = height;

    aAssetManager = manager;

    if (!InitVulkan()) {
        LOGE("Vulkan is unavailable, install vulkan and re-start");
        return;
    }

    VkApplicationInfo appInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .apiVersion = VK_MAKE_VERSION(1, 0, 0),
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .pApplicationName = "camera2GLPreview",
            .pEngineName = "camera",
    };

    createDevice(window, &appInfo);
    createSwapChain();
}

void VKEngineRenderer::render() {
    uint32_t nextIndex;

    CALL_VK(vkAcquireNextImageKHR(vkDeviceInfo->device, vkSwapChainInfo->swapchain, UINT64_MAX, vkRenderInfo->semaphore, VK_NULL_HANDLE, &nextIndex));
    CALL_VK(vkResetFences(vkDeviceInfo->device,1,&vkRenderInfo->fence));

    VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &vkRenderInfo->semaphore;
    submitInfo.pWaitDstStageMask = &waitStageMask;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vkRenderInfo->cmdBuffer[nextIndex];
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = nullptr;

    if (vkDeviceInfo->queue == nullptr){
        LOGE("zhy vkDeviceInfo->queue == nullptr");
        return ;
    }

    CALL_VK(vkQueueSubmit(vkDeviceInfo->queue,1,&submitInfo,vkRenderInfo->fence));
    CALL_VK(vkWaitForFences(vkDeviceInfo->device,1,&vkRenderInfo->fence,VK_TRUE,100000000));

    VkResult result;
    VkPresentInfoKHR presentInfo{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .swapchainCount = 1,
        .pSwapchains = &vkSwapChainInfo->swapchain,
        .pImageIndices = &nextIndex,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = nullptr,
        .pResults = &result
    };

    vkQueuePresentKHR(vkDeviceInfo->queue,&presentInfo);
};

void VKEngineRenderer::updateFrame(const video_frame &frame) {

}

void
VKEngineRenderer::draw(uint8_t *buffer, size_t length, size_t width, size_t height, int rotation) {
    m_pBuffer = buffer;
    m_length = length;
    m_rotation = rotation;

    m_width = width;
    m_height = height;

//    if (vkDeviceInfo->initialized && (m_width != width || m_height != height)) {
//        m_width = width;
//        m_height = height;
//
//        deleteUniformBuffers();
//        deleteTextures();
//        deleteCommandPool();
//
//        createUniformBuffers();
//        createTextures();
//        createCommandPool();
//
//    } else {
//        m_width = width;
//        m_height = height;
//    }

    if (!vkDeviceInfo->initialized) {
        createRenderPass();
        createFrameBuffers();

        createVertexBuffer();
        createIndexBuffer();
        createUniformBuffers();
        createTextures();

        createOffscreenReaderPassAndFramebuffer();

        offscreenFilter->init(vkDeviceInfo->device,vkOffScreenInfo->offscreenPass.renderPass);


        std::vector<VkDescriptorBufferInfo> vecBufferInfo;
        vecBufferInfo.resize(1);


        VkDescriptorBufferInfo bufferInfo {
                bufferInfo.buffer = vkBufferInfo->uboBuffer,
                bufferInfo.offset = 0,
                bufferInfo.range = sizeof(UniformBufferObject)
        };

        vecBufferInfo[0] = bufferInfo;

        LOGI("zhy vec buffer info size is %d",vecBufferInfo.size());

        std::vector<VkDescriptorImageInfo> vecImageInfo;
        vecImageInfo.resize(3);

        VkDescriptorImageInfo texDsts[3];
        memset(texDsts, 0, sizeof(texDsts));
        for (int i = 0; i < 3; ++i) {
            texDsts[i].sampler = vkTextureInfo->textures[i].sampler;
            texDsts[i].imageView = vkTextureInfo->textures[i].view;
            texDsts[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;
            vecImageInfo[i] = texDsts[i];
        }


        LOGI("zhy vec image info size is %d",vecImageInfo.size());

        offscreenFilter->updateDescriptorSet(vecBufferInfo,vecImageInfo);

        effectFilter->init(vkDeviceInfo->device,vkOffScreenInfo->offscreenPass.renderPass);

        effectFilter->updateDescriptorSet(vkOffScreenInfo->offscreenPass.descriptor[0].sampler,
                                          vkOffScreenInfo->offscreenPass.descriptor[0].imageView,
                                          VK_IMAGE_LAYOUT_GENERAL);

        vulkanFilter->init(vkDeviceInfo->device,vkRenderInfo->renderPass);

        vulkanFilter->updateDescriptorSet(
                vkOffScreenInfo->offscreenPass.descriptor[1].sampler,
                vkOffScreenInfo->offscreenPass.descriptor[1].imageView,
                VK_IMAGE_LAYOUT_GENERAL
        );

        createCommandPool();


        vkDeviceInfo->initialized = true;
    }


    if (vkDeviceInfo->initialized && m_params < 1){

    } else if (m_params != m_filter){
        m_filter = m_params;

        LOGI("zhy ew filter");
        delete effectFilter;
        effectFilter = nullptr;

//        effectFilter = FilterUtil::getFilterByType(m_filter);
        effectFilter = FilterUtil::getFilterByType(m_filter);

        effectFilter->init(vkDeviceInfo->device,vkOffScreenInfo->offscreenPass.renderPass);


        std::vector<VkDescriptorBufferInfo> vecBufferInfo;
        vecBufferInfo.resize(1);

        VkDescriptorBufferInfo bufferInfo {
                bufferInfo.buffer = vkBufferInfo->rgbBuffer,
                bufferInfo.offset = 0,
                bufferInfo.range = sizeof(RGBBufferObject),
        };

        vecBufferInfo[0] = bufferInfo;


        std::vector<VkDescriptorImageInfo> imageInfoList;
        VkDescriptorImageInfo imageInfo{
            .sampler = vkOffScreenInfo->offscreenPass.descriptor[0].sampler,
            .imageView = vkOffScreenInfo->offscreenPass.descriptor[0].imageView,
            .imageLayout = vkOffScreenInfo->offscreenPass.descriptor[0].imageLayout
        };

        imageInfoList.push_back(imageInfo);

        effectFilter->updateDescriptorSet(vecBufferInfo,imageInfoList);

//        effectFilter->updateImageDescriptorSet(imageInfoList,0);

        createCommandPool();
    }


    if (m_CurrentProcess != m_LastProcess){
        m_LastProcess = m_CurrentProcess;
        createCommandPool();
        LOGE("zhy m_CurrentProcess != m_LastProcess create command pool");
    }

    updateTextures();

    if (vkDeviceInfo->initialized) {
        render();
    }
}

void VKEngineRenderer::setParameters(uint32_t params) {
    m_params = params;
}

uint32_t VKEngineRenderer::getParameters() {
    return 0;
}

bool VKEngineRenderer::createTextures() {

    vkTextureInfo->createTexture(vkDeviceInfo,m_pBuffer,m_width,m_height);

//    vkTextureInfo->createImgTexture(vkDeviceInfo,aAssetManager);

    return true;
}

bool VKEngineRenderer::updateTextures() {
    vkTextureInfo->updateTextures(vkDeviceInfo,m_pBuffer,m_width,m_height);
    return false;
}

void VKEngineRenderer::deleteTextures() {
    vkTextureInfo->deleteTextures(vkDeviceInfo);
}


void VKEngineRenderer::createDevice(ANativeWindow *platformWindow, VkApplicationInfo *appInfo) {
    vkDeviceInfo->createDevice(platformWindow, appInfo);
}

void VKEngineRenderer::createSwapChain() {
    vkSwapChainInfo->createSwapChain(vkDeviceInfo);
}

void VKEngineRenderer::createRenderPass() {
    vkRenderInfo->createRenderPass(vkDeviceInfo, vkSwapChainInfo);
}

void VKEngineRenderer::createFrameBuffers(VkImageView depthView) {
    vkSwapChainInfo->createFrameBuffer(vkDeviceInfo, vkRenderInfo, depthView);
}

void VKEngineRenderer::createUniformBuffers() {
    vkBufferInfo->updateUniformBuffers(m_width,m_height,m_rotation,m_backingWidth,m_backingHeight);
    vkBufferInfo->createUniformBuffers(vkDeviceInfo);
    vkBufferInfo->createRGBUniformBuffer(vkDeviceInfo);
}

void VKEngineRenderer::createVertexBuffer() {

    vkBufferInfo->createVertexBuffer(vkDeviceInfo);
    vkBufferInfo->createShowVertexBuffer(vkDeviceInfo);

}

void VKEngineRenderer::createIndexBuffer() {
    vkBufferInfo->createIndexBuffer(vkDeviceInfo);
}

void VKEngineRenderer::createCommandPool() {
    vkRenderInfo->createCommandPool(vkDeviceInfo, vkSwapChainInfo, vkBufferInfo, vkOffScreenInfo, vulkanFilter, offscreenFilter,
                                    effectFilter);
}

void VKEngineRenderer::deleteUniformBuffers() {
    vkDestroyBuffer(vkDeviceInfo->device, vkBufferInfo->uboBuffer, nullptr);
    vkFreeMemory(vkDeviceInfo->device, vkBufferInfo->uboBufferMemory, nullptr);
}

void VKEngineRenderer::deleteCommandPool() {
    vkRenderInfo->deleteCommandPool(vkDeviceInfo);
}


void VKEngineRenderer::createOffscreenReaderPassAndFramebuffer() {
    vkOffScreenInfo->createOffscreen(vkDeviceInfo,vkSwapChainInfo);
}

void VKEngineRenderer::setProcess(uint32_t process) {
    m_CurrentProcess = process;

    if (effectFilter != nullptr){
        effectFilter->setProcess(m_CurrentProcess);
    }
}
