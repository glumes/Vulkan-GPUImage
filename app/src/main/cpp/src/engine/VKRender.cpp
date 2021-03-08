//
// Created by glumes on 2021/2/23.
//

#include "VKRender.h"
#include <VKDeviceManager.h>
#include <VKSwapChainManager.h>
#include <VKBufferManager.h>
#include <engine/VKOffScreen.h>
#include <VulkanFilter.h>
#include <FilterDefine.h>
#include <OffScreenFilter.h>
#include <EffectFilter.h>

VKRender::VKRender() {

}

VKRender::~VKRender() {

}

int VKRender::createRenderPass(VKDeviceManager *deviceInfo, VKSwapChainManager* swapChainInfo) {
    // Create render pass
    VkAttachmentDescription attachmentDescriptions {
            .format = swapChainInfo->displayFormat,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };

    VkAttachmentReference colourReference {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };
    VkSubpassDescription subpassDescription {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .flags = 0,
            .inputAttachmentCount = 0,
            .pInputAttachments = nullptr,
            .colorAttachmentCount = 1,
            .pColorAttachments = &colourReference,
            .pResolveAttachments = nullptr,
            .pDepthStencilAttachment = nullptr,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments = nullptr,
    };
    VkRenderPassCreateInfo renderPassCreateInfo {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .attachmentCount = 1,
            .pAttachments = &attachmentDescriptions,
            .subpassCount = 1,
            .pSubpasses = &subpassDescription,
            .dependencyCount = 0,
            .pDependencies = nullptr,
    };
    CALL_VK(vkCreateRenderPass(deviceInfo->device, &renderPassCreateInfo, nullptr, &renderPass));
    return 0;
}

