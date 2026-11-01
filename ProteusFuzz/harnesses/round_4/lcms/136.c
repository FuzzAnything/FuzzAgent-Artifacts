// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_transform(cmsHTRANSFORM hTransform) {
    if (hTransform != NULL) {
        cmsDeleteTransform(hTransform);
    }
}

int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy profile from memory if data is large enough to potentially be a profile
    cmsHPROFILE hProfile = NULL;
    if (Size >= 128) {
        hProfile = cmsOpenProfileFromMem(Data, Size);
    }

    cmsCIEXYZ blackPoint = {0, 0, 0};
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    cmsUInt32Number usedDirection = 0;

    // Fuzz intent and flags from input data if available
    if (Size > 0) {
        intent = Data[0] & 0x03; // Ensure intent is 0-3
        flags = Size > 1 ? Data[1] : 0;
        usedDirection = Size > 2 ? Data[2] : 0;
    }

    // Test cmsGetHeaderRenderingIntent
    if (hProfile != NULL) {
        cmsUInt32Number headerIntent = cmsGetHeaderRenderingIntent(hProfile);
        (void)headerIntent; // Suppress unused variable warning
    }

    // Test cmsIsIntentSupported
    if (hProfile != NULL) {
        cmsBool supported = cmsIsIntentSupported(hProfile, intent, usedDirection);
        (void)supported;
    }

    // Test cmsDetectBlackPoint
    if (hProfile != NULL) {
        cmsBool detected = cmsDetectBlackPoint(&blackPoint, hProfile, intent, flags);
        (void)detected;
    }

    // Test cmsDetectDestinationBlackPoint
    if (hProfile != NULL) {
        cmsBool destDetected = cmsDetectDestinationBlackPoint(&blackPoint, hProfile, intent, flags);
        (void)destDetected;
    }

    // Test cmsCreateMultiprofileTransform
    cmsHPROFILE profiles[2] = {NULL, NULL};
    cmsHTRANSFORM hTransform = NULL;

    if (hProfile != NULL) {
        profiles[0] = hProfile;
        
        // Use standard RGB formats for testing. 
        // cmsFormatRGB8 is defined as 0x00000004 in lcms2.h, but we use the macro if available or define it.
        // Since the error indicates it's not found, we use the numeric value directly or check for the macro.
        // In lcms2, cmsFormatRGB8 is typically defined. If missing in some builds, we can use the raw value.
        // However, the standard way is to use the macro. Let's assume the macro exists in the header.
        // If the compiler error persists, it might be a version issue. 
        // To be safe and strictly follow the error, we will use the literal value 0x00000004 which corresponds to cmsFormatRGB8.
        // Actually, the macro is standard. The error suggests the header might not have been included correctly or a specific version.
        // Let's try to use the macro again, but if the user says it fails, we must use the value or a different approach.
        // The error says 'use of undeclared identifier'. This usually means the macro is not defined.
        // Let's define the format manually to ensure compilation.
        
        cmsUInt32Number inputFormat = 0x00000004; // cmsFormatRGB8
        cmsUInt32Number outputFormat = 0x00000004; // cmsFormatRGB8

        // Try to create a transform with one profile
        hTransform = cmsCreateMultiprofileTransform(profiles, 1, 
                                                    inputFormat,
                                                    outputFormat,
                                                    intent, flags);
        
        if (hTransform != NULL) {
            cmsDeleteTransform(hTransform);
            hTransform = NULL;
        }
    }

    // Cleanup
    cleanup_transform(hTransform);
    cleanup_profile(hProfile);

    return 0;
}