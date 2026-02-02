#include <jni.h>
#include <string>
#include <vector>
#include <android/log.h>
#include <cstring>

#include "tensorflow/lite/c/c_api.h"
#include "aes.h"
#include "SecretKey.h"

#define LOG_TAG "TensorSeal"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Global variables to hold the model (Simplified for POC)
TfLiteModel* g_model = nullptr;
TfLiteInterpreter* g_interpreter = nullptr;

extern "C" JNIEXPORT jboolean JNICALL
Java_com_tensorseal_MainActivity_loadModelFromMemory(
        JNIEnv* env,
        jobject /* this */,
        jbyteArray encryptedData) {

    // 1. Check if we received data
    if (encryptedData == nullptr) {
        LOGE("Error: Received null data");
        return false;
    }

    // 2. Get the byte array from Java (This mimics decrypting into RAM)
    jsize len = env->GetArrayLength(encryptedData);
    jbyte* data = env->GetByteArrayElements(encryptedData, 0);

    //LOGI("Step 1: Received %d encrypted bytes.", len);

    // 1. Separate IV and Ciphertext
    // The first 16 bytes are the IV.
    if (len < 16) return false;

    uint8_t iv[16];
    memcpy(iv, data, 16); // Copy IV

    //LOGI("IV received: %02X %02X %02X %02X ...", iv[0], iv[1], iv[2], iv[3]);

    // The rest is the model
    size_t model_len = len - 16;
    std::vector<uint8_t> model_buffer(model_len);
    memcpy(model_buffer.data(), data + 16, model_len);

    // Release Java memory now (security best practice)
    env->ReleaseByteArrayElements(encryptedData, data, 0);

    // 2. Setup the Key (0x00...0x0F) - 16 Bytes for AES-128
    // We allocate 16 bytes for the AES-128 key
    uint8_t key[16];
    get_runtime_key(key);


    // 3. Decrypt in RAM (The Magic Moment) 🔓
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CTR_xcrypt_buffer(&ctx, model_buffer.data(), model_len);

    // --- DIAGNOSTIC CHECK ---
    // A valid TFLite model usually starts with bytes: "TFL3"
    // We print the first 4 bytes to see if decryption worked.
    uint8_t* m = model_buffer.data();
    //LOGI("First 4 bytes after decrypt: %02X %02X %02X %02X", m[0], m[1], m[2], m[3]);
    // ------------------------
    LOGI("Step 2: Decryption complete. Loading into TFLite...");

    // 4. Load via TFLite
    // Note: model_buffer.data() is now PLAINTEXT in RAM.
    g_model = TfLiteModelCreate(model_buffer.data(), model_buffer.size());

    if (g_model == nullptr) {
        LOGE("FAILED: Decryption likely failed (Bad Key?).");
        return false;
    }
    LOGI("Step 2: Model parsed successfully.");

    // 5. Create Interpreter Options
    TfLiteInterpreterOptions* options = TfLiteInterpreterOptionsCreate();
    TfLiteInterpreterOptionsSetNumThreads(options, 2);

    // 6. Create Interpreter
    g_interpreter = TfLiteInterpreterCreate(g_model, options);
    TfLiteInterpreterOptionsDelete(options); // Clean up options

    if (g_interpreter == nullptr) {
        LOGE("FAILED: Could not create Interpreter.");
        return false;
    }

    // 7. Allocate Tensors
    // This is the final proof. It effectively "compiles" the model graph in RAM.
    if (TfLiteInterpreterAllocateTensors(g_interpreter) != kTfLiteOk) {
        LOGE("FAILED: Could not allocate tensors.");
        return false;
    }

    LOGI("SUCCESS! Model loaded entirely from RAM. Logic is secure.");

    // --- PHASE 4: PROOF OF LIFE (DRY RUN) ---
    LOGI("Step 3: Running Dummy Inference to verify graph integrity...");

    // 1. Get Input Tensor details
    TfLiteTensor* input_tensor = TfLiteInterpreterGetInputTensor(g_interpreter, 0);

    // FIX: Use Accessor functions instead of direct member access
    size_t input_size = TfLiteTensorByteSize(input_tensor);
    void* input_data_ptr = TfLiteTensorData(input_tensor);

    // 2. copy dummy data (0s) into the input tensor
    memset(input_data_ptr, 0, input_size);

    // 3. INVOKE (The moment of truth)
    if (TfLiteInterpreterInvoke(g_interpreter) != kTfLiteOk) {
        LOGE("FAILED: Interpreter Invoke failed. Model weights might be corrupted.");
        return false;
    }

    LOGI("Inference Successful! The neural network is alive.");

    // 4. (Optional) Read Output
    const TfLiteTensor* output_tensor = TfLiteInterpreterGetOutputTensor(g_interpreter, 0);

    // FIX: Use Accessor functions
    size_t output_size = TfLiteTensorByteSize(output_tensor);
    void* output_data_ptr = TfLiteTensorData(output_tensor);

    LOGI("Output Tensor Address: %p, Bytes: %zu", output_data_ptr, output_size);

    // ----------------------------------------

    // Cleanup for this test (In real app, keep interpreter alive to run inference)
    TfLiteInterpreterDelete(g_interpreter);
    TfLiteModelDelete(g_model);

    return true;
}