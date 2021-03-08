//
// Created by glumes on 2021/2/24.
//

#ifndef VULKANCAMERA_VKDEFINE_H
#define VULKANCAMERA_VKDEFINE_H

struct Vertex {
    float pos[3];
    float uv[2];
};

struct UniformBufferObject {
    float projection[16];
    float rotation[16];
    float scale[16];
};


struct RGBBufferObject{
    float red;
    float green;
    float blue;
};

#endif //VULKANCAMERA_VKDEFINE_H
