//
// Created by glumes on 2021/3/8.
//

#ifndef VULKANCAMERA_HAZEFILTER_H
#define VULKANCAMERA_HAZEFILTER_H

#include <VulkanFilter.h>

static const char * HazeShader = "#version 400\n"
                                 "#extension GL_ARB_separate_shader_objects : enable\n"
                                 "#extension GL_ARB_shading_language_420pack : enable\n"
                                 "layout (binding = 0) uniform sampler2D tex;\n"
                                 "layout (location = 0) in vec2 texcoord;\n"
                                 "layout (location = 0) out vec4 uFragColor;\n"
                                 "layout (push_constant) uniform haze {\n"
                                 " float distance;\n"
                                 " float slope;\n"
                                 "} pushVals;\n"
                                 "void main() {\n"
                                 "  vec4 color = vec4(1.0);\n"
                                 "  float d = texcoord.y * pushVals.slope + pushVals.distance;\n"
                                 "    vec4 textureColor = texture(tex,  texcoord); \n"
                                 "  textureColor = (textureColor - d * color) / (1.0 - d);\n"
                                 "   uFragColor = textureColor ;\n"
                                 "}";

class HazeFilter : public VulkanFilter {

public:
    HazeFilter() : VulkanFilter() {
        pFragShader = HazeShader;
        pushConstant.resize(2);
        pushConstant[0] = distance;
        pushConstant[1] = slope;
    }

    virtual void setProcess(uint32_t process);

protected:

private:
    float  distance = 0.2f;
    float slope = 0.0f;
};


#endif //VULKANCAMERA_HAZEFILTER_H
