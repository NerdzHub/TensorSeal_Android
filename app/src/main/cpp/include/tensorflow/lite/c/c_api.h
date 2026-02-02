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
#ifndef TENSORFLOW_LITE_C_C_API_H_
#define TENSORFLOW_LITE_C_C_API_H_

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#include "tensorflow/lite/c/c_api_types.h"
#include "tensorflow/lite/c/common.h"

// --- ADD THIS BLOCK ---
#ifndef TFL_CAPI_EXPORT
#define TFL_CAPI_EXPORT
#endif
// ----------------------
#ifdef __cplusplus
extern "C" {
#endif

// --------------------------------------------------------------------------
// TfLiteVersion returns a string describing the version number of the
// TensorFlow Lite library. TensorFlow Lite uses semantic versioning.
TFL_CAPI_EXPORT extern const char* TfLiteVersion(void);

// --------------------------------------------------------------------------
// TfLiteModel wraps a loaded TensorFlow Lite model.
typedef struct TfLiteModel TfLiteModel;

// Returns a model from the provided buffer, or null on failure.
TFL_CAPI_EXPORT extern TfLiteModel* TfLiteModelCreate(const void* model_data,
                                                      size_t model_size);

// Returns a model from the provided file, or null on failure.
TFL_CAPI_EXPORT extern TfLiteModel* TfLiteModelCreateFromFile(
        const char* model_path);

// Destroys the model instance.
TFL_CAPI_EXPORT extern void TfLiteModelDelete(TfLiteModel* model);

// --------------------------------------------------------------------------
// TfLiteInterpreterOptions allows customized interpreter configuration.
typedef struct TfLiteInterpreterOptions TfLiteInterpreterOptions;

// Returns a new interpreter options instances.
TFL_CAPI_EXPORT extern TfLiteInterpreterOptions*
TfLiteInterpreterOptionsCreate();

// Destroys the interpreter options instance.
TFL_CAPI_EXPORT extern void TfLiteInterpreterOptionsDelete(
        TfLiteInterpreterOptions* options);

// Sets the number of CPU threads to use for the interpreter.
TFL_CAPI_EXPORT extern void TfLiteInterpreterOptionsSetNumThreads(
        TfLiteInterpreterOptions* options, int32_t num_threads);

// --------------------------------------------------------------------------
// TfLiteInterpreter provides inference from a provided model.
typedef struct TfLiteInterpreter TfLiteInterpreter;

// Returns a new interpreter using the provided model and options, or null on
// failure.
TFL_CAPI_EXPORT extern TfLiteInterpreter* TfLiteInterpreterCreate(
        const TfLiteModel* model, const TfLiteInterpreterOptions* optional_options);

// Destroys the interpreter.
TFL_CAPI_EXPORT extern void TfLiteInterpreterDelete(
        TfLiteInterpreter* interpreter);

// Returns the number of input tensors associated with the model.
TFL_CAPI_EXPORT extern int32_t TfLiteInterpreterGetInputTensorCount(
        const TfLiteInterpreter* interpreter);

// Returns the tensor associated with the input index.
TFL_CAPI_EXPORT extern TfLiteTensor* TfLiteInterpreterGetInputTensor(
        const TfLiteInterpreter* interpreter, int32_t input_index);

// Resizes the specified input tensor.
TFL_CAPI_EXPORT extern TfLiteStatus TfLiteInterpreterResizeInputTensor(
        TfLiteInterpreter* interpreter, int32_t input_index, const int* input_dims,
        int32_t input_dims_size);

// Updates allocations for all tensors.
TFL_CAPI_EXPORT extern TfLiteStatus TfLiteInterpreterAllocateTensors(
        TfLiteInterpreter* interpreter);

// Runs inference for the loaded graph.
TFL_CAPI_EXPORT extern TfLiteStatus TfLiteInterpreterInvoke(
        TfLiteInterpreter* interpreter);

// Returns the number of output tensors associated with the model.
TFL_CAPI_EXPORT extern int32_t TfLiteInterpreterGetOutputTensorCount(
        const TfLiteInterpreter* interpreter);

// Returns the tensor associated with the output index.
TFL_CAPI_EXPORT extern const TfLiteTensor* TfLiteInterpreterGetOutputTensor(
        const TfLiteInterpreter* interpreter, int32_t output_index);

// --------------------------------------------------------------------------
// TfLiteTensor Wrappers (The functions you were missing!)
// --------------------------------------------------------------------------

// Returns the type of a tensor.
TFL_CAPI_EXPORT extern TfLiteType TfLiteTensorType(const TfLiteTensor* tensor);

// Returns the number of dimensions that the tensor has.
TFL_CAPI_EXPORT extern int32_t TfLiteTensorNumDims(const TfLiteTensor* tensor);

// Returns the length of the tensor in the "dim_index" dimension.
TFL_CAPI_EXPORT extern int32_t TfLiteTensorDim(const TfLiteTensor* tensor,
                                               int32_t dim_index);

// Returns the size of the underlying data in bytes.
TFL_CAPI_EXPORT extern size_t TfLiteTensorByteSize(const TfLiteTensor* tensor);

// Returns a pointer to the underlying data buffer.
TFL_CAPI_EXPORT extern void* TfLiteTensorData(const TfLiteTensor* tensor);

// Returns the (null-terminated) name of the tensor.
TFL_CAPI_EXPORT extern const char* TfLiteTensorName(const TfLiteTensor* tensor);

// Copies from the provided input buffer into the tensor's buffer.
TFL_CAPI_EXPORT extern TfLiteStatus TfLiteTensorCopyFromBuffer(
        TfLiteTensor* tensor, const void* input_data, size_t input_data_size);

// Copies to the provided output buffer from the tensor's buffer.
TFL_CAPI_EXPORT extern TfLiteStatus TfLiteTensorCopyToBuffer(
        const TfLiteTensor* tensor, void* output_data, size_t output_data_size);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // TENSORFLOW_LITE_C_C_API_H_