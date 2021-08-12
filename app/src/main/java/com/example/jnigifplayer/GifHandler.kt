package com.example.jnigifplayer

import android.graphics.Bitmap
import java.net.Inet4Address

class GifHandler(path:String) {

    private var gifAddress = loadPathNative(path)

    companion object{
        init {
            System.loadLibrary("jnigifplayer")
        }
    }

    fun getWidth():Int = getWidthNative(gifAddress)
    fun getHeight():Int = getHeightNative(gifAddress)

    fun updateFrame(bitmap: Bitmap):Int = updateFrameNative(gifAddress, bitmap)

    // 初始化
    external fun loadPathNative(path:String):Long

    external fun getWidthNative(ndkGif:Long):Int

    external  fun  getHeightNative(ndkGif:Long):Int

    external fun updateFrameNative(ndkGif:Long, bitmap: Bitmap):Int
}







