// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetToneCurveSegment at cmsgamma.c:1507:34 in lcms2.h
// cmsGetToneCurveSegment at cmsgamma.c:1507:34 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetToneCurveSegment at cmsgamma.c:1507:34 in lcms2.h
// cmsGetToneCurveSegment at cmsgamma.c:1507:34 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsToneCurve* safe_build_curve_from_data(const uint8_t *data, size_t size) {
    if (size < 2) return NULL;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;

    // Try to build a parametric curve based on available data
    cmsInt32Number type = (cmsInt32Number)data[0];
    cmsFloat64Number params[10] = {0};
    
    // Fill params with data if available, up to 10 doubles (80 bytes)
    size_t param_bytes = size > 81 ? 80 : (size - 1);
    size_t param_count = param_bytes / sizeof(cmsFloat64Number);
    if (param_count > 10) param_count = 10;
    
    if (param_bytes > 0) {
        memcpy(params, data + 1, param_bytes);
    }

    cmsToneCurve *curve = cmsBuildParametricToneCurve(ctx, type, params);
    
    cmsDeleteContext(ctx);
    return curve;
}

// Helper to build a curve using a simple table approach via parametric type if possible,
// or fallback to a valid parametric type since table builders are not in the target list.
// We use TYPE_LINEAR (0) or TYPE_GAMMA (1) as safe defaults if data doesn't parse.
static cmsToneCurve* safe_build_table_curve_from_data(const uint8_t *data, size_t size) {
    if (size < 2) return NULL;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;

    // Since cmsBuildToneCurve16 is not available in the provided API list, 
    // we construct a curve using cmsBuildParametricToneCurve with a known type
    // and parameters derived from the fuzz data to ensure we get a valid curve object.
    // We use TYPE_GAMMA (1) which requires 1 parameter.
    cmsInt32Number type = 1; // TYPE_GAMMA
    cmsFloat64Number params[10] = {0.0};
    
    if (size > 1) {
        // Use the first byte as a seed for the gamma value, ensuring it's non-zero to avoid triviality
        double val = (double)data[0] / 255.0;
        if (val == 0.0) val = 1.0;
        params[0] = val;
    } else {
        params[0] = 1.0;
    }

    cmsToneCurve *curve = cmsBuildParametricToneCurve(ctx, type, params);
    
    cmsDeleteContext(ctx);
    return curve;
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsToneCurve *curve = NULL;

    // Strategy 1: Build parametric curve from raw data
    curve = safe_build_curve_from_data(Data, Size);
    
    if (curve) {
        // Test cmsGetToneCurveSegment
        // Parametric curves usually have 1 segment.
        cmsGetToneCurveSegment(0, curve);
        cmsGetToneCurveSegment(1, curve); // Likely out of bounds, tests error handling

        // Test cmsIsToneCurveMonotonic
        cmsIsToneCurveMonotonic(curve);

        // Test cmsIsToneCurveMultisegment
        cmsIsToneCurveMultisegment(curve);

        // Test cmsGetToneCurveParametricType
        cmsGetToneCurveParametricType(curve);

        // Test cmsSmoothToneCurve
        cmsFloat64Number lambda = (Size > 0) ? (cmsFloat64Number)Data[0] / 255.0 : 0.5;
        if (lambda > 1.0) lambda = 1.0;
        if (lambda < 0.0) lambda = 0.0;
        
        cmsSmoothToneCurve(curve, lambda);
        
        cmsFreeToneCurve(curve);
        curve = NULL;
    }

    // Strategy 2: Build another curve (table-like via parametric) for diverse states
    cmsToneCurve *table_curve = safe_build_table_curve_from_data(Data, Size);
    if (table_curve) {
        // Test cmsGetToneCurveSegment
        cmsGetToneCurveSegment(0, table_curve);
        cmsGetToneCurveSegment(1, table_curve);

        // Test cmsIsToneCurveMonotonic
        cmsIsToneCurveMonotonic(table_curve);

        // Test cmsIsToneCurveMultisegment
        cmsIsToneCurveMultisegment(table_curve);

        // Test cmsGetToneCurveParametricType
        cmsGetToneCurveParametricType(table_curve);

        // Test cmsSmoothToneCurve
        cmsFloat64Number lambda = (Size > 1) ? (cmsFloat64Number)Data[1] / 255.0 : 0.5;
        if (lambda > 1.0) lambda = 1.0;
        if (lambda < 0.0) lambda = 0.0;
        
        cmsSmoothToneCurve(table_curve, lambda);
        
        cmsFreeToneCurve(table_curve);
        table_curve = NULL;
    }

    cmsDeleteContext(ctx);
    return 0;
}