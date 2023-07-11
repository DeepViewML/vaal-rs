/**
 * Copyright 2021 by Au-Zone Technologies.  All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential.
 *
 * Authorization to use this header is provided a part of DeepView VisionPack.
 *
 * @file vaal.h DeepView VisionPack AI Acceleration Library
 */

#ifndef DEEPVIEW_VAAL_H
#define DEEPVIEW_VAAL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Header API Macros from Hedly - https://nemequ.github.io/hedley
 * HEDLY namespace has been updated to VAAL for DeepView VAAL Namespace and
 * functionality stripped down to reduce size of headers.
 */
#define VAAL_VERSION_ENCODE(major, minor, revision) \
    (((major) *1000000) + ((minor) *1000) + (revision))
#define VAAL_VERSION_DECODE_MAJOR(version) ((version) / 1000000)
#define VAAL_VERSION_DECODE_MINOR(version) (((version) % 1000000) / 1000)
#define VAAL_VERSION_DECODE_REVISION(version) ((version) % 1000)

#if defined(__GNUC__) && defined(__GNUC_PATCHLEVEL__)
#define VAAL_GNUC_VERSION \
    VAAL_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#elif defined(__GNUC__)
#define VAAL_GNUC_VERSION VAAL_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, 0)
#endif

#if defined(VAAL_GNUC_VERSION)
#define VAAL_GNUC_VERSION_CHECK(major, minor, patch) \
    (VAAL_GNUC_VERSION >= VAAL_VERSION_ENCODE(major, minor, patch))
#else
#define VAAL_GNUC_VERSION_CHECK(major, minor, patch) (0)
#endif

#if defined(__CC_ARM) && defined(__ARMCOMPILER_VERSION)
#define VAAL_ARM_VERSION                                           \
    VAAL_VERSION_ENCODE(__ARMCOMPILER_VERSION / 1000000,           \
                        (__ARMCOMPILER_VERSION % 1000000) / 10000, \
                        (__ARMCOMPILER_VERSION % 10000) / 100)
#elif defined(__CC_ARM) && defined(__ARMCC_VERSION)
#define VAAL_ARM_VERSION                                     \
    VAAL_VERSION_ENCODE(__ARMCC_VERSION / 1000000,           \
                        (__ARMCC_VERSION % 1000000) / 10000, \
                        (__ARMCC_VERSION % 10000) / 100)
#endif

#if defined(VAAL_ARM_VERSION)
#define VAAL_ARM_VERSION_CHECK(major, minor, patch) \
    (VAAL_ARM_VERSION >= VAAL_VERSION_ENCODE(major, minor, patch))
#else
#define VAAL_ARM_VERSION_CHECK(major, minor, patch) (0)
#endif

#if defined(__IAR_SYSTEMS_ICC__)
#if __VER__ > 1000
#define VAAL_IAR_VERSION                           \
    VAAL_VERSION_ENCODE((__VER__ / 1000000),       \
                        ((__VER__ / 1000) % 1000), \
                        (__VER__ % 1000))
#else
#define VAAL_IAR_VERSION VAAL_VERSION_ENCODE(VER / 100, __VER__ % 100, 0)
#endif
#endif

#if defined(VAAL_IAR_VERSION)
#define VAAL_IAR_VERSION_CHECK(major, minor, patch) \
    (VAAL_IAR_VERSION >= VAAL_VERSION_ENCODE(major, minor, patch))
#else
#define VAAL_IAR_VERSION_CHECK(major, minor, patch) (0)
#endif

#if defined(VAAL_GNUC_VERSION) && !defined(__clang) && \
    !defined(VAAL_ARM_VERSION)
#define VAAL_GCC_VERSION VAAL_GNUC_VERSION
#endif

#if defined(VAAL_GCC_VERSION)
#define VAAL_GCC_VERSION_CHECK(major, minor, patch) \
    (VAAL_GCC_VERSION >= VAAL_VERSION_ENCODE(major, minor, patch))
#else
#define VAAL_GCC_VERSION_CHECK(major, minor, patch) (0)
#endif

#if defined(__cplusplus) && (__cplusplus >= 201402L)
#define VAAL_DEPRECATED(since) [[deprecated("Since " #since)]]
#elif _MSC_VER >= 1400
#define VAAL_DEPRECATED(since) __declspec(deprecated("Since " #since))
#elif _MSC_VER >= 1310
#define VAAL_DEPRECATED(since) _declspec(deprecated)
#elif VAAL_IAR_VERSION_CHECK(8, 0, 0)
#define VAAL_DEPRECATED(since) _Pragma("deprecated")
#elif defined(_GHS_MULTI)
#define VAAL_DEPRECATED(since)
#else
#define VAAL_DEPRECATED(since) __attribute__((__deprecated__("Since " #since)))
#endif

