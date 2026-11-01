// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_curves(cmsToneCurve *c1, cmsToneCurve *c2, cmsToneCurve *c3, cmsToneCurve *c4, cmsToneCurve *c5) {
    if (c1) cmsFreeToneCurve(c1);
    if (c2) cmsFreeToneCurve(c2);
    if (c3) cmsFreeToneCurve(c3);
    if (c4) cmsFreeToneCurve(c4);
    if (c5) cmsFreeToneCurve(c5);
}

int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) ctx = cmsCreateContext(NULL, NULL); // Fallback to default if creation fails

    uint32_t nEntries = 0;
    uint32_t nPoints = 0;
    uint32_t nResultSamples = 0;
    float val = 0.0f;

    // Parse input data
    if (Size >= 4) {
        nEntries = *(const uint32_t *)(Data);
        nEntries = (nEntries % 1000) + 1; // Ensure at least 1, max 1000
    }
    if (Size >= 8) {
        nPoints = *(const uint32_t *)(Data + 4);
        nPoints = (nPoints % 1000) + 1;
    }
    if (Size >= 12) {
        nResultSamples = *(const uint32_t *)(Data + 8);
        nResultSamples = (nResultSamples % 1000) + 1;
    }
    if (Size >= 16) {
        val = *(const float *)(Data + 12);
        // Clamp val to 0.0 - 1.0 for safety, though API handles saturation
        if (val < 0.0f) val = 0.0f;
        if (val > 1.0f) val = 1.0f;
    }

    // Prepare values array for cmsBuildTabulatedToneCurveFloat
    cmsFloat32Number *values = NULL;
    if (nEntries > 0) {
        values = (cmsFloat32Number *)malloc(nEntries * sizeof(cmsFloat32Number));
        if (!values) return 0;
        
        // Fill with fuzzed data, clamped to 0-1 range
        size_t data_offset = 16;
        size_t data_remaining = (Size > data_offset) ? (Size - data_offset) : 0;
        
        for (uint32_t i = 0; i < nEntries; i++) {
            if (data_remaining >= 4) {
                float v = *(const float *)(Data + data_offset);
                if (v < 0.0f) v = 0.0f;
                if (v > 1.0f) v = 1.0f;
                values[i] = v;
                data_offset += 4;
                data_remaining -= 4;
            } else {
                // Generate deterministic fallback values if data runs out
                values[i] = (float)(i) / (float)(nEntries);
            }
        }
    }

    cmsToneCurve *curve1 = NULL;
    cmsToneCurve *curve2 = NULL;
    cmsToneCurve *curve3 = NULL;
    cmsToneCurve *curve4 = NULL;
    cmsToneCurve *curve5 = NULL;

    // 1. Build a tabulated curve
    if (values && nEntries > 0) {
        curve1 = cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
    }

    // 2. Duplicate the curve
    if (curve1) {
        curve2 = cmsDupToneCurve(curve1);
    }

    // 3. Reverse the curve
    if (curve1) {
        curve3 = cmsReverseToneCurve(curve1);
    }

    // 4. Reverse the curve with explicit sample count
    if (curve1 && nResultSamples > 0) {
        curve4 = cmsReverseToneCurveEx(nResultSamples, curve1);
    }

    // 5. Join curves (Compose Y o X)
    if (curve1 && curve2 && nPoints > 0) {
        curve5 = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
    }

    // 6. Evaluate curves
    if (curve1) {
        cmsFloat32Number res = cmsEvalToneCurveFloat(curve1, val);
        (void)res; // Use result to avoid unused warning
    }
    if (curve2) {
        cmsFloat32Number res = cmsEvalToneCurveFloat(curve2, val);
        (void)res;
    }
    if (curve3) {
        cmsFloat32Number res = cmsEvalToneCurveFloat(curve3, val);
        (void)res;
    }
    if (curve4) {
        cmsFloat32Number res = cmsEvalToneCurveFloat(curve4, val);
        (void)res;
    }
    if (curve5) {
        cmsFloat32Number res = cmsEvalToneCurveFloat(curve5, val);
        (void)res;
    }

    // Cleanup
    if (values) free(values);
    cleanup_curves(curve1, curve2, curve3, curve4, curve5);
    
    if (ctx) cmsDeleteContext(ctx);

    return 0;
}