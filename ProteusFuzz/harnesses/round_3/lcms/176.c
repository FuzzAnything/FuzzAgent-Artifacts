// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_176(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsCIEXYZ BlackPoint = {0.0, 0.0, 0.0};
    cmsUInt32Number Intent;
    cmsUInt32Number UsedDirection;
    cmsUInt32Number dwFlags;

    // Attempt to open a profile from the input buffer
    hProfile = cmsOpenProfileFromMem(Data, Size);

    if (hProfile == NULL) {
        return 0;
    }

    // Extract rendering intent and model to diversify test states
    cmsUInt32Number renderingIntent = cmsGetHeaderRenderingIntent(hProfile);
    cmsUInt32Number colorModel = cmsGetHeaderModel(hProfile);

    // Use extracted values or fuzzed values for parameters
    Intent = (Size > 0) ? (Data[0] % 4) : renderingIntent;
    UsedDirection = (Size > 1) ? (Data[1] % 2) : 0;
    dwFlags = (Size > 2) ? (Data[2] % 10) : 0;

    // Test cmsIsIntentSupported
    cmsBool isSupported = cmsIsIntentSupported(hProfile, Intent, UsedDirection);
    (void)isSupported; // Suppress unused variable warning

    // Test cmsDetectDestinationBlackPoint
    cmsBool destBlackPointDetected = cmsDetectDestinationBlackPoint(&BlackPoint, hProfile, Intent, dwFlags);
    (void)destBlackPointDetected;

    // Test cmsDetectBlackPoint
    cmsBool blackPointDetected = cmsDetectBlackPoint(&BlackPoint, hProfile, Intent, dwFlags);
    (void)blackPointDetected;

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}