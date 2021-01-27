#include "JniHandler.h"
JavaVM *JNI_GetJvm()
{
    return g_jvm;
}

void JNI_DeleteGlobalRefP(JNIEnv *env, jobject *obj_ptr)
{
    if (!obj_ptr || !*obj_ptr)
        return;

    env->DeleteGlobalRef(*obj_ptr);
    *obj_ptr = NULL;
}

void JNI_DeleteLocalRefP(JNIEnv *env, jobject *obj_ptr)
{
    if (!obj_ptr || !*obj_ptr)
        return;

    env->DeleteLocalRef(*obj_ptr);
    *obj_ptr = NULL;
}


static void JNI_ThreadDestroyed(void* value)
{
    JNIEnv *env = (JNIEnv*) value;
    if (env != NULL) {
        XLOGE("%s: [%d] didn't call JNI_DetachThreadEnv() explicity\n", __func__, (int)gettid());
        g_jvm->DetachCurrentThread();
        pthread_setspecific(g_thread_key, NULL);
    }
}

static void make_thread_key()
{
    pthread_key_create(&g_thread_key, JNI_ThreadDestroyed);
}

jint JNI_SetupThreadEnv(JNIEnv **p_env)
{
    JavaVM *jvm = g_jvm;
    if (!jvm) {
        XLOGE("JNI_GetJvm: AttachCurrentThread: NULL jvm");
        return -1;
    }

    pthread_once(&g_key_once, make_thread_key);

    JNIEnv *env = (JNIEnv*) pthread_getspecific(g_thread_key);
    if (env) {
        *p_env = env;
        return 0;
    }

    if (jvm->AttachCurrentThread(&env, NULL) == JNI_OK) {
        pthread_setspecific(g_thread_key, env);
        *p_env = env;
        return 0;
    }

    return -1;
}

void JNI_DetachThreadEnv()
{
    JavaVM *jvm = g_jvm;

    XLOGI("%s: [%d]\n", __func__, (int)gettid());

    pthread_once(&g_key_once, make_thread_key);

    JNIEnv *env = (JNIEnv *)pthread_getspecific(g_thread_key);
    if (!env)
        return;
    pthread_setspecific(g_thread_key, NULL);

    if (jvm->DetachCurrentThread() == JNI_OK)
        return;

    return;
}

int JNI_ThrowException(JNIEnv* env, const char* className, const char* msg)
{
    if (env->ExceptionCheck()) {
        jthrowable exception = env->ExceptionOccurred();
        env->ExceptionClear();

        if (exception != NULL) {
            XLOGW("Discarding pending exception (%s) to throw", className);
            env->DeleteLocalRef(exception);
        }
    }

    jclass exceptionClass = env->FindClass(className);
    if (exceptionClass == NULL) {
        XLOGE("Unable to find exception class %s", className);
        /* ClassNotFoundException now pending */
        goto fail;
    }

    if (env->ThrowNew( exceptionClass, msg) != JNI_OK) {
        XLOGE("Failed throwing '%s' '%s'", className, msg);
        /* an exception, most likely OOM, will now be pending */
        goto fail;
    }

    return 0;
    fail:
    if (exceptionClass)
        env->DeleteLocalRef(exceptionClass);
    return -1;
}

int JNI_ThrowIllegalStateException(JNIEnv *env, const char* msg)
{
    return JNI_ThrowException(env, "java/lang/IllegalStateException", msg);
}

jobject JNI_NewObjectAsGlobalRef(JNIEnv *env, jclass clazz, jmethodID methodID, ...)
{
    va_list args;
    va_start(args, methodID);

    jobject global_object = NULL;
    jobject local_object = env->NewObjectV(clazz, methodID, args);
    if (!J4A_ExceptionCheck__throwAny(env) && local_object) {
        global_object = env->NewGlobalRef(local_object);
        JNI_DeleteLocalRefP(env, &local_object);
    }

    va_end(args);
    return global_object;
}