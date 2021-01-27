//
// Created by admin on 2021/1/23.
//

#ifndef MEDIACODECDEMO_CMEDIACODEC_H
#define MEDIACODECDEMO_CMEDIACODEC_H
#include <jni.h>

#include "JniHandler.h"
typedef struct AMediaCodecBufferInfo {
    int32_t offset;
    int32_t size;
    int64_t presentationTimeUs;
    uint32_t flags;
} AMediaCodecBufferInfo;
enum {
    AMEDIACODEC__INFO_OUTPUT_BUFFERS_CHANGED = -3,
    AMEDIACODEC__INFO_OUTPUT_FORMAT_CHANGED  = -2,
    AMEDIACODEC__INFO_TRY_AGAIN_LATER        = -1,

    AMEDIACODEC__UNKNOWN_ERROR               = -1000,
};
class CMediaCodec {
public:
    jobject createByCodecName(JNIEnv *env, const char *codec_name);
    jobject createDecoderByType(JNIEnv *env, const char *codec_type);
    jobject getAndroidCodec();
    jobject getOutputFormat(JNIEnv *env);
    ~CMediaCodec();
    int configureSurface(
            JNIEnv*env,
            jobject media_format,
            jobject android_surface,
            jobject crypto,
            uint32_t flags);
    int SDL_AMediaCodecJava_start(JNIEnv *env);
    int SDL_AMediaCodecJava_stop(JNIEnv *env);
    int SDL_AMediaCodecJava_flush(JNIEnv *env);
    int SDL_AMediaCodecJava_writeInputData(JNIEnv *env,size_t idx, const uint8_t *data, size_t size);
    int SDL_AMediaCodecJava_dequeueInputBuffer(JNIEnv *env,int64_t timeoutUs);
    int SDL_AMediaCodecJava_queueInputBuffer(JNIEnv *env,size_t idx, off_t offset, size_t size, uint64_t time, uint32_t flags);
    int SDL_AMediaCodecJava_dequeueOutputBuffer(JNIEnv *env,int64_t timeoutUs);
    int SDL_AMediaCodecJava_releaseOutputBuffer(JNIEnv *env,size_t idx, bool render);
    int release(JNIEnv *env);
private:
    jobject android_mediacodec_ = nullptr;
    jobject output_buffer_info_ = nullptr;
    AMediaCodecBufferInfo info_;
    jobject android_input_mediaformat_ = nullptr;
    jobject android_surface_ = nullptr;
    bool is_input_buffer_valid_ = false;
};


#endif //MEDIACODECDEMO_CMEDIACODEC_H
