//
// Created by glumes on 2021/3/6.
//

#ifndef VULKANCAMERA_OFFSCREENFILTER_H
#define VULKANCAMERA_OFFSCREENFILTER_H

#include <VulkanFilter.h>

class OffScreenFilter : public VulkanFilter{

public:

    OffScreenFilter():VulkanFilter(){
        pVertexShader = kVertexShader;
        pFragShader = kFragmentShader;
    }

    virtual int init(VkDevice device,VkRenderPass renderPass) override ;


    int updateDescriptorSet(VkSampler ,VkImageView,VkImageLayout) override;

    virtual int updateDescriptorSet(std::vector<VkDescriptorBufferInfo>& bufferInfo,std::vector<VkDescriptorImageInfo>& imageInfo);

protected:
    virtual int createDescriptorLayout() override;
    virtual int createDescriptorSet() override;

};


#endif //VULKANCAMERA_OFFSCREENFILTER_H
