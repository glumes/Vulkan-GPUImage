//
// Created by glumes on 2021/3/8.
//

#include "HazeFilter.h"

void HazeFilter::setProcess(uint32_t process) {
    distance = FilterUtil::getProcess(process,-0.3f,0.3f);
    slope = FilterUtil::getProcess(process,-0.3f,0.3f);
    pushConstant[0] = distance;
    pushConstant[1] = slope;
}
