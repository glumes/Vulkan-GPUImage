//
// Created by glumes on 2021/2/23.
//

#ifndef VULKANCAMERA_VKDEVICEMANAGER_H
#define VULKANCAMERA_VKDEVICEMANAGER_H

#include <vulkan_wrapper.h>
#include <vector>
#include <string>
#include <Log.h>

class VKDeviceManager {

public:

    VKDeviceManager();

    ~VKDeviceManager();

    int createDevice(ANativeWindow* platformWindow, VkApplicationInfo* appInfo);

    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    VkDevice device;

    uint32_t queueFamilyIndex;


    VkSurfaceKHR surface;
    VkQueue queue;

    bool initialized = false;
};


#endif //VULKANCAMERA_VKDEVICEMANAGER_H
