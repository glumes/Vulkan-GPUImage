//
// Created by glumes on 2021/3/7.
//

#include "FilterUtil.h"
#include <VulkanFilter.h>
#include <MirrorFilter.h>
#include <RGBFilter.h>
#include <ColorInvertFilter.h>
#include <FilterType.h>
#include <ExposureFilter.h>
#include <GammFilter.h>
#include <ContrastFilter.h>
#include <HazeFilter.h>

VulkanFilter* FilterUtil::getFilterByType(int type) {
    switch (type){
        case SHOW_TYPE:
            return new VulkanFilter();
        case RBG_FILTER_TYPE:
            return new RGBFilter();
        case MIRROR_FILTER_TYPE:
            return new MirrorFilter();
        case REVERT_FILTER_TYPE:
            return new ColorInvertFilter();
        case EXPOSURE_FILTER_TYPE:
            return new ExposureFilter();
        case GAMMA_FILTER_TYPE:
            return new GammFilter();
        case CONTRAST_FILTER_TYPE:
            return new ContrastFilter();
        case HAZE_FILTER_TYPE:
            return new HazeFilter();
        default:
            break;
    }
    return new VulkanFilter();
}

float FilterUtil::getProcess(int progress, float start, float end) {
    return (end - start) * progress / 100.0f + start;
}
