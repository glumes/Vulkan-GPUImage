//
// Created by glumes on 2021/3/7.
//

#ifndef VULKANCAMERA_GAMMFILTER_H
#define VULKANCAMERA_GAMMFILTER_H

#include <VulkanFilter.h>

static const char *Gammashader = "#version 400\n"
                            "#extension GL_ARB_separate_shader_objects : enable\n"
                            "#extension GL_ARB_shading_language_420pack : enable\n"
                            "layout (binding = 0) uniform sampler2D tex;\n"
                            "layout (location = 0) in vec2 texcoord;\n"
                            "layout (location = 0) out vec4 uFragColor;\n"
                            "layout (push_constant) uniform gamma {\n"
                            " float gamma;\n"
                            "} pushVals;\n"
                            "void main() {\n"
                            "    vec4 textureColor = texture(tex,  texcoord); \n"
                            "   uFragColor = vec4(pow(textureColor.rgb,vec3(pushVals.gamma)),textureColor.w);\n"
                            "}";

class GammFilter : public VulkanFilter {

public:
    GammFilter() : VulkanFilter() {
        pFragShader = Gammashader;
        pushConstant.resize(1);
        pushConstant[0] = gamma;
    }

    virtual void setProcess(uint32_t process);

protected:

private:
    float  gamma = 1.2f;
};


#endif //VULKANCAMERA_GAMMFILTER_H
