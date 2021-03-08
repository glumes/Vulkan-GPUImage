//
// Created by glumes on 2021/3/7.
//

#include "GammFilter.h"

void GammFilter::setProcess(uint32_t process) {
    gamma = FilterUtil::getProcess(process,0.0f,3.0f);
    pushConstant[0] = gamma;
}
