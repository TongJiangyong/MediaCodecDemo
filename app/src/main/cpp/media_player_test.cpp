
#include <string>
#include <android/native_window_jni.h>
#include <thread>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <mutex>
#include "j4a/j4a_base.h"
#include "media_player_test.h"
#include "AndroidMediaCodec.h"


#define MEDIACODEC_PREFIX yong_mediacodecdemo_MediaCodecSync
#define CONCAT1(prefix, class, function) CONCAT2(prefix, class, function)
#define CONCAT2(prefix, class, function) Java_##prefix##_##class##_##function
#define MEDIACODEC_JAVA_INTERFACE(function) CONCAT1(MEDIACODEC_PREFIX, MediaCodecSyncTest, function)
static AndroidMediaCodec* codec_ = nullptr;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL MEDIACODEC_JAVA_INTERFACE(createDecoderByType)(JNIEnv *env, jobject instance, jstring type) {

    if (!codec_) {
        codec_ = new AndroidMediaCodec();
    }
    const char *type_string = nullptr;
    type_string = env->GetStringUTFChars(type, 0);
    XLOGD("createDecoderByType %s",type_string);
    codec_->createDecoderByType(env,type_string);
    env->ReleaseStringUTFChars(type, type_string);
    return;
}

JNIEXPORT void JNICALL Java_yong_mediacodecdemo_MainActivity_testA(JNIEnv *env, jobject instance) {
    XLOGD("testA");
}

JNIEXPORT void JNICALL MEDIACODEC_JAVA_INTERFACE(configure)(JNIEnv *env, jobject instance,jobject media_format, jobject surface, jobject crypto, jint flags) {
    XLOGD("configure");
    if (!codec_) {
        XLOGD("configure error codec_ is null");
        return;
    }
    codec_->configureSurface(env,media_format,surface,crypto,flags);
    return;
}

JNIEXPORT void JNICALL MEDIACODEC_JAVA_INTERFACE(startCodec)(JNIEnv *env, jobject instance) {
    XLOGD("startCodec");
    if (!codec_) {
        XLOGD("startCodec error codec_ is null");
        return;
    }
    codec_->start(env);
    return;
}

JNIEXPORT void JNICALL MEDIACODEC_JAVA_INTERFACE(stopCodec)(JNIEnv *env, jobject instance) {
    XLOGD("stopCodec");
    if (!codec_) {
        XLOGD("stopCodec error codec_ is null");
        return;
    }
    codec_->stop(env);
    return;
}

JNIEXPORT void JNICALL MEDIACODEC_JAVA_INTERFACE(releaseCodec)(JNIEnv *env, jobject instance) {
    XLOGD("releaseCodec");
    if (!codec_) {
        XLOGD("releaseCodec error codec_ is null");
        return;
    }
    codec_->release(env);
    return;
}

JNIEXPORT int JNICALL MEDIACODEC_JAVA_INTERFACE(dequeueInputBuffer)(JNIEnv *env, jobject instance,jlong timeoutUs) {
    XLOGD("dequeueInputBuffer");
    if (!codec_) {
        XLOGD("dequeueInputBuffer error codec_ is null");
        return -1;
    }
    int index = codec_->dequeueInputBufferIndex(env,timeoutUs);
    XLOGD("dequeueInputBuffer index:%d",index);
    return index;
}

JNIEXPORT void JNICALL MEDIACODEC_JAVA_INTERFACE(queueInputBuffer)(JNIEnv *env, jobject instance,jint index, jint offset, jint size, jlong presentationTimeUs, jint flags,jobject inputBuffer) {
    XLOGD("queueInputBuffer");
    if (!codec_) {
        XLOGD("queueInputBuffer error codec_ is null");
        return;
    }
    void *direct_video_buffer = env->GetDirectBufferAddress(inputBuffer);
    codec_->fillInputBufferByIndex(env,index, (const uint8_t *)direct_video_buffer, size);
    codec_->queueInputBufferByIndex(env,index,offset,size,presentationTimeUs,flags);
    return;
}

JNIEXPORT jlongArray  JNICALL MEDIACODEC_JAVA_INTERFACE(dequeueOutputBuffer)(JNIEnv *env, jobject instance,jlong timeoutUs) {
    XLOGD("dequeueOutputBuffer");
    jlongArray jlongarray = env->NewLongArray(2);
    jlong * jlongp = env->GetLongArrayElements(jlongarray, nullptr);
    if (!codec_) {
        XLOGD("dequeueOutputBuffer error codec_ is null");
        return nullptr;
    }
    AMediaCodecBufferInfo info;
    int index = codec_->dequeueOutputBufferIndex(env,info,timeoutUs);
    jlongp[0] = index;
    jlongp[1] = info.presentationTimeUs;
    env->ReleaseLongArrayElements(jlongarray, jlongp, 0);
    XLOGD("dequeueOutputBuffer index:%d",index);
    return jlongarray;
}

JNIEXPORT void JNICALL MEDIACODEC_JAVA_INTERFACE(releaseOutputBuffer)(JNIEnv *env, jobject instance,jint index, jboolean render) {
    XLOGD("releaseOutputBuffer");
    if (!codec_) {
        XLOGD("releaseOutputBuffer error codec_ is null");
        return;
    }
    codec_->releaseOutputBufferByIndex(env,index,render);
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
    }
    return JNI_VERSION_1_4;
}

#ifdef __cplusplus
}
#endif
