//
// Created by admin on 2021/1/23.
//


extern "C"{
#include "j4a/j4a_allclasses.include.h"
}
#include "CMediaCodec.h"

jobject CMediaCodec::createByCodecName(JNIEnv *env, const char *codec_name)
{
    jobject android_mediacodec = J4AC_MediaCodec__createByCodecName__withCString__catchAll(env, codec_name);
    if (!android_mediacodec) {
        return NULL;
    }

    android_mediacodec = android_mediacodec;
    JNI_DeleteLocalRefP(env, &android_mediacodec);
    return android_mediacodec;
}

jobject CMediaCodec::createByType(JNIEnv *env, const char *codec_name)
{
    jobject android_mediacodec = J4AC_MediaCodec__createByCodecName__withCString__catchAll(env, codec_name);
    if (!android_mediacodec) {
        return NULL;
    }

    android_mediacodec = android_mediacodec;
    JNI_DeleteLocalRefP(env, &android_mediacodec);
    return android_mediacodec;
}

jobject CMediaCodec::getAndroidCodec()
{
    return android_mediacodec;
}

jobject CMediaCodec::getOutputFormat()
{
    JNIEnv *env = NULL;
    if (JNI_OK != JNI_SetupThreadEnv(&env)) {
        XLOGE("%s: SetupThreadEnv failed", __func__);
        return NULL;
    }
    jobject local_android_format = J4AC_MediaCodec__getOutputFormat__catchAll(env, android_mediacodec);
    if (!local_android_format) {
        return NULL;
    }

//TODO do this format
    //SDL_AMediaFormat *aformat = SDL_AMediaFormatJava_init(env, local_android_format);
    JNI_DeleteLocalRefP(env, &local_android_format);
    return nullptr;
}

CMediaCodec::~CMediaCodec()
{
    XLOGI("%s\n", __func__);
    JNIEnv *env = NULL;
    if (JNI_OK != JNI_SetupThreadEnv(&env)) {
        XLOGE("SDL_AMediaCodecJava_delete: SetupThreadEnv failed");
        return;
    }
    if (android_mediacodec) {
        J4AC_MediaCodec__release__catchAll(env, android_mediacodec);
    }
    if (android_input_mediaformat) {
        JNI_DeleteGlobalRefP(env, &android_input_mediaformat);
    }
    if (output_buffer_info) {
        JNI_DeleteGlobalRefP(env, &output_buffer_info);
    }
    if (android_mediacodec) {
        JNI_DeleteGlobalRefP(env, &android_mediacodec);
    }
}

int CMediaCodec::configureSurface(
        JNIEnv*env,
        jobject android_surface,
        jobject crypto,
        uint32_t flags)
{
    XLOGD("%s", __func__);

    XLOGE("configure acodec:%p format:%p: surface:%p", android_mediacodec, android_input_mediaformat, android_surface);
    J4AC_MediaCodec__configure(env, android_mediacodec, android_input_mediaformat, android_surface, crypto, flags);
    if (J4A_ExceptionCheck__catchAll(env)) {
        return -1;
    }

    is_input_buffer_valid = true;
    return 0;
}

int CMediaCodec::SDL_AMediaCodecJava_start()
{
    XLOGD("%s", __func__);

    JNIEnv *env = NULL;
    if (JNI_OK != JNI_SetupThreadEnv(&env)) {
        XLOGE("%s: SetupThreadEnv failed", __func__);
        return -1;
    }

    J4AC_MediaCodec__start(env, android_mediacodec);
    if (J4A_ExceptionCheck__catchAll(env)) {
        XLOGE("%s: start failed", __func__);
        return -1;
    }

    return 0;
}

int CMediaCodec::SDL_AMediaCodecJava_stop()
{
    XLOGD("%s", __func__);

    JNIEnv *env = NULL;
    if (JNI_OK != JNI_SetupThreadEnv(&env)) {
        XLOGE("%s: SetupThreadEnv failed", __func__);
        return -1;
    }
    J4AC_MediaCodec__stop(env, android_mediacodec);
    if (J4A_ExceptionCheck__catchAll(env)) {
        XLOGE("%s: stop", __func__);
        return -1;
    }

    return 0;
}

int CMediaCodec::SDL_AMediaCodecJava_flush()
{
    XLOGD("%s", __func__);

    JNIEnv *env = NULL;
    if (JNI_OK != JNI_SetupThreadEnv(&env)) {
        XLOGE("%s: SetupThreadEnv failed", __func__);
        return -1;
    }

    J4AC_MediaCodec__flush(env, android_mediacodec);
    if (J4A_ExceptionCheck__catchAll(env)) {
        XLOGE("%s: flush", __func__);
        return -1;
    }

    return 0;
}

int CMediaCodec::SDL_AMediaCodecJava_writeInputData(size_t idx, const uint8_t *data, size_t size)
{
    XLOGD("%s", __func__);
    ssize_t write_ret = -1;
    jobject input_buffer_array = NULL;
    jobject input_buffer = NULL;

    JNIEnv *env = NULL;
    if (JNI_OK != JNI_SetupThreadEnv(&env)) {
        XLOGE("%s: SetupThreadEnv failed", __func__);
        return -1;
    }

    input_buffer_array = J4AC_MediaCodec__getInputBuffers__catchAll(env, android_mediacodec);
    if (!input_buffer_array)
        return -1;

    int buffer_count = env->GetArrayLength((jobjectArray)input_buffer_array);
    if (J4A_ExceptionCheck__catchAll(env) || idx < 0 || idx >= buffer_count) {
        XLOGE("%s: idx(%d) < count(%d)\n", __func__, (int)idx, (int)buffer_count);
        goto fail;
    }

    input_buffer = env->GetObjectArrayElement((jobjectArray)input_buffer_array, idx);
    if (J4A_ExceptionCheck__catchAll(env) || !input_buffer) {
        XLOGE("%s: GetObjectArrayElement failed\n", __func__);
        goto fail;
    }

    {
        jlong buf_size = env->GetDirectBufferCapacity(input_buffer);
        void *buf_ptr  = env->GetDirectBufferAddress(input_buffer);

        write_ret = size < buf_size ? size : buf_size;
        memcpy(buf_ptr, data, write_ret);
    }

    fail:
    JNI_DeleteLocalRefP(env, &input_buffer);
    JNI_DeleteLocalRefP(env, &input_buffer_array);
    return write_ret;
}

