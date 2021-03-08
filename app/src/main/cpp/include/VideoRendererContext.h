#ifndef _H_VIDEO_RENDERER_CONTEXT_
#define _H_VIDEO_RENDERER_CONTEXT_

#include "VideoRenderer.h"

#include <memory>
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

class VideoRendererContext
{
public:
	struct jni_fields_t
	{
		jfieldID context;
	};

	VideoRendererContext(int type);
	~VideoRendererContext();

    void init(ANativeWindow* window, size_t width, size_t height,AAssetManager* manager);
	void render();
	void draw(uint8_t *buffer, size_t length, size_t width, size_t height, int rotation);
	void setParameters(uint32_t params);
	void setProcess(uint32_t params);
	uint32_t getParameters();

	static void createContext(JNIEnv *env, jobject obj, jint type);
	static void storeContext(JNIEnv *env, jobject obj, VideoRendererContext *context);
	static void deleteContext(JNIEnv *env, jobject obj);
	static VideoRendererContext* getContext(JNIEnv *env, jobject obj);

	AAssetManager* manager ;

private:
    std::unique_ptr<VideoRenderer> m_pVideoRenderer;

	static jni_fields_t jni_fields;

};

////
//// AAssetManager*  VideoRendererContext::manager = nullptr ;
////
//
//AAssetManager* VideoRendererContext::manager = nullptr;

#endif // _H_VIDEO_RENDERER_CONTEXT_
