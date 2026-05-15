// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Create dummy pipelines using library functions
    cmsPipeline* pipelines[12];
    
    // Create 12 pipelines using cmsPipelineAlloc
    for (int i = 0; i < 12; i++) {
        // Use varying channel counts based on fuzzer input
        cmsUInt32Number inputChannels = 3;
        cmsUInt32Number outputChannels = 3;
        
        if (Size > i * 2) {
            inputChannels = (Data[i * 2] % 8) + 1;  // 1-8 channels
            if (Size > i * 2 + 1) {
                outputChannels = (Data[i * 2 + 1] % 8) + 1;  // 1-8 channels
            }
        }
        
        pipelines[i] = cmsPipelineAlloc(NULL, inputChannels, outputChannels);
    }
    
    // Call cmsPipelineFree 12 times as specified
    for (int i = 0; i < 12; i++) {
        cmsPipelineFree(pipelines[i]);
    }
    
    // Create dummy profiles for black point detection
    cmsHPROFILE profiles[4] = {NULL, NULL, NULL, NULL};
    
    // Create some simple profiles
    // 1. sRGB profile
    profiles[0] = cmsCreate_sRGBProfile();
    
    // 2. Lab profile
    profiles[1] = cmsCreateLab4Profile(NULL);
    
    // 3. Gray profile with gamma 2.2
    cmsToneCurve* gamma22 = cmsBuildGamma(NULL, 2.2);
    if (gamma22) {
        profiles[2] = cmsCreateGrayProfile(NULL, gamma22);
        cmsFreeToneCurve(gamma22);
    }
    
    // 4. Another sRGB profile
    profiles[3] = cmsCreate_sRGBProfile();
    
    // Create dummy CIEXYZ structures for black points
    cmsCIEXYZ blackPoints[4];
    
    // Call cmsDetectDestinationBlackPoint 4 times
    for (int i = 0; i < 4; i++) {
        // Initialize black point
        blackPoints[i].X = 0.0;
        blackPoints[i].Y = 0.0;
        blackPoints[i].Z = 0.0;
        
        // Use fuzzer data to determine intent and flags
        cmsUInt32Number intent = 0;
        cmsUInt32Number flags = 0;
        
        if (Size > i * 2) {
            intent = Data[i * 2] % 4;  // 0-3 for standard intents
        }
        if (Size > i * 2 + 1) {
            flags = Data[i * 2 + 1];
        }
        
        // Only call if we have a valid profile
        if (profiles[i]) {
            cmsDetectDestinationBlackPoint(&blackPoints[i], profiles[i], intent, flags);
        }
    }
    
    // Call cmsDetectTAC on one of the profiles
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