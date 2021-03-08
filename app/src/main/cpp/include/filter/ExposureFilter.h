//
// Created by glumes on 2021/3/7.
//

#ifndef VULKANCAMERA_EXPOSUREFILTER_H
#define VULKANCAMERA_EXPOSUREFILTER_H

#include <VulkanFilter.h>

static const char *shader = "#version 400\n"
                            "#extension GL_ARB_separate_shader_objects : enable\n"
                            "#extension GL_ARB_shading_language_420pack : enable\n"
                            "layout (binding = 0) uniform sampler2D tex;\n"
                            "layout (location = 0) in vec2 texcoord;\n"
                            "layout (location = 0) out vec4 uFragColor;\n"
                            "layout (push_constant) uniform exposure {\n"
                            " float exposure;\n"
                            "} pushVals;\n"
                            "void main() {\n"
                            "    vec4 textureColor = texture(tex,  texcoord); \n"
                            "   uFragColor = vec4(textureColor.rgb * pow(2.0,pushVals.exposure),textureColor.w);\n"
                            "}";

class ExposureFilter : public VulkanFilter {

public:
    ExposureFilter() : VulkanFilter() {
        pFragShader = shader;
        pushConstant.resize(1);
        pushConstant[0] = exposure;
    }

    virtual void setProcess(uint32_t process);

protected:

private:
    float  exposure = 1.0f;
};


#endif //VULKANCAMERA_EXPOSUREFILTER_H
