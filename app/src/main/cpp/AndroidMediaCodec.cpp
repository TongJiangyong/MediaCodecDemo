//
// Created by admin on 2021/1/23.
//


extern "C"{
#include "j4a/j4a_allclasses.include.h"
}
#include "AndroidMediaCodec.h"

bool AndroidMediaCodec::checkJNIEnv(JNIEnv *env) {
    if (env == nullptr) {
        if (JNI_OK != JNI_SetupThreadEnv(&env)) {
            XLOGE("%s: SetupThreadEnv failed", __func__);
            return false;
        }
    }
    return true;
}

jobject AndroidMediaCodec::createByCodecName(JNIEnv *env, const char *codec_name)
{
    if(!checkJNIEnv(env)) {
        return nullptr;
    }
    jobject android_mediacodec = J4AC_MediaCodec__createByCodecName__withCString__catchAll(env, codec_name);
    if (!android_mediacodec) {
        return nullptr;
    }

    android_mediacodec_ = J4A_NewGlobalRef__catchAll(env, android_mediacodec);
    JNI_DeleteLocalRefP(env, &android_mediacodec);
    return android_mediacodec_;
}

jobject AndroidMediaCodec::createDecoderByType(JNIEnv *env, const char *codec_type)
{
    if(!checkJNIEnv(env)) {
        return nullptr;
    }
    jobject android_mediacodec = J4AC_MediaCodec__createDecoderByType__withCString__catchAll(env, codec_type);
    if (!android_mediacodec) {
        return nullptr;
    }

    android_mediacodec_ = J4A_NewGlobalRef__catchAll(env, android_mediacodec);
    JNI_DeleteLocalRefP(env, &android_mediacodec);
    return android_mediacodec_;
}

jobject AndroidMediaCodec::getAndroidCodec()
{
    return android_mediacodec_;
}

jobject AndroidMediaCodec::getOutputFormat(JNIEnv *env)
{
    if(!checkJNIEnv(env)) {
        return nullptr;
    }
    jobject ret_object   = nullptr;
    jobject local_android_format = J4AC_MediaCodec__getOutputFormat__catchAll(env, android_mediacodec_);
    ret_object = J4A_NewGlobalRef__catchAll(env, local_android_format);
    if (!local_android_format) {
        return nullptr;
    }
    JNI_DeleteLocalRefP(env, &local_android_format);
    return ret_object;
}

AndroidMediaCodec::~AndroidMediaCodec()
{

}

int AndroidMediaCodec::configureSurface(
        JNIEnv*env,
        jobject media_format,
        jobject android_surface,
        jobject crypto,
        uint32_t flags)
{
    XLOGD("%s", __func__);
    if(!checkJNIEnv(env)) {
        return AMEDIACODEC__ENV_ERROR;
    }

    J4AC_MediaCodec__configure(env, android_mediacodec_, media_format, android_surface, crypto, flags);
    if (J4A_ExceptionCheck__catchAll(env)) {
        return AMEDIACODEC__EXCEPTION_ERROR;
    }

    is_input_buffer_valid_ = true;
    return AMEDIACODEC__OK;
}

int AndroidMediaCodec::start(JNIEnv *env)
{
    // XLOGD("%s", __func__);
    if(!checkJNIEnv(env)) {
        return AMEDIACODEC__ENV_ERROR;
    }

    J4AC_MediaCodec__start(env, android_mediacodec_);
    if (J4A_ExceptionCheck__catchAll(env)) {
        XLOGE("%s: start failed", __func__);
        return AMEDIACODEC__EXCEPTION_ERROR;
    }

    return AMEDIACODEC__OK;
}

int AndroidMediaCodec::stop(JNIEnv *env)
{
    // XLOGD("%s", __func__);

    if(!checkJNIEnv(env)) {
        return AMEDIACODEC__ENV_ERROR;
    }
    J4AC_MediaCodec__stop(env, android_mediacodec_);
    if (J4A_ExceptionCheck__catchAll(env)) {
        XLOGE("%s: stop", __func__);
        return AMEDIACODEC__EXCEPTION_ERROR;
    }

    return AMEDIACODEC__OK;
}

