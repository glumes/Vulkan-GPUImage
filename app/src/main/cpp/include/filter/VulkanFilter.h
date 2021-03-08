//
// Created by glumes on 2021/2/28.
//

#ifndef VULKANCAMERA_VULKANFILTER_H
#define VULKANCAMERA_VULKANFILTER_H

#include <vulkan_wrapper.h>

#include <FilterDefine.h>

// 对于有纹理的 filter ，另起一个基类
// filter 里面设置 command 信息
// command 信息里面需要 顶点，需要 pipline ，需要 renderpass
// command 需要 pipeline descritor layout 这三样东西 ，这三样都是组合到一起的

// context ？ 包括 rendrepass framebuffer ,

// pipeline 需要 pool 才行

#include <VulkanInitializers.h>
#include <Log.h>
#include <array>
#include <VKShaders.h>
#include <VKUtils.h>
#include <FilterUtil.h>

class VulkanFilter {

public:

    VulkanFilter() : pVertexShader(showVertexShader), pFragShader(showFragShader) {
        pushConstant.resize(0);
    }

    virtual int init(VkDevice device,VkRenderPass renderPass);

    virtual int buildRenderPass(VkCommandBuffer commandBuffer,VkRenderPass renderPass,VkFramebuffer framebuffer);

    virtual int buildRenderPass(VkCommandBuffer commandBuffer);

    virtual int bindFilterBuffer(FilterBuffer& buffer);
    virtual int bindFilterFramebuffer(FilterFramebuffer& framebuffer);

    virtual int updateDescriptorSet(VkSampler sampler,VkImageView imageView,VkImageLayout imageLayout);

    virtual int updateDescriptorSet(std::vector<VkDescriptorBufferInfo>& bufferInfo,std::vector<VkDescriptorImageInfo>& imageInfo);

    virtual int updateBufferDescriptorSet(std::vector<VkDescriptorBufferInfo>& bufferInfo,int binding = 0);

    virtual int updateImageDescriptorSet(std::vector<VkDescriptorImageInfo>& imageInfo,int binding = 0 );

    virtual void setProcess(uint32_t process);
private:

protected:

    const char* pVertexShader = showVertexShader;
    const char* pFragShader = showFragShader;


    virtual int createDescriptorLayout();
    virtual int createDescriptorSet();
    virtual int createPipelineCache();
    virtual int createPipeline(VkRenderPass renderPass);
    virtual int createPipeline();

    struct FilterPipeline pipeline;
    struct FilterContext context;

    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;

    std::vector<float> pushConstant;

    int indexCount;

    int width;
    int height;

    bool isInit = false;

    uint32_t mProcess ;
};


#endif //VULKANCAMERA_VULKANFILTER_H