int  CMediaCodec::SDL_AMediaCodecJava_dequeueInputBuffer(int64_t timeoutUs)
{
    XLOGD("%s(%d)", __func__, (int)timeoutUs);

    JNIEnv *env = NULL;
    if (JNI_OK != JNI_SetupThreadEnv(&env)) {
        XLOGE("%s: SetupThreadEnv failed", __func__);
        return -1;
    }

    //SDL_AMediaCodec_Opaque *opaque = (SDL_AMediaCodec_Opaque *)acodec->opaque;
    // docs lie, getInputBuffers should be good after
    // m_codec->start() but the internal refs are not
    // setup until much later on some devices.
    //if (-1 == getInputBuffers(env, acodec)) {
    //    XLOGE("%s: getInputBuffers failed", __func__);
    //    return -1;
    //}

    jint idx = J4AC_MediaCodec__dequeueInputBuffer(env, android_mediacodec, (jlong)timeoutUs);
    if (J4A_ExceptionCheck__catchAll(env)) {
        XLOGE("%s: dequeueInputBuffer failed", __func__);
        is_input_buffer_valid = false;
        return -1;
    }

    return idx;
}

int  CMediaCodec::SDL_AMediaCodecJava_queueInputBuffer(size_t idx, off_t offset, size_t size, uint64_t time, uint32_t flags)
{
    XLOGD("%s: %d", __func__, (int)idx);

    JNIEnv *env = NULL;
    if (JNI_OK != JNI_SetupThreadEnv(&env)) {
        XLOGE("SDL_AMediaCodecJava_queueInputBuffer: SetupThreadEnv failed");
        return -1;
    }

    J4AC_MediaCodec__queueInputBuffer(env, android_mediacodec, (jint)idx, (jint)offset, (jint)size, (jlong)time, (jint)flags);
    if (J4A_ExceptionCheck__catchAll(env)) {
        return -1;
    }

    return 0;
}

int CMediaCodec::SDL_AMediaCodecJava_dequeueOutputBuffer(int64_t timeoutUs)
{
    XLOGD("%s(%d)", __func__, (int)timeoutUs);

    JNIEnv *env = NULL;
    if (JNI_OK != JNI_SetupThreadEnv(&env)) {
        XLOGE("%s: SetupThreadEnv failed", __func__);
        return -1;
    }

    if (!output_buffer_info) {
        output_buffer_info = J4AC_MediaCodec__BufferInfo__BufferInfo__asGlobalRef__catchAll(env);
        if (!output_buffer_info)
            return -1;
    }

    jint idx = -1;
    while (1) {
        idx = J4AC_MediaCodec__dequeueOutputBuffer(env, android_mediacodec, output_buffer_info, (jlong)timeoutUs);
        if (J4A_ExceptionCheck__catchAll(env)) {
            XLOGI("%s: Exception\n", __func__);
            return -1;
        }
        if (idx == AMEDIACODEC__INFO_OUTPUT_BUFFERS_CHANGED) {
            XLOGI("%s: INFO_OUTPUT_BUFFERS_CHANGED\n", __func__);
            continue;
        } else if (idx == AMEDIACODEC__INFO_OUTPUT_FORMAT_CHANGED) {
            XLOGI("%s: INFO_OUTPUT_FORMAT_CHANGED\n", __func__);
        } else if (idx >= 0) {
            XLOGD("%s: buffer ready (%d) ====================\n", __func__, idx);
            info.offset              = J4AC_MediaCodec__BufferInfo__offset__get__catchAll(env, output_buffer_info);
            info.size                = J4AC_MediaCodec__BufferInfo__size__get__catchAll(env, output_buffer_info);
            info.presentationTimeUs  = J4AC_MediaCodec__BufferInfo__presentationTimeUs__get__catchAll(env, output_buffer_info);
            info.flags               = J4AC_MediaCodec__BufferInfo__flags__get__catchAll(env, output_buffer_info);
        }
        break;
    }

    return idx;
}

int CMediaCodec::SDL_AMediaCodecJava_releaseOutputBuffer(size_t idx, bool render)
{
    XLOGD("%s", __func__);

    JNIEnv *env = NULL;
    if (JNI_OK != JNI_SetupThreadEnv(&env)) {
        XLOGE("%s(%d, %s): SetupThreadEnv failed", __func__, (int)idx, render ? "true" : "false");
        return -1;
    }

    J4AC_MediaCodec__releaseOutputBuffer(env, android_mediacodec, (jint)idx, (jboolean)render);
    if (J4A_ExceptionCheck__catchAll(env)) {
        XLOGE("%s: releaseOutputBuffer\n", __func__);
        return -1;
    }

    return 0;
}

int CMediaCodec::release() {
    XLOGD("%s", __func__);
    JNIEnv *env = NULL;
    if (JNI_OK != JNI_SetupThreadEnv(&env)) {
        XLOGE("%s: SetupThreadEnv failed", __func__);
        return -1;
    }
    J4AC_MediaCodec__release__catchAll(env,android_mediacodec);
    return 0;
}