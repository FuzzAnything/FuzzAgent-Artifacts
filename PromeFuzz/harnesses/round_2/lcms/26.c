// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetAlarmCodes at cmsxform.c:113:16 in lcms2.h
// cmsGetAlarmCodes at cmsxform.c:120:16 in lcms2.h
// cmsSetAlarmCodesTHR at cmsxform.c:93:16 in lcms2.h
// cmsGetAlarmCodesTHR at cmsxform.c:104:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a simple sRGB profile for testing
    return cmsCreate_sRGBProfileTHR(ctx);
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform) {
        cmsDeleteTransform(transform);
    }
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Create dummy profiles for testing
    cmsHPROFILE input_profile = create_dummy_profile(ctx);
    cmsHPROFILE output_profile = create_dummy_profile(ctx);
    cmsHPROFILE proofing_profile = create_dummy_profile(ctx);
    
    if (!input_profile || !output_profile || !proofing_profile) {
        cleanup_profile(input_profile);
        cleanup_profile(output_profile);
        cleanup_profile(proofing_profile);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Extract parameters from fuzzer input
    // We'll use the data to determine various parameters for the API calls
    
    // For alarm codes - use first cmsMAXCHANNELS*2 bytes if available
    cmsUInt16Number alarmCodes[cmsMAXCHANNELS];
    cmsUInt16Number newAlarmCodes[cmsMAXCHANNELS];
    
    // Initialize alarm codes with some default values
    for (int i = 0; i < cmsMAXCHANNELS; i++) {
        alarmCodes[i] = 0xFFFF;
        newAlarmCodes[i] = 0;
    }
    
    // Use fuzzer data to populate alarm codes if we have enough data
    if (Size >= cmsMAXCHANNELS * sizeof(cmsUInt16Number)) {
        memcpy(alarmCodes, Data, cmsMAXCHANNELS * sizeof(cmsUInt16Number));
        memcpy(newAlarmCodes, Data, cmsMAXCHANNELS * sizeof(cmsUInt16Number));
    }

    // Test global alarm code functions
    cmsSetAlarmCodes(alarmCodes);
    cmsGetAlarmCodes(newAlarmCodes);
    
    // Test per-context alarm code functions
    cmsSetAlarmCodesTHR(ctx, alarmCodes);
    cmsGetAlarmCodesTHR(ctx, newAlarmCodes);

    // Extract transform parameters from fuzzer data
    // We'll use different parts of the input for different parameters
    size_t offset = cmsMAXCHANNELS * sizeof(cmsUInt16Number);
    
    cmsUInt32Number inputFormat = TYPE_RGB_8;
    cmsUInt32Number outputFormat = TYPE_RGB_8;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number dwFlags = 0;
    cmsUInt32Number proofingIntent = INTENT_ABSOLUTE_COLORIMETRIC;
    
    // Use fuzzer data to modify parameters if available
    if (Size >= offset + 4 * sizeof(cmsUInt32Number)) {
        // Use bytes to influence parameter values
        inputFormat = *(cmsUInt32Number*)(Data + offset) % 100;
        outputFormat = *(cmsUInt32Number*)(Data + offset + 4) % 100;
        intent = *(cmsUInt32Number*)(Data + offset + 8) % 4;
        dwFlags = *(cmsUInt32Number*)(Data + offset + 12);
        proofingIntent = *(cmsUInt32Number*)(Data + offset + 16) % 4;
    }

    // Test cmsCreateTransformTHR with various combinations
    cmsHTRANSFORM transform1 = cmsCreateTransformTHR(ctx, 
                                                    input_profile,
                                                    inputFormat,
                                                    output_profile,
                                                    outputFormat,
                                                    intent,
                                                    dwFlags);
    
    // Test with NULL output profile (device link)
    cmsHTRANSFORM transform2 = cmsCreateTransformTHR(ctx,
                                                    input_profile,
                                                    inputFormat,
                                                    NULL,
                                                    outputFormat,
                                                    intent,
                                                    dwFlags);
    
    // Test with NULL input profile
    cmsHTRANSFORM transform3 = cmsCreateTransformTHR(ctx,
                                                    NULL,
                                                    inputFormat,
                                                    output_profile,
                                                    outputFormat,
                                                    intent,
                                                    dwFlags);

    // Test cmsCreateProofingTransformTHR
    cmsHTRANSFORM proofingTransform = cmsCreateProofingTransformTHR(ctx,
                                                                    input_profile,
                                                                    inputFormat,
                                                                    output_profile,
                                                                    outputFormat,
                                                                    proofing_profile,
                                                                    intent,
                                                                    proofingIntent,
                                                                    dwFlags);
    
    // Test with different flag combinations
    cmsUInt32Number softProofFlags = dwFlags | cmsFLAGS_SOFTPROOFING;
    cmsHTRANSFORM softProofTransform = cmsCreateProofingTransformTHR(ctx,
                                                                    input_profile,
                                                                    inputFormat,
                                                                    output_profile,
                                                                    outputFormat,
                                                                    proofing_profile,
                                                                    intent,
                                                                    proofingIntent,
                                                                    softProofFlags);
    
    cmsUInt32Number gamutCheckFlags = dwFlags | cmsFLAGS_GAMUTCHECK;
    cmsHTRANSFORM gamutCheckTransform = cmsCreateProofingTransformTHR(ctx,
                                                                    input_profile,
                                                                    inputFormat,
                                                                    output_profile,
                                                                    outputFormat,
                                                                    proofing_profile,
                                                                    intent,
                                                                    proofingIntent,
                                                                    gamutCheckFlags);

    // Clean up transforms
    cleanup_transform(transform1);
    cleanup_transform(transform2);
    cleanup_transform(transform3);
    cleanup_transform(proofingTransform);
    cleanup_transform(softProofTransform);
    cleanup_transform(gamutCheckTransform);

    // Clean up profiles
    cleanup_profile(input_profile);
    cleanup_profile(output_profile);
    cleanup_profile(proofing_profile);
    
    // Clean up context
    cmsDeleteContext(ctx);

    return 0;
}