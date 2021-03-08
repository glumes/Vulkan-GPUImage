//
// Created by glumes on 2021/3/7.
//

#ifndef VULKANCAMERA_MIRRORFILTER_H
#define VULKANCAMERA_MIRRORFILTER_H

#include <VulkanFilter.h>

class MirrorFilter :public VulkanFilter{

public:

    MirrorFilter():VulkanFilter(){
        pVertexShader = showVertexShader;
        pFragShader = revertShowFragShader;
    }

};


#endif //VULKANCAMERA_MIRRORFILTER_H
