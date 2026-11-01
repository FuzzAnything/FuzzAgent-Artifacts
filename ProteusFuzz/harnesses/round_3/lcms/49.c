// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransform at cmsxform.c:192:16 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_transform(cmsHTRANSFORM hTransform) {
    if (hTransform != NULL) {
        cmsDeleteTransform(hTransform);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    // Try to create a profile from the input data
    // If the data is not a valid ICC profile, this will return NULL
    return cmsOpenProfileFromMem(data, size);
}

static cmsHTRANSFORM create_transform_from_profiles(
    cmsHPROFILE input, cmsHPROFILE output,
    cmsUInt32Number inputFormat, cmsUInt32Number outputFormat,
    cmsUInt32Number intent, cmsUInt32Number flags) {
    
    if (input == NULL || output == NULL) {
        return NULL;
    }
    
    return cmsCreateTransform(input, inputFormat, output, outputFormat, intent, flags);
}

static cmsHTRANSFORM create_proofing_transform_from_profiles(
    cmsHPROFILE input, cmsHPROFILE output, cmsHPROFILE proofing,
    cmsUInt32Number inputFormat, cmsUInt32Number outputFormat,
    cmsUInt32Number intent, cmsUInt32Number proofingIntent, cmsUInt32Number flags) {
    
    if (input == NULL || output == NULL || proofing == NULL) {
        return NULL;
    }
    
    return cmsCreateProofingTransform(input, inputFormat, output, outputFormat,
                                      proofing, intent, proofingIntent, flags);
}

static cmsHTRANSFORM create_multiprofile_transform_from_profiles(
    cmsHPROFILE *profiles, cmsUInt32Number nProfiles,
    cmsUInt32Number inputFormat, cmsUInt32Number outputFormat,
    cmsUInt32Number intent, cmsUInt32Number flags) {
    
    if (nProfiles == 0 || profiles == NULL) {
        return NULL;
    }
    
    // Check if all profiles are valid
    for (cmsUInt32Number i = 0; i < nProfiles; i++) {
        if (profiles[i] == NULL) {
            return NULL;
        }
    }
    
    return cmsCreateMultiprofileTransform(profiles, nProfiles, inputFormat, outputFormat, intent, flags);
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsHPROFILE inputProfile = NULL;
    cmsHPROFILE outputProfile = NULL;
    cmsHPROFILE proofingProfile = NULL;
    cmsHTRANSFORM transform = NULL;
    cmsHTRANSFORM proofingTransform = NULL;
    cmsHTRANSFORM multiprofileTransform = NULL;
    
    cmsUInt32Number inputFormat = 0;
    cmsUInt32Number outputFormat = 0;
    cmsUInt32Number intent = 0;
    cmsUInt32Number proofingIntent = 0;
    cmsUInt32Number flags = 0;
    
    // Extract parameters from input data
    inputFormat = Data[0];
    outputFormat = Data[1];
    intent = Data[2];
    proofingIntent = Data[3];
    flags = (Size > 4) ? Data[4] : 0;
    
    // Create dummy profiles from input data
    inputProfile = create_dummy_profile(Data, Size);
    outputProfile = create_dummy_profile(Data, Size);
    proofingProfile = create_dummy_profile(Data, Size);
    
    // Test cmsCreateTransform
    if (inputProfile != NULL && outputProfile != NULL) {
        transform = create_transform_from_profiles(inputProfile, outputProfile, inputFormat, outputFormat, intent, flags);
        if (transform != NULL) {
            cmsUInt32Number outputFmt = cmsGetTransformOutputFormat(transform);
            (void)outputFmt; // Use the return value to avoid unused variable warning
            
            // Test cmsDoTransform
            size_t bufferSize = 1024;
            uint8_t *inputBuffer = malloc(bufferSize);
            uint8_t *outputBuffer = malloc(bufferSize);
            if (inputBuffer != NULL && outputBuffer != NULL) {
                memset(inputBuffer, 0, bufferSize);
                cmsDoTransform(transform, inputBuffer, outputBuffer, 100);
                free(inputBuffer);
                free(outputBuffer);
            }
            
            // Test cmsDoTransformLineStride
            if (inputBuffer != NULL && outputBuffer != NULL) {
                cmsDoTransformLineStride(transform, inputBuffer, outputBuffer, 10, 10, 100, 100, 100, 100);
            }
            
            cleanup_transform(transform);
        }
    }
    
    // Test cmsCreateProofingTransform
    if (inputProfile != NULL && outputProfile != NULL && proofingProfile != NULL) {
        proofingTransform = create_proofing_transform_from_profiles(
            inputProfile, outputProfile, proofingProfile,
            inputFormat, outputFormat, intent, proofingIntent, flags);
        if (proofingTransform != NULL) {
            cmsUInt32Number outputFmt = cmsGetTransformOutputFormat(proofingTransform);
            (void)outputFmt; // Use the return value to avoid unused variable warning
            
            // Test cmsDoTransform
            size_t bufferSize = 1024;
            uint8_t *inputBuffer = malloc(bufferSize);
            uint8_t *outputBuffer = malloc(bufferSize);
            if (inputBuffer != NULL && outputBuffer != NULL) {
                memset(inputBuffer, 0, bufferSize);
                cmsDoTransform(proofingTransform, inputBuffer, outputBuffer, 100);
                free(inputBuffer);
                free(outputBuffer);
            }
            
            // Test cmsDoTransformLineStride
            if (inputBuffer != NULL && outputBuffer != NULL) {
                cmsDoTransformLineStride(proofingTransform, inputBuffer, outputBuffer, 10, 10, 100, 100, 100, 100);
            }
            
            cleanup_transform(proofingTransform);
        }
    }
    
    // Test cmsCreateMultiprofileTransform
    if (inputProfile != NULL && outputProfile != NULL) {
        cmsHPROFILE profiles[2] = {inputProfile, outputProfile};
        multiprofileTransform = create_multiprofile_transform_from_profiles(
            profiles, 2, inputFormat, outputFormat, intent, flags);
        if (multiprofileTransform != NULL) {
            cmsUInt32Number outputFmt = cmsGetTransformOutputFormat(multiprofileTransform);
            (void)outputFmt; // Use the return value to avoid unused variable warning
            
            // Test cmsDoTransform
            size_t bufferSize = 1024;
            uint8_t *inputBuffer = malloc(bufferSize);
            uint8_t *outputBuffer = malloc(bufferSize);
            if (inputBuffer != NULL && outputBuffer != NULL) {
                memset(inputBuffer, 0, bufferSize);
                cmsDoTransform(multiprofileTransform, inputBuffer, outputBuffer, 100);
                free(inputBuffer);
                free(outputBuffer);
            }
            
            // Test cmsDoTransformLineStride
            if (inputBuffer != NULL && outputBuffer != NULL) {
                cmsDoTransformLineStride(multiprofileTransform, inputBuffer, outputBuffer, 10, 10, 100, 100, 100, 100);
            }
            
            cleanup_transform(multiprofileTransform);
        }
    }
    
    // Cleanup profiles
    cleanup_profile(inputProfile);
    cleanup_profile(outputProfile);
    cleanup_profile(proofingProfile);
    
    return 0;
}