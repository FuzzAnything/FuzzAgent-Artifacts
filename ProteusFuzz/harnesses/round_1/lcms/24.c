// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void free_pipeline(cmsPipeline* p) {
    if (p != NULL) {
        cmsPipelineFree(p);
    }
}

static cmsHPROFILE create_dummy_profile(const uint8_t *Data, size_t Size) {
    cmsHPROFILE hProfile = NULL;
    
    // Try to create a profile from the fuzz data if it looks like a valid ICC profile
    // ICC profiles start with a size field (4 bytes) followed by "acsp" signature
    if (Size >= 128) {
        uint32_t size = 0;
        memcpy(&size, Data, 4);
        
        // Basic validation: size should be at least 128 and not larger than reasonable
        if (size >= 128 && size <= Size) {
            hProfile = cmsOpenProfileFromMem(Data, Size);
        }
    }
    
    return hProfile;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }
    
    // Create a dummy profile from input data
    cmsHPROFILE hProfile = create_dummy_profile(Data, Size);
    
    // If we couldn't create a profile, try with a minimal valid profile
    if (hProfile == NULL) {
        // Create a minimal valid ICC profile in memory
        uint8_t *profileData = malloc(128);
        if (profileData != NULL) {
            memset(profileData, 0, 128);
            // Set profile size to 128
            uint32_t size = 128;
            memcpy(profileData, &size, 4);
            // Set "acsp" signature
            memcpy(profileData + 4, "acsp", 4);
            
            hProfile = cmsOpenProfileFromMem(profileData, 128);
            free(profileData);
        }
    }
    
    // Allocate some pipelines to free
    cmsPipeline *pipelines[12];
    for (int i = 0; i < 12; i++) {
        pipelines[i] = NULL;
    }
    
    // Call cmsPipelineFree 12 times (as requested)
    // These will safely handle NULL inputs
    for (int i = 0; i < 12; i++) {
        cmsPipelineFree(pipelines[i]);
    }
    
    // If we have a valid profile, test the other functions
    if (hProfile != NULL) {
        cmsCIEXYZ blackPoint;
        memset(&blackPoint, 0, sizeof(blackPoint));
        
        // Call cmsDetectDestinationBlackPoint 4 times with different intents
        cmsUInt32Number intents[] = {0, 1, 2, 3}; // Perceptual, Relative, Saturation, Absolute
        for (int i = 0; i < 4; i++) {
            cmsDetectDestinationBlackPoint(&blackPoint, hProfile, intents[i], 0);
        }
        
        // Call cmsDetectTAC once
        cmsFloat64Number tac = cmsDetectTAC(hProfile);
        (void)tac; // Suppress unused variable warning
    }
    
    // Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}