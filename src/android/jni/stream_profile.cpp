// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2019 Intel Corporation. All Rights Reserved.

#include <jni.h>
#include "error.h"
#include "../../../include/librealsense2/rs.h"

extern "C" JNIEXPORT void JNICALL
Java_com_intel_realsense_librealsense_StreamProfile_nGetProfile(JNIEnv *env, jclass type,
                                                                jlong handle, jobject params) {
    rs2_stream stream_type = RS2_STREAM_ANY;
    rs2_format format = RS2_FORMAT_ANY;
    int index = -1;
    int uniqueId = -1;
    int frameRate = -1;
    rs2_error *e = NULL;

    rs2_get_stream_profile_data((const rs2_stream_profile *) handle, &stream_type, &format, &index, &uniqueId, &frameRate, &e);
    handle_error(env, e);

    jclass clazz = env->GetObjectClass(params);

    jfieldID typeField = env->GetFieldID(clazz, "type", "I");
    jfieldID formatField = env->GetFieldID(clazz, "format", "I");
    jfieldID indexField = env->GetFieldID(clazz, "index", "I");
    jfieldID uniqueIdField = env->GetFieldID(clazz, "uniqueId", "I");
    jfieldID frameRateField = env->GetFieldID(clazz, "frameRate", "I");

    env->SetIntField(params, typeField, stream_type);
    env->SetIntField(params, formatField, format);
    env->SetIntField(params, indexField, index);
    env->SetIntField(params, uniqueIdField, uniqueId);
    env->SetIntField(params, frameRateField, frameRate);
}

extern "C" JNIEXPORT void JNICALL
Java_com_intel_realsense_librealsense_VideoStreamProfile_nGetResolution(JNIEnv *env, jclass type,
                                                                        jlong handle,
                                                                        jobject params) {
    int width = -1;
    int height = -1;
    rs2_error *e = NULL;

    rs2_get_video_stream_resolution((const rs2_stream_profile *) handle, &width, &height, &e);
    handle_error(env, e);

    jclass clazz = env->GetObjectClass(params);

    jfieldID widthField = env->GetFieldID(clazz, "width", "I");
    jfieldID heightField = env->GetFieldID(clazz, "height", "I");

    env->SetIntField(params, widthField, width);
    env->SetIntField(params, heightField, height);
}

