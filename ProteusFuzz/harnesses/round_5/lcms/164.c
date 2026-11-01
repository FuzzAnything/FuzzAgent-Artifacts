// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsCIEXYZ BlackPoint = {0.0, 0.0, 0.0};
    cmsUInt32Number Intent = 0;
    cmsUInt32Number dwFlags = 0;
    cmsUInt32Number UsedDirection = 0;

    // Attempt to open a profile from the fuzzed data
    hProfile = cmsOpenProfileFromMem((const void*)Data, Size);

    if (hProfile != NULL) {
        // Test cmsGetHeaderModel
        cmsUInt32Number model = cmsGetHeaderModel(hProfile);
        (void)model; // Suppress unused variable warning

        // Test cmsSetHeaderRenderingIntent with various intents (0-3)
        for (int i = 0; i < 4; i++) {
            Intent = (cmsUInt32Number)i;
            cmsSetHeaderRenderingIntent(hProfile, Intent);
        }

        // Test cmsIsIntentSupported with various intents and directions
        for (int i = 0; i < 4; i++) {
            Intent = (cmsUInt32Number)i;
            cmsBool supported = cmsIsIntentSupported(hProfile, Intent, UsedDirection);
            (void)supported;
        }

        // Test cmsDetectBlackPoint with various intents
        for (int i = 0; i < 4; i++) {
            Intent = (cmsUInt32Number)i;
            cmsBool detected = cmsDetectBlackPoint(&BlackPoint, hProfile, Intent, dwFlags);
            (void)detected;
        }

        // Test cmsDetectDestinationBlackPoint with various intents
        for (int i = 0; i < 4; i++) {
            Intent = (cmsUInt32Number)i;
            cmsBool destDetected = cmsDetectDestinationBlackPoint(&BlackPoint, hProfile, Intent, dwFlags);
            (void)destDetected;
        }

        // Cleanup
        cleanup_profile(hProfile);
    }

    return 0;
}