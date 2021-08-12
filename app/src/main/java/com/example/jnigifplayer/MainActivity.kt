package com.example.jnigifplayer

import android.graphics.Bitmap
import android.os.*
import androidx.appcompat.app.AppCompatActivity
import android.view.View
import android.widget.TextView
import com.example.jnigifplayer.databinding.ActivityMainBinding
import java.io.File

// gif文件格式介绍：https://blog.csdn.net/wzy198852/article/details/17266507
class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private lateinit var bitmap:Bitmap
    private lateinit var gifHandler:GifHandler

    var handler: Handler? = object : Handler(Looper.getMainLooper()) {
        override fun handleMessage(msg: Message) {
            val mNextFrame: Int = gifHandler.updateFrame(bitmap)
            this.sendEmptyMessageDelayed(1, mNextFrame.toLong())
            binding.image.setImageBitmap(bitmap)
        }
    }


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

    }

    fun ndkLoadGif(view: View?) {
        val file = File(Environment.getExternalStorageDirectory(), "demo.gif")
        gifHandler = GifHandler(file.absolutePath)

        // 得到gif的宽高生成Bitmap
        val width = gifHandler.getWidth()
        val height = gifHandler.getHeight()
        bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
        // 下一帧的刷新事件
        val nextFrame = gifHandler.updateFrame(bitmap)
        handler!!.sendEmptyMessageDelayed(1, nextFrame.toLong())
    }

    /**
     * A native method that is implemented by the 'jnigifplayer' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
}