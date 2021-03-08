//
// Created by glumes on 2021/3/6.
//

#ifndef VULKANCAMERA_EFFECTFILTER_H
#define VULKANCAMERA_EFFECTFILTER_H

#include <VulkanFilter.h>

class EffectFilter : public VulkanFilter {

public:

    EffectFilter():VulkanFilter(){
        pVertexShader = showVertexShader;
        pFragShader = showFragShader;
    }

};


#endif //VULKANCAMERA_EFFECTFILTER_H
