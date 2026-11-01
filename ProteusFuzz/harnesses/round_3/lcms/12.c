// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
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

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsHPROFILE hProfile = NULL;
    cmsCIEXYZ BlackPoint = {0.0, 0.0, 0.0};
    cmsFloat64Number version;
    cmsBool result;
    cmsFloat64Number tac;
    cmsUInt32Number intent;

    // Attempt to open a profile from the input buffer
    hProfile = cmsOpenProfileFromMem(Data, Size);

    if (hProfile != NULL) {
        // 1. cmsGetProfileVersion
        version = cmsGetProfileVersion(hProfile);

        // 2. cmsDetectDestinationBlackPoint
        // Try different intents (0-3) and flags (0, 1)
        for (intent = 0; intent <= 3; intent++) {
            for (uint32_t flags = 0; flags <= 1; flags++) {
                result = cmsDetectDestinationBlackPoint(&BlackPoint, hProfile, intent, flags);
                (void)result; // Suppress unused variable warning
            }
        }

        // 3. cmsSetHeaderRenderingIntent
        // Try setting different intents (0-3)
        for (intent = 0; intent <= 3; intent++) {
            cmsSetHeaderRenderingIntent(hProfile, intent);
        }

        // 4. cmsSetProfileVersion
        // Try setting various versions derived from input or fixed values
        cmsSetProfileVersion(hProfile, 2.0);
        cmsSetProfileVersion(hProfile, 4.0);
        cmsSetProfileVersion(hProfile, 4.2);
        if (Size > 0) {
            // Use input data to generate a version if possible
            cmsSetProfileVersion(hProfile, (cmsFloat64Number)(Data[0] / 10.0));
        }

        // 5. cmsDetectTAC
        tac = cmsDetectTAC(hProfile);
        (void)tac; // Suppress unused variable warning

        // Cleanup
        cleanup_profile(hProfile);
    }

    // Avoid calling functions with NULL profile that cause dereference errors.
    // cmsGetProfileVersion, cmsDetectDestinationBlackPoint, and cmsDetectTAC
    // are documented to handle NULL gracefully or return error values, but
    // the sanitizer indicates internal dereference in cmsGetProfileVersion.
    // Therefore, we skip explicit NULL testing for these functions to prevent crashes.

    return 0;
}