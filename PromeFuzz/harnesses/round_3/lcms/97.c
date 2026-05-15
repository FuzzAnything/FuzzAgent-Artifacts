// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreate_OkLabProfile at cmsvirt.c:690:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_test_tone_curve(cmsContext ctx) {
    // Create a simple gamma curve for testing
    cmsFloat64Number gamma = 2.2;
    return cmsBuildParametricToneCurve(ctx, 1, &gamma);
}

static void cleanup_tone_curves(cmsToneCurve* curves[3]) {
    for (int i = 0; i < 3; i++) {
        if (curves[i]) {
            cmsFreeToneCurve(curves[i]);
        }
    }
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIExyY) * 4) {
        // Not enough data for meaningful fuzzing
        return 0;
    }

    // Create a context for THR functions
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Extract white point from fuzzer data
    cmsCIExyY whitePoint;
    size_t offset = 0;
    memcpy(&whitePoint, Data + offset, sizeof(cmsCIExyY));
    offset += sizeof(cmsCIExyY);

    // Extract primaries for RGB profile
    cmsCIExyYTRIPLE primaries;
    memcpy(&primaries, Data + offset, sizeof(cmsCIExyYTRIPLE));
    offset += sizeof(cmsCIExyYTRIPLE);

    // Create tone curves for RGB profile
    cmsToneCurve* transferFunction[3] = {NULL, NULL, NULL};
    for (int i = 0; i < 3; i++) {
        transferFunction[i] = create_test_tone_curve(ctx);
        if (!transferFunction[i]) {
            cleanup_tone_curves(transferFunction);
            cmsDeleteContext(ctx);
            return 0;
        }
    }

    // Test cmsCreateLab2Profile
    cmsHPROFILE lab2Profile = cmsCreateLab2Profile(&whitePoint);
    if (lab2Profile) {
        cmsCloseProfile(lab2Profile);
    }

    // Test cmsCreateRGBProfileTHR
    cmsHPROFILE rgbProfile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, transferFunction);
    if (rgbProfile) {
        cmsCloseProfile(rgbProfile);
    }

    // Test cmsD50_xyY
    const cmsCIExyY* d50 = cmsD50_xyY();
    (void)d50; // Use to avoid unused variable warning

    // Test cmsCreate_OkLabProfile
    cmsHPROFILE okLabProfile = cmsCreate_OkLabProfile(ctx);
    if (okLabProfile) {
        cmsCloseProfile(okLabProfile);
    }

    // Test cmsCreateLab4Profile
    cmsHPROFILE lab4Profile = cmsCreateLab4Profile(&whitePoint);
    if (lab4Profile) {
        cmsCloseProfile(lab4Profile);
    }

    // Test cmsCreateLab4ProfileTHR
    cmsHPROFILE lab4ProfileTHR = cmsCreateLab4ProfileTHR(ctx, &whitePoint);
    if (lab4ProfileTHR) {
        cmsCloseProfile(lab4ProfileTHR);
    }

    // Cleanup
    cleanup_tone_curves(transferFunction);
    cmsDeleteContext(ctx);

    return 0;
}