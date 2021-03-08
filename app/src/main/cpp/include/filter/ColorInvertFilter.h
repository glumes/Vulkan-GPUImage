//
// Created by glumes on 2021/3/7.
//

#ifndef VULKANCAMERA_COLORINVERTFILTER_H
#define VULKANCAMERA_COLORINVERTFILTER_H

#include <VulkanFilter.h>

static const char* fragShader = "#version 400\n"
                                "#extension GL_ARB_separate_shader_objects : enable\n"
                                "#extension GL_ARB_shading_language_420pack : enable\n"
                                "layout (binding = 0) uniform sampler2D tex;\n"
                                "layout (location = 0) in vec2 texcoord;\n"
                                "layout (location = 0) out vec4 uFragColor;\n"
                                "void main() {\n"
                                "    vec4 textureColor = texture(tex,  texcoord); \n"
                                "   uFragColor = vec4((1.0 - textureColor.rgb),textureColor.w);\n"
                                "}";

class ColorInvertFilter :public VulkanFilter{

public:
    ColorInvertFilter():VulkanFilter(){
        pFragShader = fragShader;
    }
};


#endif //VULKANCAMERA_COLORINVERTFILTER_H
