# TensorSeal 🛡️
**Zero-Disk-Footprint Secure Model Loader for Android**

![Android](https://img.shields.io/badge/Platform-Android-green)
![Language](https://img.shields.io/badge/Language-C%2B%2B%20%7C%20Kotlin%20%7C%20Python-blue)
![License](https://img.shields.io/badge/License-MIT-purple)

## 🚨 The Problem
Standard Android AI deployment is insecure. If you place a `.tflite` file in your `assets/` folder, it can be extracted, unzipped, and stolen by any competitor in seconds.

## 💡 The Solution
**TensorSeal** is a secure model loading pipeline that ensures your proprietary model **never touches the disk** in decrypted form.

1.  **Encryption at Rest:** The model is encrypted (AES-128-CTR) at build time.
2.  **In-Memory Decryption:** The model is decrypted directly into RAM buffers using a custom C++ JNI bridge.
3.  **Zero-Disk Footprint:** The decrypted bytes are fed directly to the TensorFlow Lite Interpreter via memory mapping. No temporary files are ever created.
4.  **Key Obfuscation:** The decryption keys are constructed at runtime using "Stack Strings" to prevent static analysis ( `strings` command) attacks.



## 🚀 Quick Start

### 1. Prerequisites
* Android Studio Koala+ (or newer)
* Python 3.10+
* NDK (Side-by-side)

### 2. Installation
```bash
git clone [https://github.com/NerdzHub/TensorSeal_Android.git](https://github.com/NerdzHub/TensorSeal_Android.git)
cd TensorSeal
pip install -r requirements.txt
```

### 3. Encrypt & Build]
    
1. Run the sealer tool. This will:

2. Encrypt `model.tflite` -> `model.lock`

3. Generate `SecretKey.h` with obfuscated C++ logic.

```bash
python3 tools/tensorseal.py \
  --input app/src/main/assets/model.tflite \
  --output app/src/main/assets/model.lock \
  --header app/src/main/cpp/SecretKey.h
```
### 4. Run
   Open the project in Android Studio and hit Run. Check Logcat for: `TensorSeal: Inference Successful! The neural network is alive.`

## 🛠️ Architecture
`tools/tensorseal.py`: The "Packer." Encrypts assets and generates matching C++ headers.

`native-lib.cpp`: The JNI Bridge. Handles memory allocation and TFLite C API calls.

`aes.c`: Lightweight, dependency-free AES-128-CTR implementation.

graph LR
A[model.tflite] -->|Python AES-128| B(model.lock)
B -->|Assets| C[Android App]
C -->|JNI Bridge| D{Native C++}
D -->|Runtime Decrypt| E[RAM Buffer]
E -->|Memory Map| F[TFLite Interpreter]
style E fill:#f9f,stroke:#333,stroke-width:2px
style F fill:#ccf,stroke:#333,stroke-width:2px

## ⚠️ Disclaimer
This tool raises the bar significantly against IP theft, but no client-side protection is 100% unbreakable. 
A determined attacker with root access and advanced reverse-engineering skills (Frida, GDB) can eventually dump memory. 
This tool is designed to stop automated scrapers, casual thieves, and static analysis.

## License
MIT License. Free to use for personal and commercial projects.