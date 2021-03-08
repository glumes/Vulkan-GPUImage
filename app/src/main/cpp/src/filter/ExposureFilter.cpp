//
// Created by glumes on 2021/3/7.
//

#include "ExposureFilter.h"

void ExposureFilter::setProcess(uint32_t process) {
    exposure = FilterUtil::getProcess(process,-10.0f,10.0f);
    pushConstant[0] = exposure;
}

