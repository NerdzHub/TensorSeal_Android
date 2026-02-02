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
#ifndef TENSORFLOW_LITE_C_C_API_TYPES_H_
#define TENSORFLOW_LITE_C_C_API_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

// Define TfLiteStatus
typedef enum TfLiteStatus {
    kTfLiteOk = 0,
    kTfLiteError = 1,
    kTfLiteDelegateError = 2,
    kTfLiteApplicationError = 3,
    kTfLiteDelegateDataNotFound = 4,
    kTfLiteDelegateDataWriteError = 5,
    kTfLiteDelegateDataReadError = 6,
    kTfLiteUnresolvedOps = 7,
    kTfLiteCancelled = 8,
} TfLiteStatus;

// Define TfLiteType (Data types for tensors)
typedef enum TfLiteType {
    kTfLiteNoType = 0,
    kTfLiteFloat32 = 1,
    kTfLiteInt32 = 2,
    kTfLiteUInt8 = 3,
    kTfLiteInt64 = 4,
    kTfLiteString = 5,
    kTfLiteBool = 6,
    kTfLiteInt16 = 7,
    kTfLiteComplex64 = 8,
    kTfLiteInt8 = 9,
    kTfLiteFloat16 = 10,
    kTfLiteFloat64 = 11,
    kTfLiteComplex128 = 12,
    kTfLiteUInt64 = 13,
    kTfLiteResource = 14,
    kTfLiteVariant = 15,
    kTfLiteUInt32 = 16,
    kTfLiteUInt16 = 17,
    kTfLiteInt4 = 18,
} TfLiteType;

// Define opaque types (forward declarations)
typedef struct TfLiteTensor TfLiteTensor;
typedef struct TfLiteInterpreter TfLiteInterpreter;
typedef struct TfLiteInterpreterOptions TfLiteInterpreterOptions;
typedef struct TfLiteModel TfLiteModel;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // TENSORFLOW_LITE_C_C_API_TYPES_H_