int VKRender::createCommandPool(VKDeviceManager *deviceInfo, VKSwapChainManager *swapChainInfo,
                                VKBufferManager *bufferInfo,
                                VKOffScreen *vkOffScreenInfo, VulkanFilter *filter,
                                OffScreenFilter *offScreenFilter,
                                VulkanFilter *effectFilter) {

    VkCommandPoolCreateInfo cmdPoolCreateInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = 0,
    };

    CALL_VK(vkCreateCommandPool(deviceInfo->device, &cmdPoolCreateInfo, nullptr,
                                &cmdPool));

    // Record a command buffer that just clear the screen
    // 1 command buffer draw in 1 framebuffer
    // In our case we need 2 command as we have 2 framebuffer
    cmdBufferLen = swapChainInfo->swapchainLength;
    cmdBuffer = std::make_unique<VkCommandBuffer[]>(swapChainInfo->swapchainLength);
    VkCommandBufferAllocateInfo cmdBufferCreateInfo {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = cmdPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = cmdBufferLen,
    };
    CALL_VK(vkAllocateCommandBuffers(deviceInfo->device, &cmdBufferCreateInfo, cmdBuffer.get()));

    for (int bufferIndex = 0; bufferIndex < swapChainInfo->swapchainLength; bufferIndex++) {
        // We start by creating and declare the "beginning" our command buffer
        VkCommandBufferBeginInfo cmdBufferBeginInfo {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .pNext = nullptr,
                .flags = 0,
                .pInheritanceInfo = nullptr,
        };
        CALL_VK(vkBeginCommandBuffer(cmdBuffer[bufferIndex], &cmdBufferBeginInfo));

        // transition the buffer into color attachment
        setImageLayout(cmdBuffer[bufferIndex],
                       swapChainInfo->displayImages[bufferIndex],
                       VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                       VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                       VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

        {

            FilterBuffer buffer{
                    .vertexBuffer = bufferInfo->vertexBuffer,
                    .indexBuffer = bufferInfo->indexBuffer,
                    .indexCount = (int)bufferInfo->m_indexCount,
            };
            offScreenFilter->bindFilterBuffer(buffer);

            FilterFramebuffer framebuffer{
                    .framebuffer = vkOffScreenInfo->offscreenPass.frameBuffer[0],
                    .width = (int)vkOffScreenInfo->offscreenPass.width,
                    .height= (int)vkOffScreenInfo->offscreenPass.height,
            };
            offScreenFilter->bindFilterFramebuffer(framebuffer);

            offScreenFilter->buildRenderPass(cmdBuffer[bufferIndex]);


            FilterBuffer effectBuffer{
                    .vertexBuffer = bufferInfo->vertexBuffer,
                    .indexBuffer = bufferInfo->indexBuffer,
                    .indexCount = (int)bufferInfo->m_indexCount,
            };
            effectFilter->bindFilterBuffer(effectBuffer);

            FilterFramebuffer effectFramebuffer{
                    .framebuffer = vkOffScreenInfo->offscreenPass.frameBuffer[1],
                    .width = (int)vkOffScreenInfo->offscreenPass.width,
                    .height= (int)vkOffScreenInfo->offscreenPass.height,
            };
            effectFilter->bindFilterFramebuffer(effectFramebuffer);

            effectFilter->buildRenderPass(cmdBuffer[bufferIndex]);



        }



        FilterBuffer buffer{
            .vertexBuffer = bufferInfo->vertexBuffer,
            .indexBuffer = bufferInfo->indexBuffer,
            .indexCount = (int)bufferInfo->m_indexCount,
        };
        filter->bindFilterBuffer(buffer);

        FilterFramebuffer framebuffer{
            .framebuffer = swapChainInfo->framebuffers[bufferIndex],
            .width = (int)swapChainInfo->displaySize.width,
            .height= (int)swapChainInfo->displaySize.height,
        };
        filter->bindFilterFramebuffer(framebuffer);

        filter->buildRenderPass(cmdBuffer[bufferIndex]);

        setImageLayout(cmdBuffer[bufferIndex],
                       swapChainInfo->displayImages[bufferIndex],
                       VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                       VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                       VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);
        CALL_VK(vkEndCommandBuffer(cmdBuffer[bufferIndex]));
    }

    // We need to create a fence to be able, in the main loop, to wait for our
    // draw command(s) to finish before swapping the framebuffers
    VkFenceCreateInfo fenceCreateInfo {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
    };
    CALL_VK(vkCreateFence(deviceInfo->device, &fenceCreateInfo, nullptr, &fence));


    // We need to create a semaphore to be able to wait, in the main loop, for our
    // framebuffer to be available for us before drawing.
    VkSemaphoreCreateInfo semaphoreCreateInfo {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
    };
    CALL_VK(vkCreateSemaphore(deviceInfo->device, &semaphoreCreateInfo, nullptr, &semaphore));
    return 0;
}

void
VKRender::setImageLayout(VkCommandBuffer cmdBuffer, VkImage image, VkImageLayout oldImageLayout,
                         VkImageLayout newImageLayout, VkPipelineStageFlags srcStages,
                         VkPipelineStageFlags destStages) {

    VkImageMemoryBarrier imageMemoryBarrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = NULL,
            .srcAccessMask = 0,
            .dstAccessMask = 0,
            .oldLayout = oldImageLayout,
            .newLayout = newImageLayout,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = image,
            .subresourceRange =
                    {
                            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                            .baseMipLevel = 0,
                            .levelCount = 1,
                            .baseArrayLayer = 0,
                            .layerCount = 1,
                    },
    };

    switch (oldImageLayout) {
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_PREINITIALIZED:
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
            break;

        default:
            break;
    }

    switch (newImageLayout) {
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            imageMemoryBarrier.dstAccessMask =
                    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;

        default:
            break;
    }

    vkCmdPipelineBarrier(cmdBuffer, srcStages, destStages, 0, 0, NULL, 0, NULL, 1,
                         &imageMemoryBarrier);
}

int VKRender::deleteCommandPool(VKDeviceManager *deviceInfo) {
    vkFreeCommandBuffers(deviceInfo->device, cmdPool, cmdBufferLen, cmdBuffer.get());
    vkDestroyCommandPool(deviceInfo->device, cmdPool, nullptr);
    vkDestroyFence(deviceInfo->device, fence, nullptr);
    vkDestroySemaphore(deviceInfo->device, semaphore, nullptr);
    return 0;
}
