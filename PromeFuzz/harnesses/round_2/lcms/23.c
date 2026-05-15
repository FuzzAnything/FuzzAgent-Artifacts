// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsHPROFILE create_test_profile(const uint8_t *data, size_t size) {
    // Create a placeholder profile that we can modify
    cmsHPROFILE profile = cmsCreateProfilePlaceholder(NULL);
    
    if (!profile) {
        // Fallback: create a simple sRGB profile if placeholder fails
        profile = cmsCreate_sRGBProfile();
    }
    
    return profile;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    // Create a test profile using the fuzzer input
    cmsHPROFILE hProfile = create_test_profile(Data, Size);
    
    if (!hProfile) {
        return 0;
    }
    
    // Use fuzzer data to generate values for the API calls
    // We'll use the first bytes of input for parameter values
    
    // 1. Test cmsGetHeaderRenderingIntent
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(hProfile);
    (void)intent; // Use result to avoid compiler warnings
    
    // 2. Test cmsSetDeviceClass and cmsGetDeviceClass
    if (Size >= 4) {
        cmsProfileClassSignature sig = *(cmsProfileClassSignature*)Data;
        cmsSetDeviceClass(hProfile, sig);
        
        cmsProfileClassSignature retrieved_sig = cmsGetDeviceClass(hProfile);
        (void)retrieved_sig;
    }
    
    // 3. Test cmsSetHeaderModel and cmsGetHeaderModel
    if (Size >= 8) {
        cmsUInt32Number model = *(cmsUInt32Number*)(Data + 4);
        cmsSetHeaderModel(hProfile, model);
        
        cmsUInt32Number retrieved_model = cmsGetHeaderModel(hProfile);
        (void)retrieved_model;
    }
    
    // 4. Test cmsSetEncodedICCversion
    if (Size >= 12) {
        cmsUInt32Number version = *(cmsUInt32Number*)(Data + 8);
        cmsSetEncodedICCversion(hProfile, version);
    }
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}