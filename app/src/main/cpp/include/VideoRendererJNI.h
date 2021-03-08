#ifndef _H_VIDEO_RENDER_JNI_
#define _H_VIDEO_RENDER_JNI_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JCMCPRV(rettype, name)                                             \
  rettype JNIEXPORT JNICALL Java_com_glumes_vulkancamera_render_VideoRenderer_##name

JCMCPRV(void, create)(JNIEnv * env, jobject obj, jint type);
JCMCPRV(void, destroy)(JNIEnv * env, jobject obj);
JCMCPRV(void, init)(JNIEnv * env, jobject obj, jobject surface, jint width, jint height,jobject);
JCMCPRV(void, render)(JNIEnv * env, jobject obj);
JCMCPRV(void, draw)(JNIEnv * env, jobject obj, jbyteArray data, jint width, jint height, jint rotation);
JCMCPRV(void, setParameters)(JNIEnv * env, jobject obj, jint params);
JCMCPRV(void, setProgress)(JNIEnv * env, jobject obj, jint params);
JCMCPRV(jint, getParameters)(JNIEnv * env, jobject obj);

#ifdef __cplusplus
}
#endif

#endif // _H_VIDEO_RENDER_JNI_
