// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsHPROFILE create_dummy_profile(void) {
    // Create a minimal valid sRGB profile for testing
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile) {
        return hProfile;
    }
    
    // Fallback: create a simple RGB profile if sRGB creation fails
    cmsToneCurve* gamma = cmsBuildGamma(NULL, 2.2);
    if (!gamma) {
        return NULL;
    }
    
    cmsToneCurve* gammaCurves[3] = {gamma, gamma, gamma};
    hProfile = cmsCreateRGBProfile(cmsD50_xyY(), NULL, gammaCurves);
    cmsFreeToneCurve(gamma);
    
    return hProfile;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Call cmsPipelineFree 12 times with NULL (safe according to documentation)
    for (int i = 0; i < 12; i++) {
        cmsPipelineFree(NULL);
    }
    
    // Create dummy profiles for testing detection functions
    cmsHPROFILE profiles[4] = {NULL, NULL, NULL, NULL};
    cmsCIEXYZ black_points[4];
    
    // Create valid profiles for testing
    for (int i = 0; i < 4; i++) {
        profiles[i] = create_dummy_profile();
    }
    
    // Initialize black points
    for (int i = 0; i < 4; i++) {
        memset(&black_points[i], 0, sizeof(cmsCIEXYZ));
    }
    
    // Call cmsDetectDestinationBlackPoint 4 times
    for (int i = 0; i < 4; i++) {
        if (profiles[i]) {
            cmsUInt32Number intent = 0;
            cmsUInt32Number flags = 0;
            
            // Use fuzzer data to vary intent and flags if available
            if (Size >= 8) {
                // Use different parts of the fuzzer data for different calls
                size_t data_offset = (i * 2) % (Size - 3);
                if (data_offset + 3 < Size) {
                    intent = Data[data_offset] % 4;  // Intent values are 0-3
                    flags = Data[data_offset + 1] % 256;
                }
            }
            
            cmsDetectDestinationBlackPoint(&black_points[i], profiles[i], intent, flags);
        }
    }
    
    // Call cmsDetectTAC
    cmsFloat64Number tac = 0.0;
    if (profiles[0]) {
        tac = cmsDetectTAC(profiles[0]);
    }
    
    // Clean up profiles
    for (int i = 0; i < 4; i++) {
        if (profiles[i]) {
            cmsCloseProfile(profiles[i]);
        }
    }
    
    return 0;
}