//
// Created by glumes on 2021/3/8.
//

#ifndef VULKANCAMERA_CONTRASTFILTER_H
#define VULKANCAMERA_CONTRASTFILTER_H

#include <VulkanFilter.h>

static const char *ExposureShader = "#version 400\n"
                                    "#extension GL_ARB_separate_shader_objects : enable\n"
                                    "#extension GL_ARB_shading_language_420pack : enable\n"
                                    "layout (binding = 0) uniform sampler2D tex;\n"
                                    "layout (location = 0) in vec2 texcoord;\n"
                                    "layout (location = 0) out vec4 uFragColor;\n"
                                    "layout (push_constant) uniform contrast {\n"
                                    " float contrast;\n"
                                    "} pushVals;\n"
                                    "void main() {\n"
                                    "    vec4 textureColor = texture(tex,  texcoord); \n"
                                    "   uFragColor = vec4(((textureColor.rgb - vec3(0.5)) * pushVals.contrast + vec3(0.5)),textureColor.w);\n"
                                    "}";

class ContrastFilter : public VulkanFilter {

public:
    ContrastFilter() : VulkanFilter() {
        pFragShader = ExposureShader;
        pushConstant.resize(1);
        pushConstant[0] = contrast;
    }

    virtual void setProcess(uint32_t process);

protected:

private:
    float  contrast = 1.2f;
};


#endif //VULKANCAMERA_CONTRASTFILTER_H
