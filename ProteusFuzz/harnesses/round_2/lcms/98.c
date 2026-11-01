// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_curve(cmsToneCurve* curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsToneCurve *curve = NULL;
    cmsBool result;
    cmsInt32Number type;
    cmsFloat64Number params[10];
    cmsFloat64Number lambda;

    // Extract parameters from input data
    type = Data[0];
    if (Size > 1) {
        lambda = (cmsFloat64Number)(Data[1]) / 255.0;
    } else {
        lambda = 0.0;
    }

    // Initialize params array with fuzzed data
    memset(params, 0, sizeof(params));
    size_t param_count = (Size > 2) ? (Size - 2) / sizeof(cmsFloat64Number) : 0;
    if (param_count > 10) {
        param_count = 10;
    }
    for (size_t i = 0; i < param_count; i++) {
        if (Size >= 2 + (i + 1) * sizeof(cmsFloat64Number)) {
            memcpy(&params[i], Data + 2 + i * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
        }
    }

    // Try to build a parametric tone curve
    // Valid types are typically 0 to 4 (e.g., CMS_PARAMETRIC_CURVE_TYPE_1 to 4)
    if (type >= 0 && type <= 4) { 
        curve = cmsBuildParametricToneCurve(ctx, type, params);
    }

    if (curve != NULL) {
        // Test cmsIsToneCurveMonotonic
        result = cmsIsToneCurveMonotonic(curve);
        (void)result;

        // Test cmsIsToneCurveLinear
        result = cmsIsToneCurveLinear(curve);
        (void)result;

        // Test cmsIsToneCurveDescending
        result = cmsIsToneCurveDescending(curve);
        (void)result;

        // Test cmsIsToneCurveMultisegment
        result = cmsIsToneCurveMultisegment(curve);
        (void)result;

        // Test cmsSmoothToneCurve (only if curve is not linear)
        if (!cmsIsToneCurveLinear(curve)) {
            cmsToneCurve *smooth_curve = cmsDupToneCurve(curve);
            if (smooth_curve != NULL) {
                result = cmsSmoothToneCurve(smooth_curve, lambda);
                cleanup_curve(smooth_curve);
            }
        }

        cleanup_curve(curve);
    }

    cmsDeleteContext(ctx);
    return 0;
}