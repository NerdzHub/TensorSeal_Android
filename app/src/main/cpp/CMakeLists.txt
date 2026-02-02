# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("tensorseal")

# FORCE AES-256 mode
add_definitions(-DAES128=1)

# 1. Define the include path
include_directories(${CMAKE_SOURCE_DIR}/include)

# 2. Add your library
add_library(tensorseal SHARED
        native-lib.cpp
        aes.c)

# 3. Explicitly import the TFLite library we manually placed
add_library(tflite-lib SHARED IMPORTED)
set_target_properties(tflite-lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libtensorflowlite_jni.so)

# 4. Link them
target_link_libraries(tensorseal
        android
        log
        tflite-lib)