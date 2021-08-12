#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jnigifplayer_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_jnigifplayer_GifHandler_loadPathNative(JNIEnv *env, jobject thiz, jstring path) {
    // TODO: implement loadPathNative()
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_jnigifplayer_GifHandler_getWidthNative(JNIEnv *env, jobject thiz, jlong ndk_gif) {
    // TODO: implement getWidthNative()
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_jnigifplayer_GifHandler_getHeightNative(JNIEnv *env, jobject thiz, jlong ndk_gif) {
    // TODO: implement getHeightNative()
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_jnigifplayer_GifHandler_updateFrameNative(JNIEnv *env, jobject thiz, jlong ndk_gif,
                                                           jobject bitmap) {
    // TODO: implement updateFrameNative()
}