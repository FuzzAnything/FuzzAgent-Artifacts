// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetAlarmCodes at cmsxform.c:113:16 in lcms2.h
// cmsGetAlarmCodes at cmsxform.c:120:16 in lcms2.h
// cmsSetAlarmCodesTHR at cmsxform.c:93:16 in lcms2.h
// cmsGetAlarmCodesTHR at cmsxform.c:104:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_dummy_profile(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 128) return NULL;
    
    // Create a simple sRGB profile for testing
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);
    if (!profile) {
        // Fallback to creating a simple RGB profile
        cmsToneCurve* curves[3];
        for (int i = 0; i < 3; i++) {
            cmsFloat32Number gamma = 2.2f;
            curves[i] = cmsBuildGamma(ctx, gamma);
        }
        
        cmsCIExyY whitePoint = {0.3127f, 0.3290f, 1.0f};
        cmsCIExyYTRIPLE primaries = {
            {0.64f, 0.33f, 1.0f},
            {0.30f, 0.60f, 1.0f},
            {0.15f, 0.06f, 1.0f}
        };
        
        profile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, curves);
        
        for (int i = 0; i < 3; i++) {
            if (curves[i]) cmsFreeToneCurve(curves[i]);
        }
    }
    
    return profile;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create dummy profiles
    cmsHPROFILE input_profile = create_dummy_profile(ctx, Data, Size);
    cmsHPROFILE output_profile = create_dummy_profile(ctx, Data + (Size/2), Size/2);
    cmsHPROFILE proofing_profile = create_dummy_profile(ctx, Data + (Size/3), Size/3);
    
    // Test cmsSetAlarmCodes and cmsGetAlarmCodes
    cmsUInt16Number alarm_codes[cmsMAXCHANNELS];
    cmsUInt16Number new_alarm[cmsMAXCHANNELS];
    
    // Initialize alarm codes from fuzzer data
    for (int i = 0; i < cmsMAXCHANNELS && i * 2 + 1 < Size; i++) {
        alarm_codes[i] = (Data[i * 2] << 8) | Data[i * 2 + 1];
    }
    
    cmsSetAlarmCodes(alarm_codes);
    cmsGetAlarmCodes(new_alarm);
    
    // Test cmsSetAlarmCodesTHR and cmsGetAlarmCodesTHR
    cmsSetAlarmCodesTHR(ctx, alarm_codes);
    cmsGetAlarmCodesTHR(ctx, new_alarm);
    
    // Test cmsCreateTransformTHR
    if (input_profile && output_profile) {
        cmsUInt32Number input_format = (Size > 4) ? *(cmsUInt32Number*)Data : 0;
        cmsUInt32Number output_format = (Size > 8) ? *(cmsUInt32Number*)(Data + 4) : 0;
        cmsUInt32Number intent = (Size > 12) ? *(cmsUInt32Number*)(Data + 8) % 4 : 0;
        cmsUInt32Number flags = (Size > 16) ? *(cmsUInt32Number*)(Data + 12) : 0;
        
        cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, 
            input_profile, input_format,
            output_profile, output_format,
            intent, flags);
        
        if (transform) {
            cmsDeleteTransform(transform);
        }
    }
    
    // Test cmsCreateProofingTransformTHR
    if (input_profile && output_profile && proofing_profile) {
        cmsUInt32Number input_format = (Size > 20) ? *(cmsUInt32Number*)(Data + 16) : 0;
        cmsUInt32Number output_format = (Size > 24) ? *(cmsUInt32Number*)(Data + 20) : 0;
        cmsUInt32Number intent = (Size > 28) ? *(cmsUInt32Number*)(Data + 24) % 4 : 0;
        cmsUInt32Number proofing_intent = (Size > 32) ? *(cmsUInt32Number*)(Data + 28) % 4 : 0;
        cmsUInt32Number flags = (Size > 36) ? *(cmsUInt32Number*)(Data + 32) : 0;
        
        cmsHTRANSFORM proofing_transform = cmsCreateProofingTransformTHR(ctx,
            input_profile, input_format,
            output_profile, output_format,
            proofing_profile,
            intent, proofing_intent, flags);
        
        if (proofing_transform) {
            cmsDeleteTransform(proofing_transform);
        }
    }
    
    // Cleanup
    if (input_profile) cmsCloseProfile(input_profile);
    if (output_profile) cmsCloseProfile(output_profile);
    if (proofing_profile) cmsCloseProfile(proofing_profile);
    
    cmsDeleteContext(ctx);
    
    return 0;
}