int AndroidMediaCodec::flush(JNIEnv *env)
{
    // XLOGD("%s", __func__);

    if(!checkJNIEnv(env)) {
        return AMEDIACODEC__ENV_ERROR;
    }

    J4AC_MediaCodec__flush(env, android_mediacodec_);
    if (J4A_ExceptionCheck__catchAll(env)) {
        XLOGE("%s: flush", __func__);
        return AMEDIACODEC__EXCEPTION_ERROR;
    }

    return AMEDIACODEC__OK;
}

int AndroidMediaCodec::fillInputBufferByIndex(JNIEnv *env,size_t idx, const uint8_t *data, size_t size)
{
    // XLOGD("%s", __func__);
    ssize_t write_ret = -1;
    jobject input_buffer_array = nullptr;
    jobject input_buffer = nullptr;

    if(!checkJNIEnv(env)) {
        return AMEDIACODEC__ENV_ERROR;
    }

    input_buffer_array = J4AC_MediaCodec__getInputBuffers__catchAll(env, android_mediacodec_);
    if (!input_buffer_array)
        return AMEDIACODEC__BYTE_ARRAY_ERROR;

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

int  AndroidMediaCodec::dequeueInputBufferIndex(JNIEnv *env,int64_t timeoutUs)
{
    // XLOGD("%s(%d)", __func__, (int)timeoutUs);

    if(!checkJNIEnv(env)) {
        return AMEDIACODEC__ENV_ERROR;
    }

    //SDL_AMediaCodec_Opaque *opaque = (SDL_AMediaCodec_Opaque *)acodec->opaque;
    // docs lie, getInputBuffers should be good after
    // m_codec->start() but the internal refs are not
    // setup until much later on some devices.
    //if (-1 == getInputBuffers(env, acodec)) {
    //    XLOGE("%s: getInputBuffers failed", __func__);
    //    return -1;
    //}

    jint idx = J4AC_MediaCodec__dequeueInputBuffer(env, android_mediacodec_, (jlong)timeoutUs);
    if (J4A_ExceptionCheck__catchAll(env)) {
        XLOGE("%s: dequeueInputBuffer failed", __func__);
        is_input_buffer_valid_ = false;
        return AMEDIACODEC__EXCEPTION_ERROR;
    }

    return idx;
}

int AndroidMediaCodec::getOutputBufferByIndex(JNIEnv *env,size_t idx, uint8_t *data, size_t size)
{
    // XLOGD("%s", __func__);
    ssize_t write_ret = -1;
    jobject output_buffer_array = nullptr;
    jobject output_buffer = nullptr;

    if(!checkJNIEnv(env)) {
        return AMEDIACODEC__ENV_ERROR;
    }

    output_buffer_array = J4AC_MediaCodec__getOutputBuffers__catchAll(env, android_mediacodec_);
    if (!output_buffer_array)
        return AMEDIACODEC__BYTE_ARRAY_ERROR;

    int buffer_count = env->GetArrayLength((jobjectArray)output_buffer_array);
    if (J4A_ExceptionCheck__catchAll(env) || idx < 0 || idx >= buffer_count) {
        XLOGE("%s: idx(%d) < count(%d)\n", __func__, (int)idx, (int)buffer_count);
        goto fail;
    }

    output_buffer = env->GetObjectArrayElement((jobjectArray)output_buffer_array, idx);
    if (J4A_ExceptionCheck__catchAll(env) || !output_buffer) {
        XLOGE("%s: GetObjectArrayElement failed\n", __func__);
        goto fail;
    }

    {
        jlong buf_size = env->GetDirectBufferCapacity(output_buffer);
        void *buf_ptr  = env->GetDirectBufferAddress(output_buffer);

        write_ret = size < buf_size ? size : buf_size;
        memcpy(data, buf_ptr, write_ret);
    }

    fail:
    JNI_DeleteLocalRefP(env, &output_buffer);
    JNI_DeleteLocalRefP(env, &output_buffer_array);
    return write_ret;
}

int  AndroidMediaCodec::queueInputBufferByIndex(JNIEnv *env,size_t idx, off_t offset, size_t size, uint64_t time, uint32_t flags)
{
    // XLOGD("%s: %d", __func__, (int)idx);

    if(!checkJNIEnv(env)) {
        return AMEDIACODEC__ENV_ERROR;
    }

    J4AC_MediaCodec__queueInputBuffer(env, android_mediacodec_, (jint)idx, (jint)offset, (jint)size, (jlong)time, (jint)flags);
    if (J4A_ExceptionCheck__catchAll(env)) {
        return AMEDIACODEC__EXCEPTION_ERROR;
    }

    return AMEDIACODEC__OK;
}

int AndroidMediaCodec::dequeueOutputBufferIndex(JNIEnv *env,AMediaCodecBufferInfo &info,int64_t timeoutUs)
{
    // XLOGD("%s(%d)", __func__, (int)timeoutUs);

    if(!checkJNIEnv(env)) {
        return AMEDIACODEC__ENV_ERROR;
    }

    if (!output_buffer_info_) {
        output_buffer_info_ = J4AC_MediaCodec__BufferInfo__BufferInfo__asGlobalRef__catchAll(env);
        if (!output_buffer_info_)
            return AMEDIACODEC__OUTPUT_BUFFER_ERROR;
    }

    jint idx = -1;
    while (1) {
        idx = J4AC_MediaCodec__dequeueOutputBuffer(env, android_mediacodec_, output_buffer_info_, (jlong)timeoutUs);
        if (J4A_ExceptionCheck__catchAll(env)) {
            XLOGI("%s: Exception\n", __func__);
            return AMEDIACODEC__EXCEPTION_ERROR;
        }
        if (idx == AMEDIACODEC__INFO_OUTPUT_BUFFERS_CHANGED) {
            XLOGI("%s: INFO_OUTPUT_BUFFERS_CHANGED\n", __func__);
            continue;
        } else if (idx == AMEDIACODEC__INFO_OUTPUT_FORMAT_CHANGED) {
            XLOGI("%s: INFO_OUTPUT_FORMAT_CHANGED\n", __func__);
        } else if (idx >= 0) {
            XLOGD("%s: buffer ready (%d) ====================\n", __func__, idx);
            info.offset              = J4AC_MediaCodec__BufferInfo__offset__get__catchAll(env, output_buffer_info_);
            info.size                = J4AC_MediaCodec__BufferInfo__size__get__catchAll(env, output_buffer_info_);
            info.presentationTimeUs  = J4AC_MediaCodec__BufferInfo__presentationTimeUs__get__catchAll(env, output_buffer_info_);
            info.flags               = J4AC_MediaCodec__BufferInfo__flags__get__catchAll(env, output_buffer_info_);
        }
        break;
    }

    return idx;
}

int AndroidMediaCodec::releaseOutputBufferByIndex(JNIEnv *env,size_t idx, bool render)
{
    // XLOGD("%s", __func__);

    if(!checkJNIEnv(env)) {
        return AMEDIACODEC__ENV_ERROR;
    }

    J4AC_MediaCodec__releaseOutputBuffer(env, android_mediacodec_, (jint)idx, (jboolean)render);
    if (J4A_ExceptionCheck__catchAll(env)) {
        XLOGE("%s: releaseOutputBuffer\n", __func__);
        return AMEDIACODEC__EXCEPTION_ERROR;
    }

    return AMEDIACODEC__OK;
}

int AndroidMediaCodec::release(JNIEnv *env) {
    XLOGD("%s", __func__);
    if(!checkJNIEnv(env)) {
        return AMEDIACODEC__ENV_ERROR;
    }

    if (android_mediacodec_) {
        J4AC_MediaCodec__release__catchAll(env, android_mediacodec_);
        JNI_DeleteGlobalRefP(env, &android_mediacodec_);
        android_mediacodec_ = nullptr;
    }
    if (output_buffer_info_) {
        JNI_DeleteGlobalRefP(env, &output_buffer_info_);
        output_buffer_info_ = nullptr;
    }
    is_input_buffer_valid_ = false;
    return AMEDIACODEC__OK;
}