/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef TENSORFLOW_LITE_C_COMMON_H_
#define TENSORFLOW_LITE_C_COMMON_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tensorflow/lite/c/c_api_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Fixed size array for dimensions.
typedef struct TfLiteIntArray {
    int size;
    int data[];
} TfLiteIntArray;

// Fixed size array for float params.
typedef struct TfLiteFloatArray {
    int size;
    float data[];
} TfLiteFloatArray;

// Opaque context type
typedef struct TfLiteContext TfLiteContext;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // TENSORFLOW_LITE_C_COMMON_H_