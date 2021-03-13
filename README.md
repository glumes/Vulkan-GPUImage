## Vulkan-GPUImage

基于 Vulkan 渲染的 GPUImage 版本，实现渲染链机制，复刻 GPUImage 上的多个效果（逐渐增加中）。

Filter 效果添加参考该文章: [用 Vulkan 渲染写一个 Android GPUImage](https://mp.weixin.qq.com/s/gHzuXq-Yil1If1qs_SVXJA)

更多技术实现，详见源码~~

## Vulkan 学习文章

1. [进击的 Vulkan 移动开发（一）之今生前世](https://glumes.com/post/vulkan/vulkan-tutorial-concept/)
2. [进击的 Vulkan 移动开发之 Instance & Device & Queue](https://glumes.com/post/vulkan/vulkan-tutorial-instance-device-queue/)
3. [进击的 Vulkan 移动开发之 Command Buffer](https://glumes.com/post/vulkan/vulkan-tutorial-command-buffer/)
4. [进击的 Vulkan 移动开发之 SwapChain](https://glumes.com/post/vulkan/vulkan-tutorial-swap-chain/)
5. 更多内容，努力更新中...

## 编译

在运行前最好确保项目目录下 cdep 程序(Window 下对应 cdep.bat) 具有执行权限。

在 app module 下的 build.gradle 中会执行上述程序。

```build
def CDEP_CMD = './cdep'
if (System.properties['os.name'].toLowerCase().contains('windows')) {
    CDEP_CMD = 'cdep.bat'
}
if (!file("../.cdep").exists()) {
    exec {
        workingDir("..")
        executable "${CDEP_CMD}"
        args = []
    }
}
```

该程序会下载好 Vulkan Shader 相关的配置，不用再去链接 NDK 里面的路径了。

```cpp
# 关联 Shader 相关的库
get_filename_component(cdep-dependencies_DIR
        ${SRC_DIR}/../../../../../.cdep/modules  ABSOLUTE)
find_package(cdep-dependencies REQUIRED)
add_all_cdep_dependencies(media-lib)
```

编译运行过程中如有问题，欢迎联系我处理。

## 微信公众号

欢迎关注微信公众号："音视频开发进阶"，技术交流欢迎加我微信：ezglumes ，拉你入群讨论~~

![](https://image.glumes.com/wechat-planet.png)