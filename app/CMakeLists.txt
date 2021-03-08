cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DVK_USE_PLATFORM_ANDROID_KHR -DVK_NO_PROTOTYPES")

set(HEADER_DIR src/main/cpp/include)
set(SRC_DIR src/main/cpp/src)
set(WRAPPER_DIR src/main/cpp/vulkan_wrapper)
set(ENGINE_SRC src/main/cpp/src/engine)
set(ENGINE_HEADER_DIR src/main/cpp/include/engine)
set(FILTER_SRC src/main/cpp/src/filter)
set(FILTER_HEADER_DIR src/main/cpp/include/filter)
set(STB_DIR src/main/cpp/include/stb)

include_directories(${WRAPPER_DIR})
include_directories(${HEADER_DIR})
include_directories(${ENGINE_HEADER_DIR})
include_directories(${FILTER_HEADER_DIR})
include_directories(${STB_DIR})

add_library( # Sets the name of the library.
        media-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${SRC_DIR}/VideoRenderer.cpp
        ${SRC_DIR}/VideoRendererContext.cpp
        ${SRC_DIR}/VideoRendererJNI.cpp
        ${SRC_DIR}/CommonUtils.cpp
        ${SRC_DIR}/VKUtils.cpp
        ${WRAPPER_DIR}/vulkan_wrapper.cpp

        ${ENGINE_SRC}/VKEngineRenderer.cpp

        ${ENGINE_SRC}/VKDeviceManager.cpp
        ${ENGINE_SRC}/VKSwapChainManager.cpp
        ${ENGINE_SRC}/VKRender.cpp
        ${ENGINE_SRC}/VKBufferManager.cpp
        ${ENGINE_SRC}/VKTextureManager.cpp
        ${ENGINE_SRC}/VKOffScreen.cpp

        ${FILTER_SRC}/VulkanFilter.cpp
        ${FILTER_SRC}/OffScreenFilter.cpp
        ${FILTER_SRC}/EffectFilter.cpp
        ${FILTER_SRC}/MirrorFilter.cpp
        ${FILTER_SRC}/RGBFilter.cpp
        ${FILTER_SRC}/ColorInvertFilter.cpp
        ${FILTER_SRC}/ExposureFilter.cpp
        ${FILTER_SRC}/GammFilter.cpp
        ${FILTER_SRC}/ContrastFilter.cpp
        ${FILTER_SRC}/HazeFilter.cpp


        ${FILTER_SRC}/FilterUtil.cpp
        )


find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

find_library( # Sets the name of the path variable.
        GLESv2-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        GLESv2 )

target_link_libraries( # Specifies the target library.
        media-lib
        android
        ${log-lib}
        ${GLESv2-lib})



# 关联 Shader 相关的库
get_filename_component(cdep-dependencies_DIR
        ${SRC_DIR}/../../../../../.cdep/modules  ABSOLUTE)
find_package(cdep-dependencies REQUIRED)
add_all_cdep_dependencies(media-lib)