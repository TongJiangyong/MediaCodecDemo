
#include <string>
#include <android/native_window_jni.h>
#include <thread>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <mutex>
#include "j4a/j4a_base.h"
#include "media_player_test.h"
#include "CMediaCodec.h"


#define MEDIACODEC_PREFIX yong_mediacodecdemo_MediaCodecSync
#define CONCAT1(prefix, class, function) CONCAT2(prefix, class, function)
#define CONCAT2(prefix, class, function) Java_##prefix##_##class##_##function
#define MEDIACODEC_JAVA_INTERFACE(function) CONCAT1(MEDIACODEC_PREFIX, MediaCodecSyncTest, function)
static CMediaCodec* codec_ = nullptr;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL MEDIACODEC_JAVA_INTERFACE(createDecoderByType)(JNIEnv *env, jobject instance, jstring type) {

    if (!codec_) {
        codec_ = new CMediaCodec();
    }
    const char *type_string = nullptr;
    type_string = env->GetStringUTFChars(type, 0);
    XLOGD("createDecoderByType %s",type_string);
    codec_->createByCodecName(env,type_string);
    env->ReleaseStringUTFChars(type, type_string);
    return;
}

JNIEXPORT void JNICALL Java_yong_mediacodecdemo_MainActivity_testA(JNIEnv *env, jobject instance) {
    XLOGD("testA");
}

JNIEXPORT void JNICALL MEDIACODEC_JAVA_INTERFACE(configure)(JNIEnv *env, jobject instance,jobject surface, jobject crypto, jint flags) {
    XLOGD("configure");
    if (!codec_) {
        XLOGD("configure error codec_ is null");
        return;
    }
    codec_->configureSurface(env,surface,crypto,flags);
    return;
}

JNIEXPORT void JNICALL MEDIACODEC_JAVA_INTERFACE(startCodec)(JNIEnv *env, jobject instance) {
    XLOGD("startCodec");
    if (!codec_) {
        XLOGD("startCodec error codec_ is null");
        return;
    }
    codec_->SDL_AMediaCodecJava_start();
    return;
}

JNIEXPORT void JNICALL MEDIACODEC_JAVA_INTERFACE(stopCodec)(JNIEnv *env, jobject instance) {
    XLOGD("stopCodec");
    if (!codec_) {
        XLOGD("stopCodec error codec_ is null");
        return;
    }
    codec_->SDL_AMediaCodecJava_stop();
    return;
}

JNIEXPORT void JNICALL MEDIACODEC_JAVA_INTERFACE(releaseCodec)(JNIEnv *env, jobject instance) {
    XLOGD("releaseCodec");
    if (!codec_) {
        XLOGD("releaseCodec error codec_ is null");
        return;
    }
    codec_->release();
    return;
}

JNIEXPORT int JNICALL MEDIACODEC_JAVA_INTERFACE(dequeueInputBuffer)(JNIEnv *env, jobject instance,jlong timeoutUs) {
    XLOGD("dequeueInputBuffer");
    if (!codec_) {
        XLOGD("dequeueInputBuffer error codec_ is null");
        return -1;
    }
    int index = codec_->SDL_AMediaCodecJava_dequeueInputBuffer(timeoutUs);
    XLOGD("dequeueInputBuffer index:%d",index);
    return 0;
}

JNIEXPORT void JNICALL MEDIACODEC_JAVA_INTERFACE(queueInputBuffer)(JNIEnv *env, jobject instance,jint index, jint offset, jint size, jlong presentationTimeUs, jint flags) {
    XLOGD("queueInputBuffer");
    if (!codec_) {
        XLOGD("queueInputBuffer error codec_ is null");
        return;
    }
    codec_->SDL_AMediaCodecJava_queueInputBuffer(index,offset,size,presentationTimeUs,flags);
    return;
}

JNIEXPORT int JNICALL MEDIACODEC_JAVA_INTERFACE(dequeueOutputBuffer)(JNIEnv *env, jobject instance,jlong timeoutUs) {
    XLOGD("dequeueOutputBuffer");
    if (!codec_) {
        XLOGD("dequeueOutputBuffer error codec_ is null");
        return -1;
    }
    int index = codec_->SDL_AMediaCodecJava_dequeueOutputBuffer(timeoutUs);
    XLOGD("dequeueOutputBuffer index:%d",index);
    return 0;
}

JNIEXPORT void JNICALL MEDIACODEC_JAVA_INTERFACE(releaseOutputBuffer)(JNIEnv *env, jobject instance,jint index, jboolean render) {
    XLOGD("releaseOutputBuffer");
    if (!codec_) {
        XLOGD("releaseOutputBuffer error codec_ is null");
        return;
    }
    codec_->SDL_AMediaCodecJava_releaseOutputBuffer(index,render);
    return;
}

jint JNIEXPORT JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved) {
    XLOGE("TJY JNI_OnLoad");
    JNIEnv *jni_env = nullptr;
    g_jvm = jvm;

    int status = jvm->GetEnv((void **)&jni_env, JNI_VERSION_1_4);

    if (jni_env) {
        J4A_loadClass__J4AC_android_os_Build(jni_env);
        J4A_loadClass__J4AC_android_os_Bundle(jni_env);
        J4A_loadClass__J4AC_android_media_MediaCodec(jni_env);
        J4A_loadClass__J4AC_java_nio_Buffer(jni_env);
        J4A_loadClass__J4AC_java_nio_ByteBuffer(jni_env);
        J4A_loadClass__J4AC_java_util_ArrayList(jni_env);
        J4A_loadClass__J4AC_android_media_MediaFormat(jni_env);
        J4A_loadClass__J4AC_android_media_PlaybackParams(jni_env);
    }
    return JNI_VERSION_1_4;
}

#ifdef __cplusplus
}
#endif
