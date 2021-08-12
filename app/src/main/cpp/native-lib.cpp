#include <jni.h>
#include <string>
#include <malloc.h>
#include <cstring>
#include <android/log.h>
#include <android/bitmap.h>
#include "gif_lib.h"

#define LOG_TAG "GIF_PLAY"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  argb(a, r, g, b) ( ((a) & 0xff) << 24 ) | ( ((b) & 0xff) << 16 ) | ( ((g) & 0xff) << 8 ) | ((r) & 0xff)

typedef struct GifBean {
    int current_frame; // 当前帧，第几帧
    int total_frame;   // 总帧数
    int *delays;      // 延迟时间数组
} GifPan;

void drawFrame(GifFileType *type, GifPan *gifBean, AndroidBitmapInfo info, void *pixels) {
    // 当前帧
    SavedImage savedImage = type->SavedImages[gifBean->current_frame];
    // 该帧图片首地址
    int *px = (int *) pixels;
    int pointPixel;
    GifImageDesc frameInfo = savedImage.ImageDesc;

    // 压缩数据
    GifByteType gifByteType;
    ColorMapObject *colorMapObject = frameInfo.ColorMap;
    // Bitmap向下偏移
    px = (int *) ((char *) px + info.stride * frameInfo.Top);
    // 每一行首地址
    int *line;
    for (int y = frameInfo.Top; y < frameInfo.Top + frameInfo.Height; ++y) {
        line = px;
        for (int x = frameInfo.Left; x < frameInfo.Left + frameInfo.Width; ++x) {
            // 拿到每一个点的坐标位置
            pointPixel = (y - frameInfo.Top) * frameInfo.Width + (x - frameInfo.Left);
            gifByteType = savedImage.RasterBits[pointPixel];
            GifColorType gifColorType = colorMapObject->Colors[gifByteType];
            line[x] = argb(255, gifColorType.Red, gifColorType.Green, gifColorType.Blue);
        }
        px = (int *) ((char *) px + info.stride);
    }
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jnigifplayer_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_jnigifplayer_GifHandler_loadPathNative(JNIEnv *env, jobject thiz, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);
    int err;
    GifFileType *gifFileType = DGifOpenFileName(path, &err);
    DGifSlurp(gifFileType);

    GifPan *gifPan = (GifPan *) malloc(sizeof(GifPan));
    // 清空内存地址
    memset(gifPan, 0, sizeof(GifPan));
    gifFileType->UserData = gifPan;
    // 初始化数组
    gifPan->delays = (int *) malloc(sizeof(int) * gifFileType->ImageCount);
    memset(gifPan->delays, 0, sizeof(int) * gifFileType->ImageCount);
    //    获取时间
    gifFileType->UserData = gifPan;
    gifPan->current_frame = 0;
    gifPan->total_frame = gifFileType->ImageCount;
    ExtensionBlock *ext;
    for (int i = 0; i < gifFileType->ImageCount; ++i) {
        SavedImage frame = gifFileType->SavedImages[i];
        for (int j = 0; j < frame.ExtensionBlockCount; ++j) {
            if (frame.ExtensionBlocks[j].Function == GRAPHICS_EXT_FUNC_CODE) {
                ext = &frame.ExtensionBlocks[j];
                break;
            }
        }
        if (ext) {
        //	Delay Time - 单位1/100秒   1s/100
            int frame_delay = 10 * (ext->Bytes[1] | (ext->Bytes[2] << 8));//ms
            LOGE("时间 : %d", frame_delay);
            gifPan->delays[i] = frame_delay;
        }
    }
    LOGE("gif长度大小: %d  ", gifFileType->ImageCount);

    env->ReleaseStringUTFChars(path_, path);
    return (jlong) gifFileType;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_jnigifplayer_GifHandler_getWidthNative(JNIEnv *env, jobject thiz, jlong ndk_gif) {
    GifFileType  *gifFileType = (GifFileType *)ndk_gif;
    return gifFileType->SWidth;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_jnigifplayer_GifHandler_getHeightNative(JNIEnv *env, jobject thiz, jlong ndk_gif) {
    GifFileType  *gifFileType = (GifFileType *)ndk_gif;
    return gifFileType->SHeight;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_jnigifplayer_GifHandler_updateFrameNative(JNIEnv *env, jobject thiz, jlong ndk_gif,
                                                           jobject bitmap) {
    GifFileType *gifFileType = (GifFileType *) ndk_gif;
    GifBean * gifBean= (GifBean *)gifFileType->UserData;
    AndroidBitmapInfo info;
    // 像素数组
    AndroidBitmap_getInfo(env, bitmap, &info);

    void *pixels;
    AndroidBitmap_lockPixels(env, bitmap, &pixels);
    drawFrame(gifFileType, gifBean, info, pixels);
    gifBean->current_frame += 1;
    if (gifBean->current_frame >= gifBean->total_frame - 1) {
        gifBean->current_frame = 0;
        LOGE("reverse, current: %d  ",gifBean->current_frame);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
    return gifBean->delays[gifBean->current_frame];
}