#include "VideoRendererJNI.h"
#include "VideoRendererContext.h"

#include <android/native_window_jni.h>
#include <Log.h>

JCMCPRV(void, create)(JNIEnv * env, jobject obj, jint type)
{
	LOGE("create");
	VideoRendererContext::createContext(env, obj, type);
}

JCMCPRV(void, destroy)(JNIEnv * env, jobject obj)
{
	LOGE("destroy");
	VideoRendererContext::deleteContext(env, obj);
}

JCMCPRV(void, init)(JNIEnv * env, jobject obj, jobject surface, jint width, jint height,jobject assetManager)
{
	LOGE("init");
	VideoRendererContext* context = VideoRendererContext::getContext(env, obj);

	ANativeWindow *window = surface ? ANativeWindow_fromSurface(env, surface) : nullptr;

	AAssetManager*manager = AAssetManager_fromJava(env,assetManager);

    if (context) context->init(window, (size_t)width, (size_t)height, manager);
}

JCMCPRV(void, render)(JNIEnv * env, jobject obj)
{
	LOGE("render");
	VideoRendererContext* context = VideoRendererContext::getContext(env, obj);

	if (context) context->render();
}

JCMCPRV(void, draw)(JNIEnv * env, jobject obj, jbyteArray data, jint width, jint height, jint rotation)
{
	LOGE("draw");
	jbyte* bufferPtr = env->GetByteArrayElements(data, 0);

	jsize arrayLength = env->GetArrayLength(data);

	VideoRendererContext* context = VideoRendererContext::getContext(env, obj);

	if (context) context->draw((uint8_t *)bufferPtr, (size_t)arrayLength, (size_t)width, (size_t)height, rotation);

	env->ReleaseByteArrayElements(data, bufferPtr, 0);
}

JCMCPRV(void, setParameters)(JNIEnv * env, jobject obj, jint params)
{
	LOGE("setParameters");
	VideoRendererContext* context = VideoRendererContext::getContext(env, obj);

	if (context) context->setParameters((uint32_t)params);
}

JCMCPRV(void, setProgress)(JNIEnv * env, jobject obj, jint params)
{
	LOGE("setParameters");
	VideoRendererContext* context = VideoRendererContext::getContext(env, obj);

	if (context) context->setProcess((uint32_t)params);
}

JCMCPRV(jint, getParameters)(JNIEnv * env, jobject obj)
{
	LOGE("getParameters");
	VideoRendererContext* context = VideoRendererContext::getContext(env, obj);

	if (context) return context->getParameters();

	return 0;
}
