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
    jobject getAndroidCodec();
    jobject getOutputFormat();
    ~CMediaCodec();
    int configureSurface(
            JNIEnv*env,
            jobject android_surface,
            jobject crypto,
            uint32_t flags);
    int SDL_AMediaCodecJava_start();
    int SDL_AMediaCodecJava_stop();
    int SDL_AMediaCodecJava_flush();
    int SDL_AMediaCodecJava_writeInputData(size_t idx, const uint8_t *data, size_t size);
    int SDL_AMediaCodecJava_dequeueInputBuffer(int64_t timeoutUs);
    int SDL_AMediaCodecJava_queueInputBuffer(size_t idx, off_t offset, size_t size, uint64_t time, uint32_t flags);
    int SDL_AMediaCodecJava_dequeueOutputBuffer(int64_t timeoutUs);
    int SDL_AMediaCodecJava_releaseOutputBuffer(size_t idx, bool render);
    int release();
private:
    jobject android_mediacodec = nullptr;
    jobject output_buffer_info = nullptr;
    AMediaCodecBufferInfo info;
    jobject android_input_mediaformat = nullptr;
    bool is_input_buffer_valid = false;
};


#endif //MEDIACODECDEMO_CMEDIACODEC_H