#if VAAL_GCC_VERSION_CHECK(4, 3, 0)
#define VAAL_UNAVAILABLE(available_since) \
    __attribute__((__warning__("Not available until " #available_since)))
#else
#define VAAL_UNAVAILABLE(available_since)
#endif

#if defined(__cplusplus) && (__cplusplus >= 201703L)
#define VAAL_WARN_UNUSED_RESULT [[nodiscard]]
#elif defined(_Check_return_) /* SAL */
#define VAAL_WARN_UNUSED_RESULT _Check_return_
#elif !defined(__IAR_SYSTEMS_ICC__)
#define VAAL_WARN_UNUSED_RESULT __attribute__((__warn_unused_result__))
#else
#define VAAL_WARN_UNUSED_RESULT
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
#define VAAL_PRIVATE
#define VAAL_PUBLIC __declspec(dllexport)
#define VAAL_IMPORT __declspec(dllimport)
#else
#define VAAL_PRIVATE __attribute__((__visibility__("hidden")))
#define VAAL_PUBLIC __attribute__((__visibility__("default")))
#define VAAL_IMPORT extern
#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) && \
    !defined(__STDC_NO_VLA__) && !defined(__cplusplus) &&         \
    !defined(VAAL_PGI_VERSION) && !defined(VAAL_TINYC_VERSION)
#define VAAL_ARRAY_PARAM(name) static(name)
#else
#define VAAL_ARRAY_PARAM(name)
#endif

#if !defined(__cplusplus) &&                                         \
    ((defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)) || \
     defined(_Static_assert))
#define VAAL_STATIC_ASSERT(expr, message) _Static_assert(expr, message)
#elif (defined(__cplusplus) && (__cplusplus >= 201703L)) || (_MSC_VER >= 1600)
#define VAAL_STATIC_ASSERT(expr, message) static_assert(expr, message)
#elif defined(__cplusplus) && (__cplusplus >= 201103L)
#define VAAL_STATIC_ASSERT(expr, message) static_assert(expr)
#else
#define VAAL_STATIC_ASSERT(expr, message)
#endif

#ifdef VAAL_API_STATIC
#define VAAL_API
#else
#ifdef VAAL_API_EXPORT
#define VAAL_API VAAL_PUBLIC
#else
#define VAAL_API VAAL_IMPORT
#endif
#endif

/**
 * DeepView VAAL Version 1.0
 */
#define VAAL_VERSION_1_0 VAAL_VERSION_ENCODE(1, 0, 0)

/**
 * DeepView VAAL Version 1.1
 */
#define VAAL_VERSION_1_1 VAAL_VERSION_ENCODE(1, 1, 0)

/**
 * DeepView VAAL Version 1.4
 */
#define VAAL_VERSION_1_4 VAAL_VERSION_ENCODE(1, 4, 0)

/**
 * This macro defines the target version when compiling against vaal.h and will
 * cause warnings to be generated when the target version does not include a
 * symbol or if a symbol is marked as depracated.
 *
 * By default the target version is the latest version of VAAL for the provided
 * vaal.h file.
 */
#ifndef VAAL_TARGET_VERSION
#define VAAL_TARGET_VERSION VAAL_VERSION_1_4
#endif

#if VAAL_TARGET_VERSION < VAAL_VERSION_ENCODE(1, 0, 0)
#define VAAL_AVAILABLE_SINCE_1_0 VAAL_UNAVAILABLE(1.0)
#define VAAL_DEPRECATED_SINCE_1_0
#else
#define VAAL_AVAILABLE_SINCE_1_0
#define VAAL_DEPRECATED_SINCE_1_0 VAAL_DEPRECATED(1.0)
#endif

#if VAAL_TARGET_VERSION < VAAL_VERSION_ENCODE(1, 1, 0)
#define VAAL_AVAILABLE_SINCE_1_1 VAAL_UNAVAILABLE(1.1)
#define VAAL_DEPRECATED_SINCE_1_1
#else
#define VAAL_AVAILABLE_SINCE_1_1
#define VAAL_DEPRECATED_SINCE_1_1 VAAL_DEPRECATED(1.1)
#endif

#if VAAL_TARGET_VERSION < VAAL_VERSION_ENCODE(1, 4, 0)
#define VAAL_AVAILABLE_SINCE_1_4 VAAL_UNAVAILABLE(1.4)
#define VAAL_DEPRECATED_SINCE_1_4
#else
#define VAAL_AVAILABLE_SINCE_1_4
#define VAAL_DEPRECATED_SINCE_1_4 VAAL_DEPRECATED(1.4)
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef CLAMP
#define CLAMP(x, min, max) MAX(min, MIN(x, max))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct NNContext
 *
 * DeepViewRT Context.  Refer to the DeepviewRT User Manual for details.
 */
typedef struct nn_context NNContext;

/**
 * @struct NNTensor
 *
 * DeepViewRT Tensor objects.  Refer to the DeepViewRT User Manual for details.
 */
typedef struct nn_tensor NNTensor;

/**
 * @struct NNModel
 *
 * DeepviewRT Model (RTM).  Refer to the DeepViewRT User Manual for details.
 */
typedef void NNModel;

/**
 * VAALContext will run on the local CPU.
 */
#define VAAL_DEVICE_CPU "cpu"

/**
 * VAALContext will off-load processing to the GPU accelerator.
 */
#define VAAL_DEVICE_GPU "gpu"

/**
 * VAALContext will off-load processing to the NPU accelerator.
 */
#define VAAL_DEVICE_NPU "npu"

/**
 * VAALContext will off-load processing to the DeepViewRT OpenCL plugin which
 * can run computations on any OpenCL 1.1 EP compatible device.  Refer to the
 * DeepViewRT OpenCL plugin documentation for details and limitations.
 */
#define VAAL_DEVICE_OPENCL "opencl"

/**
 * Unsigned normalization (0..1).
 */
#define VAAL_IMAGE_PROC_UNSIGNED_NORM 0x0001

/**
 * Image whitening (standardization) normalizes the image using the mean of
 * the image as the 0 point.
 */
#define VAAL_IMAGE_PROC_WHITENING 0x0002

/**
 * Signed normalization (-1..1)
 */
#define VAAL_IMAGE_PROC_SIGNED_NORM 0x0004

/**
 * Applies image whitening/standardization but with pre-calculated values which
 * have been captured from the ImageNet dataset.  This is common with PyTorch
 * and ONNX models.
 */
#define VAAL_IMAGE_PROC_IMAGENET 0x0008

/**
 * Mirror the image on load, a horizontal flip.
 */
#define VAAL_IMAGE_PROC_MIRROR 0x1000

/**
 * Flip the image vertically on load.
 */
#define VAAL_IMAGE_PROC_FLIP 0x2000

/**
 * Enumeration of all errors provided by VAAL.  Most functions will return a
 * VAALError with VAAL_SUCCESS being zero. A common usage pattern for client
 * code is to check for err using `if (err) ...` as any error condition will
 * return non-zero.
 */
typedef enum {
    /**
     * Successfull operation, no error.
     */
    VAAL_SUCCESS = 0,
    /**
     * Internal error without a specific error code, catch-all error.
     */
    VAAL_ERROR_INTERNAL,
    /**
     * The provided handle is invalid.  This error is typically used by NNEngine
     * when interfacing with another API
     */
    VAAL_ERROR_INVALID_HANDLE,
    /**
     * Out of memory error, returned if a call to malloc returns NULL or similar
     * error.
     */
    VAAL_ERROR_OUT_OF_MEMORY,
    /**
     * Out of resources errors are similar to out of memory though sometimes
     * treated separately by underlying plugins.
     */
    VAAL_ERROR_OUT_OF_RESOURCES,
    /**
     * Signals an API has not been implemented..
     */
    VAAL_ERROR_NOT_IMPLEMENTED,
    /**
     * A required parameter was missing or NULL or simply invalid.
     */
    VAAL_ERROR_INVALID_PARAMETER,
    /**
     * When attempting to run an operation where the input/output tensors are
     * of different types and the operation does not support automatic type
     * conversions.
     */
    VAAL_ERROR_TYPE_MISMATCH,
    /**
     * When attempting to run an operation and the input/output tensors have
     * invalid or unsupported shape combinations.  Some operations require the
     * shapes to be the same while others, such as arithmetic broadcasting
     * operations, will support various shape combinations but if the provided
     * pairs are invalid then the shape mismatch is returned.
     */
    VAAL_ERROR_SHAPE_MISMATCH,
    /**
     * The tensor's shape is invalid for the given operation.  It differs from
     * the shape mismatch in that the shape is invalid on its own and not
     * relative to another related tensor.  An example would be a shape with
     * more than one -1 dimension.
     */
    VAAL_ERROR_INVALID_SHAPE,
    /**
     * The requested ordering was invalid.
     */
    VAAL_ERROR_INVALID_ORDER,
    /**
     * The requested axis for an operation was invalid or unsupported.
     */
    VAAL_ERROR_INVALID_AXIS,
    /**
     * A required resource was missing or the reference invalid.
     */
    VAAL_ERROR_MISSING_RESOURCE,
    /**
     * The requested engine is invalid.
     */
    VAAL_ERROR_INVALID_ENGINE,
    /**
     * The tensor has no data or the data is not currently accessible.
     */
    VAAL_ERROR_TENSOR_NO_DATA,
    /**
     * The internal kernel or subroutine required to complete an operation using
     * the engine plugin was missing.  An example would be OpenCL or OpenVX
     * operation where the kernel implementation cannot be located.
     */
    VAAL_ERROR_KERNEL_MISSING,
    /**
     * The operation does not support the tensor's type.
     */
    VAAL_ERROR_TENSOR_TYPE_UNSUPPORTED,
    /**
     * For operations which can operate on an array of inputs, the provided list
     * of inputs was too large.
     */
    VAAL_ERROR_TOO_MANY_INPUTS,
    /**
     * A system error occured when interfacing with an operating system
     * function.  On some systems errno might be updated with the underlying
     * error code.
     */
    VAAL_ERROR_SYSTEM_ERROR,
    /**
     * When working with a model a reference was made to a layer which did not
     * exist.
     */
    VAAL_ERROR_INVALID_LAYER,
    /**
     * The model is invalid or corrupted.
     */
    VAAL_ERROR_MODEL_INVALID,
    /**
     * An operation referenced a model but the model was not provided.
     */
    VAAL_ERROR_MODEL_MISSING,
    /**
     * The string was too large.
     */
    VAAL_ERROR_STRING_TOO_LARGE,
    /**
     * The quantization parameters are invalid.
     */
    VAAL_ERROR_INVALID_QUANT,
    /**
     * Failed to generate graph representation of model.
     */
    VAAL_ERROR_MODEL_GRAPH_FAILED,
    /**
     * Failed to verify graph generateed from model.
     */
    VAAL_ERROR_GRAPH_VERIFY_FAILED,
    /**
     * Unknown decoder was provided or failed to detect decoder.
     */
    VAAL_ERROR_UNKNOWN_DECODER = 0x10000,
    /**
     * Attempted to write to read-only parameter.
     */
    VAAL_ERROR_PARAMETER_READ_ONLY,
    /**
     * Lookup of named parameter was not found.
     */
    VAAL_ERROR_PARAMETER_NOT_FOUND,
    /**
     * Attempted an operation which is unsupported on the current platform.
     */
    VAAL_ERROR_PLATFORM_UNSUPPORTED,
    /**
     * Library has not been initialized for use, this should happen
     * automatically but can be done manually using vaal_init().
     */
    VAAL_ERROR_LIBRARY_UNINITIALIZED,
} VAALError;

/**
 * VAAL datatype definitions are used for identifying type information of the
 * context parameters.
 */
typedef enum {
    /**
     * The VAAL_RAW type is untyped and left to the caller to handle.
     */
    VAAL_RAW = 0,
    /**
     * The VAAL_PTR is a standard void* pointer.
     */
    VAAL_PTR,
    /**
     * The VAAL_FUNC is a function pointer.
     */
    VAAL_FUNC,
    /**
     * The VAAL_STR type is a standard NULL-terminated C-string.
     */
    VAAL_STR,
    /**
     * The VAAL_I8 type maps to a C int8_t
     */
    VAAL_I8,
    /**
     * The VAAL_U8 type maps to a C uint8_t
     */
    VAAL_U8,
    /**
     * The VAAL_I16 type maps to a C int16_t
     */
    VAAL_I16,
    /**
     * The VAAL_U16 type maps to a C uint16_t
     */
    VAAL_U16,
    /**
     * The VAAL_I32 type maps to a C int32_t
     */
    VAAL_I32,
    /**
     * The VAAL_U32 type maps to a C uint32_t
     */
    VAAL_U32,
    /**
     * The VAAL_I64 type maps to a C int64_t
     */
    VAAL_I64,
    /**
     * The VAAL_U64 type maps to a C uint64_t
     */
    VAAL_U64,
    /**
     * The VAAL_F16 type is currently unsupported but would map to either an ARM
     * __fp16 or C11 (GCC extension) _Float16 type.
     */
    VAAL_F16,
    /**
     * The VAAL_F32 type maps to a C float
     */
    VAAL_F32,
    /**
     * The VAAL_F64 type maps to a C double
     */
    VAAL_F64,
} VAALType;

/**
 * Common bounding box structure used by all decoders.  The box is defined by
 * the minimum and maximum coordinates of the x,y points as normalized values.
 * Normalized values are in the range 0..1 and can be multiplied by the size of
 * an image to de-normalize back to actual pixel locations.
 *
 * The label is an index which can be used to access the textual label from the
 * model or a labels file.
 *
 * The score's definition is model-specific but generally it is a sigmoid
 * processed value.
 */
typedef struct {
    /**
     * left-most normalized coordinate of the bounding box.
     */
    float xmin;
    /**
     * top-most normalized coordinate of the bounding box.
     */
    float ymin;
    /**
     * right-most normalized coordinate of the bounding box.
     */
    float xmax;
    /**
     * bottom-most normalized coordinate of the bounding box.
     */
    float ymax;
    /**
     * model-specific score for this detection, higher implies more confidence.
     */
    float score;
    /**
     * label index for this detection, text representation can be retrived using
     * @ref VAALContext::vaal_label()
     */
    int label;
} VAALBox;

/**
 * @struct VAALEuler
 *
 */
typedef struct {
    float yaw;
    float pitch;
    float roll;
} VAALEuler;

/**
 * @struct VAALKeypoint
 *
 */
typedef struct {
    float x;
    float y;
    float score;
} VAALKeypoint;

/**
 * @struct VAALContext
 *
 * Context object for VAAL to manage internal model data structures.  The
 * VAALContext manages the DeepViewRT NNContext as well as parameters related
 * to model post-processing.
 *
 * Models typically require further post-processing before the outputs are
 * ready to be interpreted.  What post-processing is required varies greatly
 * between various model architectures. A common post-processing step is the
 * decoder which is responsible for translating an internal model representation
 * into one which represents the problem space.
 *
 * For detection models the post-processing includes the box decoding step which
 * can either be integrated into the model or run as a separate step using the
 * model's outputs followed by sorting and non-maximum supression "NMS" which is
 * tasked with cleaning the bounding box output.
 *
 * For classification models the post-processing is typically implemented by
 * finding within an output vector the index of the maximum value "argmax" where
 * the index has meaning. For example, the index into the labels with which the
 * classifier was trained. The value itself can also be interpreted as a weak
 * confidence metric in cases where a softmax or sigmoid was applied to the
 * vector.
 */
typedef struct vaal_context VAALContext;

/**
 * DeepView VAAL library version as MAJOR.MINOR.PATCH.  The library returns
 * a string encoding the version and also optionally sets the provided
 * arguments.
 *
 * @return library version string
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
const char*
vaal_version(int* major, int* minor, int* patch, const char** extra);

/**
 * DeepView VAAL string encoding of error enum
 *
 * @return error string
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
const char*
vaal_strerror(VAALError error);

/**
 * Returns the size of the @ref VAALType in bytes.  Note that VAAL parameters
 * can be arrays of types, so a full buffer would be length * vaal_type_sizeof.
 *
 * @returns element size of the @ref VAALType.
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
size_t
vaal_type_sizeof(VAALType type);

/**
 * Returns the string name for the @ref VAALType.
 *
 * @param type to be returned as a string name.
 *
 * @returns string representation of the type.
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
const char*
vaal_type_name(VAALType type);

/**
 * Returns the current monotonic time in nanoseconds.
 *
 * @return monotonic time as nanoseconds
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
int64_t
vaal_clock_now();

/**
 * Creates a VAALContext for the provided device.  A context can manage a single
 * model but many contexts can be created within an application.  If device is
 * NULL then the default DeepViewRT engine will be used which evaluates models
 * on the CPU.
 *
 * The device string can be further extended to specify device details such as
 * using "gpu:1" to use the second GPU instead of the default one.  Refer to the
 * VAAL_DEVICE documentation for details on the extended parameters.
 *
 * Supported devices:
 *  - @ref VAAL_DEVICE_CPU
 *  - @ref VAAL_DEVICE_GPU
 *  - @ref VAAL_DEVICE_NPU
 *
 * @param device
 *
 * @return VAALContext
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALContext*
vaal_context_create(const char* device);

/**
 * Releases the VAALContext unloading models and releasing any data structures
 * under the control of the context.
 *
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
void
vaal_context_release(VAALContext* context);

/**
 * Retrieves the DeepViewRT NNContext associated with the VAALContext.  This can
 * be used to further query the underlying NNEngine, NNModel, and other
 * DeepViewRT API.
 *
 * @param context
 *
 * @return NNContext
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
NNContext*
vaal_context_deepviewrt(VAALContext* context);

/**
 * Retrieves the cache tensor associated with VAAL context.
 * Cache is used for post-processing and CPU inference
 *
 * @param context
 *
 * @return NNTensor
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
NNTensor*
vaal_context_cache(VAALContext* context);

/**
 * Retrieves the dictionary data struct associated with the VAALContext.
 *
 * @param context
 *
 * @return NNContext
 * @private
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
void*
vaal_context_dict(VAALContext* context);

/**
 * Retrieves the model associated with the VAALContext.
 *
 * @param context
 *
 * @return NNContext
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
const void*
vaal_context_model(const VAALContext* context);

/**
 * Queries the number of available parameters in the current context.  The count
 * can be used along with @vaal_parameter_name() to query the names of the
 * parameters.
 *
 * You should have a model loaded using @ref vaal_load_model before calling this
 * function to ensure all available parameters are provided.
 *
 * The available parameters vary depending on model and target harwdware as
 * documented in the VAAL User's Manual (TODO: need to document parameters).
 *
 * @return number of currently available parameters for the context.
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
size_t
vaal_parameter_count(VAALContext* context);

/**
 * Queries the names of parameters according to the index.  Note the indices
 * are not deterministic and will vary depending on the state of the context.
 * This function is meant to be used to lookup currently available parameter
 * names.
 *
 * @return VAALError
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
VAALError
vaal_parameter_name(VAALContext* context,
                    size_t       index,
                    char*        name,
                    size_t       max_name,
                    size_t*      name_length);

/**
 * Queries the parameter for information about the type, length, and mutability.
 *
 * @param context
 * @param name Parameter name as found using @ref vaal_parameter_query
 * @param type Optional pointer to receive the parameter data type.
 * @param length Optional pointer to receive the length of the parameter. The
 *               length is measured in number of elements.  The total length can
 *               be calculated by multiplying with @ref vaal_type_sizeof().
 * @param readonly Optional pointer to receive the mutability of the parameter.
 *                 If readonly=1 then the parameter is immutable and attempts to
 *                 set a new value will return the error code
 *                 @ref VAAL_ERROR_PARAMETER_READ_ONLY.
 *
 * @return VAALType
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
VAALError
vaal_parameter_info(VAALContext* context,
                    const char*  name,
                    VAALType*    type,
                    size_t*      length,
                    int*         readonly);

/**
 * Reads the current value as a string for the parameter into the user provided
 * string buffer.  It can be called with max_value of 0 or value of NULL to
 * query the required storage size.  If length is non-NULL then the total size
 * of the string representation will be stored, even if the actual result was
 * truncated.
 *
 * @param context
 * @param name Parameter name as found using @ref vaal_parameter_query
 * @param value Buffer to receive the string representation of the value
 * @param max_value Size of the value buffer
 * @param length Total length of the string representation of the value
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_parameter_gets(VAALContext* context,
                    const char*  name,
                    char*        value,
                    size_t       max_value,
                    size_t*      length);

/**
 * Writes the provided value into the parameter.  The value is provided as a
 * string representation.
 *
 * @param context
 * @param name Parameter name as found using @ref vaal_parameter_query
 * @param value String representation of the new value for the parameter
 * @param length Optional length of string, if left out will use strlen(value)+1
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_parameter_sets(VAALContext* context,
                    const char*  name,
                    const char*  value,
                    size_t       length);

/**
 * Read the float values for a given "name" key in the context. It can be a
 * scalar value or an array.
 *
 * @param context
 * @param name Parameter name as found using @ref vaal_parameter_query
 * @param values pointer to the returned float value(s)
 * @param max_values length of the values array
 * @param num_values number of actual values in the parameter
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_parameter_getf(VAALContext* context,
                    const char*  name,
                    float*       values,
                    size_t       max_values,
                    size_t*      num_values);

/**
 * Writes the provided value into the parameter.  The value is provided as a
 * float array representation.
 *
 * @param context
 * @param name Parameter name as found using @ref vaal_parameter_query
 * @param values address of the first float value
 * @param num_values length of the input float array
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_parameter_setf(VAALContext* context,
                    const char*  name,
                    const float* values,
                    size_t       num_values);

/**
 * Read the int values for a given "name" key in the context. It can be a
 * scalar or an array.
 *
 * @param context
 * @param name Parameter name as found using @ref vaal_parameter_query
 * @param values pointer to the returned int values
 * @param max_values size of the values array
 * @param num_values number of actual values in the parameter
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_parameter_geti(VAALContext* context,
                    const char*  name,
                    int32_t*     values,
                    size_t       max_values,
                    size_t*      num_values);

/**
 * Writes the provided value into the parameter.  The value is provided as a
 * int array representation.
 *
 * @param context
 * @param name Parameter name as found using @ref vaal_parameter_query
 * @param value address of the first int value
 * @param len length of the input int array
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_parameter_seti(VAALContext*   context,
                    const char*    name,
                    const int32_t* values,
                    size_t         num_values);

/**
 * Read the int values for a given "name" key in the context. It can be a
 * scalar or an array.
 *
 * @param context
 * @param name Parameter name as found using @ref vaal_parameter_query
 * @param values pointer to the returned uint32_t values
 * @param max_salues length of the values array
 * @param num_values number of actual values in the parameter
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_parameter_getu(VAALContext* context,
                    const char*  name,
                    uint32_t*    values,
                    size_t       max_values,
                    size_t*      num_values);

/**
 * Writes the provided value into the parameter.  The value is provided as a
 * uint32_t array representation.
 *
 * @param context
 * @param name Parameter name as found using @ref vaal_parameter_query
 * @param value address of the first int value
 * @param len length of the input array
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_parameter_setu(VAALContext*    context,
                    const char*     name,
                    const uint32_t* values,
                    size_t          num_values);

/**
 * Writes the function pointer into the context under a given key.
 *
 * @param context
 * @param name Parameter name as found using @ref vaal_parameter_query
 * @param cb_ptr function pointer
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_parameter_set_func(VAALContext* context, const char* name, void* cb_ptr);

/**
 * Get the raw memory address of values given a key
 *
 * @param context
 * @param name Parameter name as found using @ref vaal_parameter_query
 * @param ptr Pointer to a void* which will receive the raw pointer to the data.
 * @param length Optional pointer to size_t to receive the size of the data
 * pointed to by ptr.
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_parameter_get_raw(VAALContext* context,
                       const char*  name,
                       void**       ptr,
                       size_t*      length);

/**
 * Loads a model into the context.  This function extends the DeepViewRT model
 * loading API to add model parameters and enabling input/output acceleration.
 *
 * This function is a convenience around @ref vaal_load_model() to load the
 * model from file into memory then calling the direct from memory function.
 *
 * This function will also attempt to detect the model's post-processing
 * requirements such as the box decoder and assign default post-processing
 * parameters.  These parameters can be adjusted using the various parameter set
 * and get functions.
 *
 * @param context The @ref VAALContext which will own this model.
 * @param filename the path to the DeepViewRT (RTM) model file.
 *
 * @return VAALError
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
VAALError
vaal_load_model_file(VAALContext* context, const char* filename);

/**
 * Loads a model into the context.  This function extends the DeepViewRT model
 * loading API to add model parameters and enabling input/output acceleration.
 *
 * This function will also attempt to detect the model's post-processing
 * requirements such as the box decoder and assign default post-processing
 * parameters.  These parameters can be adjusted using the various parameter set
 * and get functions.
 *
 * @param context The @ref VAALContext which will own this model.
 * @param memory_size the size of the model blob.
 * @param memory pointer to the start of the model blob (RTM file).
 *
 * @return VAALError
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
VAALError
vaal_load_model(VAALContext* context, size_t memory_size, const void* memory);

/**
 * Unloads the model from the current context.
 *
 * @param context The @ref VAALContext from which to unload the model.
 *
 * @return VAALError
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
VAALError
vaal_unload_model(VAALContext* context);

/**
 * Runs the model loaded by the VAALContext.
 *
 * @param context The @ref VAALContext with a loaded model to run.
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_run_model(VAALContext* context);

/**
 * Loads a video frame from virtual memory into the tensor, handling any
 * required conversions (such as casting to floating point, if required). The
 * frame must have a stride calculated from with and a known fourcc code, for
 * example YUYV would need stride to be width*2 whereas NV12 would required
 * stride to be width. For planar formats each plane must be packed
 * sequentially, so for NV12 the UV planes must follow immediately after the Y
 * plane.
 *
 * @param context VAALContext which owns the model into which the video frame
 * will be loaded.
 * @param tensor optional DeepViewRT tensor to receive the video frame.  If NULL
 * the model's first input tensor will be used.
 * @param memory the virtual memory location of the frame.
 * @param fourcc the fourcc code for the video frame which will be used to
 * convert to the model's expected format (assumed to be RGB).
 * @param width the width of the source frame, it will be resized to the target
 * tensor's width.
 * @param height the height of the source frame, it will be resized to the
 * target tensor's height.
 * @param roi optional roi which will cause the input frame to be cropped before
 * resizing.
 * @param proc mask of image pre-processing to apply on load.
 *
 * @return VAALError
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
VAALError
vaal_load_frame_memory(VAALContext*   context,
                       NNTensor*      tensor,
                       const void*    memory,
                       uint32_t       fourcc,
                       int32_t        width,
                       int32_t        height,
                       const int32_t* roi,
                       uint32_t       proc);

/**
 * Loads a video frame from physical memory into the tensor, handling any
 * required conversions (such as casting to floating point, if required). The
 * frame must have a stride calculated from with and a known fourcc code, for
 * example YUYV would need stride to be width*2 whereas NV12 would required
 * stride to be width. For planar formats each plane must be packed
 * sequentially, so for NV12 the UV planes must follow immediately after the Y
 * plane.
 *
 * @param context VAALContext which owns the model into which the video frame
 * will be loaded.
 * @param tensor optional DeepViewRT tensor to receive the video frame.  If NULL
 * the model's first input tensor will be used.
 * @param memory the virtual memory location of the frame.
 * @param fourcc the fourcc code for the video frame which will be used to
 * convert to the model's expected format (assumed to be RGB).
 * @param width the width of the source frame, it will be resized to the target
 * tensor's width.
 * @param height the height of the source frame, it will be resized to the
 * target tensor's height.
 * @param roi optional roi which will cause the input frame to be cropped before
 * resizing.
 * @param proc mask of image pre-processing to apply on load.
 *
 * @return VAALError
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
VAALError
vaal_load_frame_physical(VAALContext*   context,
                         NNTensor*      tensor,
                         uint64_t       physical,
                         uint32_t       fourcc,
                         int32_t        width,
                         int32_t        height,
                         const int32_t* roi,
                         uint32_t       proc);

/**
 * Loads a video frame from a dmabuf video buffer into the tensor, handling any
 * required conversions (such as casting to floating point, if required). The
 * frame must have a stride calculated from with and a known fourcc code, for
 * example YUYV would need stride to be width*2 whereas NV12 would required
 * stride to be width. For planar formats each plane must be packed
 * sequentially, so for NV12 the UV planes must follow immediately after the Y
 * plane.
 *
 * @param context VAALContext which owns the model into which the video frame
 * will be loaded.
 * @param tensor optional DeepViewRT tensor to receive the video frame.  If NULL
 * the model's first input tensor will be used.
 * @param dmabuf file descriptor to a dmabuf holding the video frame.
 * @param fourcc the fourcc code for the video frame which will be used to
 * convert to the model's expected format (assumed to be RGB).
 * @param width the width of the source frame, it will be resized to the target
 * tensor's width.
 * @param height the height of the source frame, it will be resized to the
 * target tensor's height.
 * @param roi optional roi which will cause the input frame to be cropped before
 * resizing.
 * @param proc mask of image pre-processing to apply on load.
 *
 * @return VAALError
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
VAALError
vaal_load_frame_dmabuf(VAALContext*   context,
                       NNTensor*      tensor,
                       int            dmabuf,
                       uint32_t       fourcc,
                       int32_t        width,
                       int32_t        height,
                       const int32_t* roi,
                       uint32_t       proc);

/**
 * Convenience around the @ref vaal_load_frame_memory() function which will
 * first decode the image into a raw buffer.  Once the image has been decoded it
 * will be sent to the load_frame function for further processing.
 *
 * To load an image directly from file see @ref vaal_load_image_file() instead.
 *
 * The following image formats are supported.
 *
 * - JPEG
 * - PNG
 * - BMP
 * - TIFF
 *
 * @param context VAALContext which owns the model into which the video frame
 * will be loaded.
 * @param tensor optional DeepViewRT tensor to receive the video frame.  If NULL
 * the model's first input tensor will be used.
 * @param image buffer to encoded image.
 * @param len size of the image buffer.
 * @param roi optional roi which will cause the input frame to be cropped before
 * resizing.
 * @param proc mask of image pre-processing to apply on load.
 *
 * @return VAALError
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
VAALError
vaal_load_image(VAALContext*   context,
                NNTensor*      tensor,
                const uint8_t* image,
                size_t         len,
                const int32_t  roi[4],
                uint32_t       proc);

/**
 * Convenience around the @ref vaal_load_image() function which first loads the
 * image from a file.
 *
 * @param context VAALContext which owns the model into which the video frame
 * will be loaded.
 * @param tensor optional DeepViewRT tensor to receive the video frame.  If NULL
 * the model's first input tensor will be used.
 * @param filename path to image to be loaded.
 * @param roi optional roi which will cause the input frame to be cropped before
 * resizing.
 * @param proc mask of image pre-processing to apply on load.
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_load_image_file(VAALContext*   context,
                     NNTensor*      tensor,
                     const char*    filename,
                     const int32_t* roi,
                     uint32_t       proc);

/**
 * Returns the text value for the label.  If the label is not available in the
 * model then NULL will be returned.
 *
 * @param context The VAALContext owning the model from which we want labels.
 * @param label The label index to be translated to a string.
 *
 * @return label string or NULL.
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
const char*
vaal_label(VAALContext* context, int label);

/**
 * Handles post-processing the model's output and reading the bounding boxes
 * into the user provided array of boxes, up to @param max_boxes.  The function
 * can be called with max_boxes set to 0 to cause the function to return the
 * number of boxes detected, doing so causes the post-processing function to
 * avoid storing the box results into @param boxes and ignoring @param max_boxes
 * for early stopping.
 *
 * @param context The VAALContext which owns the model from which we want boxes.
 * @param boxes An array of boxes which will received the decoded results.
 * @param max_boxes The size of @param boxes limits the maximum number of boxes.
 * @param num_boxes The number of boxes detected.
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_boxes(VAALContext* context,
           VAALBox*     boxes,
           size_t       max_boxes,
           size_t*      num_boxes);

/**
 * @brief Handles post-processing of the model's outputs and reading the
 * keypoints into the user provided array of keypoints, up to @param
 * max_keypoints.
 *
 * @param context The VAALContext which owns the model from which we want
 * keypoints.
 * @param keypoints An array of keypoints which will receive the decoded
 * results.
 * @param max_keypoints The size of @param keypoints limits the maximum number
 * of keypoints.
 * @param num_keypoints The number of keypoints detected.
 *
 * @return VAALError
 *
 * @since 1.4
 */
VAAL_AVAILABLE_SINCE_1_4
VAAL_API
VAALError
vaal_keypoints(VAALContext*  context,
               VAALKeypoint* keypoints,
               size_t        max_keypoints,
               size_t*       num_keypoints);

/**
 * @brief Handles post-processing of the model's outputs and reading the euler
 * angles into the user provided array of orientations. This function currently
 * will only return a single set of euler angles, but will be extended in the
 * future to have the same capability as other post-processing functions.
 *
 * @param context The VAALContext which owns the model from which we want euler
 * angles.
 * @param orientations An array of orientations which will receive the decoded
 * results.
 * @param num_orientations The number of orientations detected. (Will always be
 * 1 currently)
 *
 * @return VAALError
 *
 * @since 1.4
 */
VAAL_AVAILABLE_SINCE_1_4
VAAL_API
VAALError
vaal_euler(VAALContext* context,
           VAALEuler*   orientations,
           size_t*      num_orientations);

/**
 * Access the DeepViewRT NNTensor objects for the model's output.  The function
 * supports multiple outputs which are selected using the index parameter.  To
 * confirm the number of outputs you may use the @ref vaal_output_count()
 * function and to read the names of the outputs as defined in the model file
 * you may use @ref vaal_output_name().
 *
 * An output will fail to be found if there's no model loaded, the context is
 * not valid, or the index is out of range.  It is also possible the model has
 * no outputs identified.  In these cases the function will return NULL.
 *
 * @param context The VAALContext which owns the model from which we want to
 * retrieve output tensors.
 * @param index The output layer index.
 *
 * @return NNTensor or NULL if the output cannot be found, see notes.
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
NNTensor*
vaal_output_tensor(VAALContext* context, int index);

/**
 * Returns the number of tensor outputs defined by the current model.  If the
 * context is invalid, has no model loaded, or the model does not identify any
 * outputs then the function will return 0.
 *
 * @param context The VAALContext which owns the model from which we want to
 * count the number of outputs.
 *
 * @return The number of outputs defined by the model.
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
int
vaal_output_count(VAALContext* context);

/**
 * Returns the layer name from the model which corresponds to the output at the
 * provided index.  If the context is invalid, has no model loaded, or the index
 * is out of range then the function will return NULL.
 *
 * @param context The VAALContext which owns the model from which we want to get
 * the output name.
 * @param index the output layer index.
 *
 * @return The name of the layer in the model which owns the output tensor.
 *
 * @since 1.1
 */
VAAL_AVAILABLE_SINCE_1_1
VAAL_API
const char*
vaal_output_name(VAALContext* context, int index);

/**
 * Callback to customized remap model outputs to decoder/postprocessing
 *
 */
typedef VAALError (*cus_remap_tensors_func_t)(VAALContext* context,
                                              NNTensor**   detection_tensors);
/**
 * Callback to customized decoder
 *
 */
typedef VAALError (*cus_decode_func_t)(VAALContext* ctx,
                                       NNTensor**   detection_tensors,
                                       NNTensor**   score_box_tensors);

//////////////////////////////////////////////
//                legacy
//////////////////////////////////////////////

/**
 * The following are low-level box-decoding API provided by VAAL.  Typically a
 * user would be expected to use the @ref vaal_read_boxes function and avoid
 * using the low-level functions directly but they are exposed for cases where
 * the user wishes to control the post-processing directly.
 */

/**
 * CenterNet decoder.
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_postprocessing_centernet(NNTensor* heatmap_tensor,
                              NNTensor* regression_tensor,
                              NNTensor* size_tensor,
                              NNTensor* cache,
                              NNTensor* decode_out);

/**
 * YOLO decoder.
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_postprocessing_centernet_sigmoid(NNTensor* heatmap_tensor,
                                      NNTensor* regression_tensor,
                                      NNTensor* size_tensor,
                                      NNTensor* cache_tensor,
                                      NNTensor* decode_out);

VAAL_API
VAALError
vaal_postprocessing_yolo(NNTensor** feature_tensors,
                         int        input_shape,
                         int        yolo_model_idx,
                         NNTensor*  cache,
                         float      score_threshold,
                         float      iou_threshold,
                         int        max_output_size_per_class,
                         NNTensor*  bbx_out_tensor,
                         NNTensor*  bbx_out_dim_tensor);

VAAL_API
VAALError
vaal_postprocessing_nms(NNTensor* scores_tensor,
                        NNTensor* boxes_tensor,
                        NNTensor* cache_tensor,
                        float     score_threshold,
                        float     iou_threshold,
                        int       max_output_size_per_class,
                        NNTensor* bbx_out_tensor,
                        NNTensor* bbx_out_dim_tensor);

VAAL_API
VAALError
vaal_facedet_decode(NNTensor* priors,
                    NNTensor* loc,
                    NNTensor* iou,
                    NNTensor* conf,
                    NNTensor* scores,
                    NNTensor* boxes);

/**
 * Box NMS.
 *
 * @return VAALError
 *
 * @since 1.0
 */
VAAL_AVAILABLE_SINCE_1_0
VAAL_API
VAALError
vaal_postprocessing_ssd_standard_bbx(NNTensor* score_tensor,
                                     NNTensor* trans,
                                     NNTensor* anchors,
                                     NNTensor* cache,
                                     float     score_threshold,
                                     float     iou_threshold,
                                     int32_t   max_output_size,
                                     NNTensor* bbx_out_tensor,
                                     NNTensor* bbx_out_dim_tensor);

VAAL_API
VAALError
vaal_remap_detection_tensors(NNModel*   mode,
                             NNContext* context,
                             NNTensor** detection_tensors);

VAAL_API
VAALError
vaal_set_class_filter(NNTensor* decode_out, int* class_idx_array, int len);

VAAL_API
VAALError
vaal_set_nms_type(NNTensor* decode_out, int nms_type_in);

VAAL_API
char*
vaal_check_model_string(const int idx);

VAAL_API
void
vaal_set_detection_model_type(NNModel*   model,
                              NNContext* context,
                              int        model_type_id);

VAAL_API
VAALError
detection_remap_tensors(VAALContext* context, NNTensor** detection_tensors);

enum vaal_model_type {
    model_type_none             = 0,
    model_type_people_detection = 10,
    model_type_face_detection   = 20,
    model_type_head_pose        = 30,
    model_type_human_pose       = 40,
};

/**
 * @brief Returns the model path that VAAL will search for models requested.
 *
 * @return const char*
 *
 * @since 1.4
 */
VAAL_AVAILABLE_SINCE_1_4
VAAL_API
const char*
vaal_model_path();

/**
 * @brief Creates a VAALContext that is using the specified engine and attempts
 * to load a model that is of the provided model type. Must call 
 * vaal_context_release on returned context, to avoid memory leak.
 *
 * @param engine The engine that this context should use ('cpu', 'gpu', 'npu')
 *
 * @param m_type The type of model to load into this context
 *
 * @return VAALContext* A fully initialized context, which will be loaded with
 * the specified model type if it was found.
 *
 * @since 1.4
 */
VAAL_AVAILABLE_SINCE_1_4
VAAL_API
VAALContext*
vaal_model_probe(const char* engine, enum vaal_model_type m_type);

#ifdef __cplusplus
}
#endif

#endif /* DEEPVIEW_VAAL_H */
