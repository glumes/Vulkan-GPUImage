//
// Created by glumes on 2021/3/7.
//

#ifndef VULKANCAMERA_FILTERUTIL_H
#define VULKANCAMERA_FILTERUTIL_H


class VulkanFilter;

class FilterUtil {
public:
    static VulkanFilter* getFilterByType(int type);

    static float getProcess(int progress, float start, float end);
};


#endif //VULKANCAMERA_FILTERUTIL_H
