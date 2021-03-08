//
// Created by glumes on 2021/3/8.
//

#include "ContrastFilter.h"

void ContrastFilter::setProcess(uint32_t process) {
    contrast = FilterUtil::getProcess(process,0.0,2.0f);
    pushConstant[0] = contrast;
}
