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
    AMEDIACODEC__OK                          = 0,
    AMEDIACODEC__INFO_OUTPUT_BUFFERS_CHANGED = -3,
    AMEDIACODEC__INFO_OUTPUT_FORMAT_CHANGED  = -2,
    AMEDIACODEC__INFO_TRY_AGAIN_LATER        = -1,
    AMEDIACODEC__UNKNOWN_ERROR               = -1000,
    AMEDIACODEC__ENV_ERROR                   = -1001,
    AMEDIACODEC__EXCEPTION_ERROR             = -1002,
    AMEDIACODEC__OUTPUT_BUFFER_ERROR         = -1003,
    AMEDIACODEC__BYTE_ARRAY_ERROR            = -1003,
};
class AndroidMediaCodec {
public:
    jobject createByCodecName(JNIEnv *env, const char *codec_name);
    jobject createDecoderByType(JNIEnv *env, const char *codec_type);
    jobject getAndroidCodec();
    jobject getOutputFormat(JNIEnv *env);
    ~AndroidMediaCodec();
    int configureSurface(
            JNIEnv*env,
            jobject media_format,
            jobject android_surface,
            jobject crypto,
            uint32_t flags);
    int start(JNIEnv *env);
    int stop(JNIEnv *env);
    int flush(JNIEnv *env);
    int fillInputBufferByIndex(JNIEnv *env,size_t idx, const uint8_t *data, size_t size);
    int getOutputBufferByIndex(JNIEnv *env,size_t idx, uint8_t *data, size_t size);
    int dequeueInputBufferIndex(JNIEnv *env,int64_t timeoutUs);
    int queueInputBufferByIndex(JNIEnv *env,size_t idx, off_t offset, size_t size, uint64_t time, uint32_t flags);
    int dequeueOutputBufferIndex(JNIEnv *env, AMediaCodecBufferInfo &info, int64_t timeoutUs);
    int releaseOutputBufferByIndex(JNIEnv *env,size_t idx, bool render);
    int release(JNIEnv *env);

private:
    bool checkJNIEnv(JNIEnv *env);
private:
    jobject android_mediacodec_ = nullptr;
    jobject output_buffer_info_ = nullptr;
    bool is_input_buffer_valid_ = false;
};


#endif //MEDIACODECDEMO_CMEDIACODEC_H
