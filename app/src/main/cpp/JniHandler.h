#ifndef JNIHANDLER_H
#define JNIHANDLER_H
#include <jni.h>
#include <unistd.h>
#include <pthread.h>
extern "C"{
#include "j4a/j4a_allclasses.include.h"
}

#ifdef ANDROID
#include <android/log.h>
#define XLOGD(...) __android_log_print(ANDROID_LOG_DEBUG,"[player_cpp]",__VA_ARGS__)
#define XLOGI(...) __android_log_print(ANDROID_LOG_INFO,"[player_cpp]",__VA_ARGS__)
#define XLOGW(...) __android_log_print(ANDROID_LOG_WARN,"[player_cpp]",__VA_ARGS__)
#define XLOGE(...) __android_log_print(ANDROID_LOG_ERROR,"[player_cpp]",__VA_ARGS__)
#else
#include <stdio.h>
#define XLOGD(format, ...) printf("[player_cpp][DEBUG][%s][%d]: " format "\n", __FUNCTION__,\
                            __LINE__, ##__VA_ARGS__)
#define XLOGI(format, ...) printf("[player_cpp][INFO][%s][%d]: " format "\n", __FUNCTION__,\
                            __LINE__, ##__VA_ARGS__)
#define XLOGW(format, ...) printf("[player_cpp][WARN][%s][%d]: " format "\n", __FUNCTION__,\
                            __LINE__, ##__VA_ARGS__)
#define XLOGE(format, ...) printf("[player_cpp][ERROR][%s][%d]: " format "\n", __FUNCTION__,\
                            __LINE__, ##__VA_ARGS__)
#endif

static JavaVM *g_jvm;

static pthread_key_t g_thread_key;
static pthread_once_t g_key_once = PTHREAD_ONCE_INIT;

JavaVM *JNI_GetJvm();

void JNI_DeleteGlobalRefP(JNIEnv *env, jobject *obj_ptr);

void JNI_DeleteLocalRefP(JNIEnv *env, jobject *obj_ptr);

static void JNI_ThreadDestroyed(void* value);

static void make_thread_key();

jint JNI_SetupThreadEnv(JNIEnv **p_env);

void JNI_DetachThreadEnv();

int JNI_ThrowException(JNIEnv* env, const char* className, const char* msg);

int JNI_ThrowIllegalStateException(JNIEnv *env, const char* msg);
jobject JNI_NewObjectAsGlobalRef(JNIEnv *env, jclass clazz, jmethodID methodID, ...);

#endif