extern "C" JNIEXPORT void JNICALL
Java_com_intel_realsense_librealsense_StreamProfile_nDelete(JNIEnv *env, jclass type,
                                                            jlong handle) {
    rs2_delete_stream_profile((rs2_stream_profile *) handle);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_intel_realsense_librealsense_StreamProfile_nIsProfileExtendableTo(JNIEnv *env, jclass type,
                                                                           jlong handle,
                                                                           jint extension) {
    rs2_error *e = NULL;
    int rv = rs2_stream_profile_is(reinterpret_cast<const rs2_stream_profile *>(handle),
                                   static_cast<rs2_extension>(extension), &e);
    handle_error(env, e);
    return rv;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_intel_realsense_librealsense_StreamProfile_nGetExtrinsicsTo(JNIEnv *env, jclass type,
                                                                         jlong handle, jlong otherHandle, jobject extrinsics) {
    rs2_error* e = nullptr;
    rs2_extrinsics extr;
    rs2_get_extrinsics(reinterpret_cast<const rs2_stream_profile *>(handle),
            reinterpret_cast<const rs2_stream_profile *>(otherHandle),&extr, &e);
    handle_error(env, e);

    if (e != nullptr)
    {
        return;
    }

    jclass clazz = env->GetObjectClass(extrinsics);

    //retrieving the array members
    //mRotation
    jfieldID rotation_field = env->GetFieldID(clazz, "mRotation", "[F");
    jfloatArray rotationArray = env->NewFloatArray(9);
    if (rotationArray != NULL)
    {
        env->SetFloatArrayRegion(rotationArray, 0, 9, reinterpret_cast<jfloat*>(&extr.rotation));
    }
    env->SetObjectField(extrinsics, rotation_field, rotationArray);

    //mTranslation
    jfieldID translation_field = env->GetFieldID(clazz, "mTranslation", "[F");
    jfloatArray translationArray = env->NewFloatArray(3);
    if (translationArray != NULL)
    {
        env->SetFloatArrayRegion(translationArray, 0, 3, reinterpret_cast<jfloat*>(&extr.translation));
    }
    env->SetObjectField(extrinsics, translation_field, translationArray);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_intel_realsense_librealsense_VideoStreamProfile_nGetIntrinsics(JNIEnv *env, jclass type,
                                                                   jlong handle, jobject intrinsics) {
    rs2_error* e = nullptr;
    rs2_intrinsics intr;
    rs2_get_video_stream_intrinsics(reinterpret_cast<const rs2_stream_profile *>(handle), &intr, &e);
    handle_error(env, e);

    if (e != nullptr)
    {
        return;
    }

    jclass clazz = env->GetObjectClass(intrinsics);

    //retrieving all the built-in types members
    jfieldID width_field = env->GetFieldID(clazz, "mWidth", "I");
    jfieldID height_field = env->GetFieldID(clazz, "mHeight", "I");
    jfieldID ppx_field = env->GetFieldID(clazz, "mPpx", "F");
    jfieldID ppy_field = env->GetFieldID(clazz, "mPpy", "F");
    jfieldID fx_field = env->GetFieldID(clazz, "mFx", "F");
    jfieldID fy_field = env->GetFieldID(clazz, "mFy", "F");
    jfieldID model_field = env->GetFieldID(clazz, "mModelValue", "I");


    env->SetIntField(intrinsics, width_field, intr.width);
    env->SetIntField(intrinsics, height_field, intr.height);
    env->SetFloatField(intrinsics, ppx_field, intr.ppx);
    env->SetFloatField(intrinsics, ppy_field, intr.ppy);
    env->SetFloatField(intrinsics, fx_field, intr.fx);
    env->SetFloatField(intrinsics, fy_field, intr.fy);
    env->SetIntField(intrinsics, model_field, intr.model);

    //retrieving the array member
    jfieldID coeff_field = env->GetFieldID(clazz, "mCoeffs", "[F");
    jfloatArray coeffsArray = env->NewFloatArray(5);
    if (coeffsArray != NULL)
    {
        env->SetFloatArrayRegion(coeffsArray, 0, 5, reinterpret_cast<jfloat*>(&intr.coeffs));
    }
    env->SetObjectField(intrinsics, coeff_field, coeffsArray);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_intel_realsense_librealsense_MotionStreamProfile_nGetIntrinsics(JNIEnv *env, jclass type,
                                                                        jlong handle, jobject intrinsics) {
    rs2_error* e = nullptr;
    rs2_motion_device_intrinsic intr;
    rs2_get_motion_intrinsics(reinterpret_cast<const rs2_stream_profile *>(handle), &intr, &e);
    handle_error(env, e);

    if (e != nullptr)
    {
        return;
    }

    jclass clazz = env->GetObjectClass(intrinsics);

    //retrieving the array members
    //mData
    jfieldID data_field = env->GetFieldID(clazz, "mData", "[F");
    jfloatArray dataArray = env->NewFloatArray(12);
    if (dataArray != NULL)
    {
        env->SetFloatArrayRegion(dataArray, 0, 12, reinterpret_cast<jfloat*>(&intr.data));
    }
    env->SetObjectField(intrinsics, data_field, dataArray);

    //mNoiseVariances
    jfieldID noise_field = env->GetFieldID(clazz, "mNoiseVariances", "[F");
    jfloatArray noiseArray = env->NewFloatArray(3);
    if (noiseArray != NULL)
    {
        env->SetFloatArrayRegion(noiseArray, 0, 3, reinterpret_cast<jfloat*>(&intr.noise_variances));
    }
    env->SetObjectField(intrinsics, noise_field, noiseArray);

    //mBiasVariances
    jfieldID bias_field = env->GetFieldID(clazz, "mBiasVariances", "[F");
    jfloatArray biasArray = env->NewFloatArray(3);
    if (biasArray != NULL)
    {
        env->SetFloatArrayRegion(biasArray, 0, 3, reinterpret_cast<jfloat*>(&intr.bias_variances));
    }
    env->SetObjectField(intrinsics, bias_field, biasArray);
}
