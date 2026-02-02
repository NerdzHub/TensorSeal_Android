package com.tensorseal

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.TextView // Optional, if you want to show status on screen
import java.io.IOException

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // 1. Read the model file into a ByteArray
        // In the final product, this file will be encrypted (e.g., model.lock)
        // For now, we use a plain .tflite to test the RAM loader.
        val modelBytes = loadFileFromAssets("model.lock")

        if (modelBytes != null) {
            // 2. Send the Bytes to C++ (Simulating decrypted RAM buffer)
            val success = loadModelFromMemory(modelBytes)

            if (success) {
                Log.i("TensorSeal", "Hybrid Loader Verification: PASSED")
            } else {
                Log.e("TensorSeal", "Hybrid Loader Verification: FAILED")
            }
        } else {
            Log.e("TensorSeal", "Asset file 'model.tflite' not found!")
        }
    }

    private fun loadFileFromAssets(fileName: String): ByteArray? {
        return try {
            val inputStream = assets.open(fileName)
            val bytes = inputStream.readBytes()
            inputStream.close()
            bytes
        } catch (e: IOException) {
            Log.e("TensorSeal", "Error reading asset: ${e.message}")
            null
        }
    }

    // Declare the native method
    external fun loadModelFromMemory(data: ByteArray): Boolean

    companion object {
        // Must match the name in CMakeLists.txt
        init {
            System.loadLibrary("tensorseal")
        }
    }
}