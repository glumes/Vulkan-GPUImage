//
// Created by glumes on 2021/2/23.
//

#ifndef VULKANCAMERA_VKDEVICEINFO_H
#define VULKANCAMERA_VKDEVICEINFO_H

#include <vulkan_wrapper.h>
#include <vector>
#include <string>
#include <Log.h>

class VKDeviceInfo {

public:

    VKDeviceInfo();

    ~VKDeviceInfo();

    int createDevice(ANativeWindow* platformWindow, VkApplicationInfo* appInfo);

//private:
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    VkDevice device;

    uint32_t queueFamilyIndex;


    VkSurfaceKHR surface;
    VkQueue queue;

    bool initialized = false;
};


#endif //VULKANCAMERA_VKDEVICEINFO_H
