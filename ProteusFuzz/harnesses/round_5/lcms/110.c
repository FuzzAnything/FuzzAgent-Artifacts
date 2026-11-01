// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
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

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsToneCurve* curve = NULL;
    cmsBool result;

    // Extract parameters from input data
    cmsInt32Number type = (cmsInt32Number)(Data[0] & 0x0F); // Limit to 0-15
    cmsFloat64Number lambda = (cmsFloat64Number)(Data[1] / 255.0) * 10.0;

    // Prepare parameters for parametric curve
    cmsFloat64Number params[10] = {0};
    for (int i = 0; i < 10 && i < Size - 2; i++) {
        params[i] = (cmsFloat64Number)(Data[2 + i]) / 255.0;
    }

    // Try to build a parametric tone curve
    curve = cmsBuildParametricToneCurve(ctx, type, params);

    if (curve != NULL) {
        // Test cmsIsToneCurveLinear
        result = cmsIsToneCurveLinear(curve);
        (void)result;

        // Test cmsIsToneCurveMultisegment
        result = cmsIsToneCurveMultisegment(curve);
        (void)result;

        // Test cmsIsToneCurveDescending
        result = cmsIsToneCurveDescending(curve);
        (void)result;

        // Test cmsIsToneCurveMonotonic
        result = cmsIsToneCurveMonotonic(curve);
        (void)result;

        // Test cmsSmoothToneCurve (only if curve is not linear)
        if (!cmsIsToneCurveLinear(curve)) {
            cmsSmoothToneCurve(curve, lambda);
        }

        cleanup_curve(curve);
    }

    // Additional test: create a simple identity curve using cmsBuildGamma
    // This is a standard way to create a curve in lcms2 without needing internal APIs
    curve = cmsBuildGamma(ctx, 1.0);
    if (curve != NULL) {
        // We cannot easily modify the internal table of a gamma curve directly via public API
        // without complex reconstruction, so we just test the properties of the generated curve.
        
        result = cmsIsToneCurveLinear(curve);
        (void)result;

        result = cmsIsToneCurveMultisegment(curve);
        (void)result;

        result = cmsIsToneCurveDescending(curve);
        (void)result;

        result = cmsIsToneCurveMonotonic(curve);
        (void)result;

        // Gamma 1.0 is linear, so smoothing might be skipped or do nothing, 
        // but we call it to test the logic path.
        cmsSmoothToneCurve(curve, lambda);

        cleanup_curve(curve);
    }

    // Test with another gamma value to ensure non-linear paths are hit
    cmsFloat64Number gammaVal = (cmsFloat64Number)(Data[0]) / 128.0; // Range 0 to ~2
    if (gammaVal > 0.0) {
        curve = cmsBuildGamma(ctx, gammaVal);
        if (curve != NULL) {
            result = cmsIsToneCurveLinear(curve);
            (void)result;

            result = cmsIsToneCurveMultisegment(curve);
            (void)result;

            result = cmsIsToneCurveDescending(curve);
            (void)result;

            result = cmsIsToneCurveMonotonic(curve);
            (void)result;

            if (!cmsIsToneCurveLinear(curve)) {
                cmsSmoothToneCurve(curve, lambda);
            }

            cleanup_curve(curve);
        }
    }

    cmsDeleteContext(ctx);
    return 0;
}