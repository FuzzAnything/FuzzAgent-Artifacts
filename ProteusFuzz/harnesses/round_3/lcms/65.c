// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "lcms2.h"

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

static cmsToneCurve* create_dummy_curve(cmsContext ctxID, const uint8_t* data, size_t size) {
    if (ctxID == NULL) ctxID = cmsCreateContext(NULL, NULL);
    if (ctxID == NULL) return NULL;

    // Create a simple identity curve or a curve based on input data if possible
    // For fuzzing, we often just need a valid structure.
    // cmsBuildGamma is a safe way to create a curve.
    cmsToneCurve* curve = cmsBuildGamma(ctxID, 1.0);
    if (curve == NULL) {
        cmsDeleteContext(ctxID);
        return NULL;
    }
    
    // If we want to vary the curve based on input, we could try to parse data,
    // but for robustness, a simple valid curve is often enough to test the profile creation logic.
    // To add diversity, we can try to build a curve with a gamma derived from input if size > 0.
    if (size > 0) {
        double gamma = 1.0 + (double)(data[0] % 100) / 100.0; // 1.0 to 1.99
        cmsToneCurve* newCurve = cmsBuildGamma(ctxID, gamma);
        if (newCurve != NULL) {
            cmsFreeToneCurve(curve);
            curve = newCurve;
        }
    }
    
    return curve;
}

static void cleanup_curve(cmsToneCurve* curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctxID = cmsCreateContext(NULL, NULL);
    if (ctxID == NULL) return 0;

    cmsCIExyY whitePoint = {0.0, 0.0, 1.0};
    cmsFloat64Number tempK = 0.0;
    cmsBool success = FALSE;
    cmsHPROFILE profile = NULL;
    cmsToneCurve* curve = NULL;

    // 1. Test cmsWhitePointFromTemp
    // Extract a temperature value from input (4000K - 25000K)
    if (Size >= 2) {
        uint16_t raw_temp = (Data[0] << 8) | Data[1];
        // Map to range 4000-25000
        uint16_t temp_range = 25000 - 4000;
        uint16_t mapped_temp = 4000 + (raw_temp % (temp_range + 1));
        success = cmsWhitePointFromTemp(&whitePoint, (cmsFloat64Number)mapped_temp);
        if (success) {
            // 2. Test cmsTempFromWhitePoint
            cmsTempFromWhitePoint(&tempK, &whitePoint);
        }
    } else {
        // Fallback to D50 if not enough data for temp calculation
        const cmsCIExyY* d50 = cmsD50_xyY();
        if (d50 != NULL) {
            whitePoint = *d50;
            cmsTempFromWhitePoint(&tempK, d50);
        }
    }

    // 3. Test cmsCreateLab2Profile
    profile = cmsCreateLab2Profile(&whitePoint);
    cleanup_profile(profile);

    // 4. Test cmsCreateLab4Profile
    profile = cmsCreateLab4Profile(&whitePoint);
    cleanup_profile(profile);

    // 5. Test cmsCreateGrayProfile
    // We need a valid tone curve.
    curve = create_dummy_curve(ctxID, Data, Size);
    if (curve != NULL) {
        profile = cmsCreateGrayProfile(&whitePoint, curve);
        cleanup_profile(profile);
        cleanup_curve(curve);
    }

    // Additional exploration: Try with D50 explicitly
    const cmsCIExyY* d50_ptr = cmsD50_xyY();
    if (d50_ptr != NULL) {
        profile = cmsCreateLab2Profile(d50_ptr);
        cleanup_profile(profile);

        profile = cmsCreateLab4Profile(d50_ptr);
        cleanup_profile(profile);

        curve = create_dummy_curve(ctxID, Data, Size);
        if (curve != NULL) {
            profile = cmsCreateGrayProfile(d50_ptr, curve);
            cleanup_profile(profile);
            cleanup_curve(curve);
        }
    }

    // Cleanup context
    cmsDeleteContext(ctxID);

    return 0;
}