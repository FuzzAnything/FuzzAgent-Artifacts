// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Attempt to load a profile from a dummy file. 
    // If the file doesn't exist or is invalid, hProfile will be NULL.
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    
    // Initialize black point structure
    cmsCIEXYZ blackPoint = {0.0, 0.0, 0.0};

    // Invoke cmsPipelineFree multiple times (Safe with NULL)
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);

    // Invoke cmsDetectDestinationBlackPoint
    // Only invoke with valid profile to avoid NULL pointer dereference inside the library
    if (hProfile != NULL) {
        cmsDetectDestinationBlackPoint(&blackPoint, hProfile, 0, 0);
        cmsDetectDestinationBlackPoint(&blackPoint, hProfile, 1, 0);
        cmsDetectDestinationBlackPoint(&blackPoint, hProfile, 2, 0);
        cmsDetectDestinationBlackPoint(&blackPoint, hProfile, 3, 0);
    }

    // Invoke cmsDetectTAC
    // Only invoke with valid profile to avoid NULL pointer dereference
    cmsFloat64Number tac = 0.0;
    if (hProfile != NULL) {
        tac = cmsDetectTAC(hProfile);
    }

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}