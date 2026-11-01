// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_resources(cmsContext ctx, cmsToneCurve *curve1, cmsToneCurve *curve2, cmsHPROFILE profile) {
    if (curve1 != NULL) {
        cmsFreeToneCurve(curve1);
    }
    if (curve2 != NULL) {
        cmsFreeToneCurve(curve2);
    }
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Extract parameters from fuzz input
    cmsFloat64Number gamma_val = 1.0;
    cmsFloat64Number threshold_val = 0.01;
    cmsFloat64Number precision_val = 0.01;
    cmsFloat64Number lambda_val = 0.5;

    // Safe extraction with bounds checking to avoid NaN/Inf
    if (Size >= 8) {
        double d = *(double*)(Data);
        if (d > 0.0 && d < 100.0) gamma_val = d;
    }

    if (Size >= 16) {
        double d = *(double*)(Data + 8);
        if (d > 0.0 && d < 100.0) threshold_val = d;
    }

    if (Size >= 24) {
        double d = *(double*)(Data + 16);
        if (d > 0.0 && d < 100.0) precision_val = d;
    }

    if (Size >= 32) {
        double d = *(double*)(Data + 24);
        if (d > 0.0 && d < 100.0) lambda_val = d;
    }

    cmsContext ctx = NULL;
    cmsToneCurve *curve1 = NULL;
    cmsToneCurve *curve2 = NULL;
    cmsHPROFILE profile = NULL;
    cmsBool smooth_result = FALSE;
    cmsInt32Number param_type = 0;
    cmsFloat64Number est_gamma = -1.0;
    cmsFloat64Number detect_gamma = -1.0;

    // Initialize context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // 1. Test cmsBuildGamma
    curve1 = cmsBuildGamma(ctx, gamma_val);
    if (curve1 != NULL) {
        // 4. Test cmsEstimateGamma
        est_gamma = cmsEstimateGamma(curve1, precision_val);
        
        // 5. Test cmsSmoothToneCurve
        smooth_result = cmsSmoothToneCurve(curve1, lambda_val);

        // 6. Test cmsGetToneCurveParametricType
        param_type = cmsGetToneCurveParametricType(curve1);

        // 3. Test cmsDupToneCurve
        curve2 = cmsDupToneCurve(curve1);
        if (curve2 != NULL) {
            // Test operations on duplicate
            est_gamma = cmsEstimateGamma(curve2, precision_val);
            smooth_result = cmsSmoothToneCurve(curve2, lambda_val);
            param_type = cmsGetToneCurveParametricType(curve2);
        }
    }

    // 2. Test cmsDetectRGBProfileGamma
    // Prepare data for cmsCreateRGBProfile
    cmsCIExyY white_point = { 0.3127, 0.3290, 1.0 };
    cmsCIExyYTRIPLE primaries = {
        { 0.64, 0.33, 1.0 }, // Red
        { 0.30, 0.60, 1.0 }, // Green
        { 0.15, 0.06, 1.0 }  // Blue
    };
    cmsToneCurve* transfer[3] = { NULL, NULL, NULL };

    // Create a simple gamma curve for the profile if needed, or use NULL for linear
    // Using NULL for linear transfer functions to simplify
    profile = cmsCreateRGBProfile(&white_point, &primaries, transfer);
    
    if (profile != NULL) {
        detect_gamma = cmsDetectRGBProfileGamma(profile, threshold_val);
    }

    // Cleanup
    cleanup_resources(ctx, curve1, curve2, profile);

    return 0;
}