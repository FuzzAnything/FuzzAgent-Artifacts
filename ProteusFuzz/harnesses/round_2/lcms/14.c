// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_curves(cmsToneCurve *c1, cmsToneCurve *c2, cmsToneCurve *c3, cmsToneCurve *c4) {
    if (c1) cmsFreeToneCurve(c1);
    if (c2) cmsFreeToneCurve(c2);
    if (c3) cmsFreeToneCurve(c3);
    if (c4) cmsFreeToneCurve(c4);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    // Extract parameters from fuzz input
    uint32_t nEntries = (Data[0] << 8) | Data[1];
    
    // Ensure nEntries is within reasonable bounds to avoid OOM or stack overflow
    if (nEntries == 0 || nEntries > 10000) {
        nEntries = 100; // Default safe value
    }

    // We need at least 2 entries to form a curve, but let's try to build with what we have
    // If Size is too small for the array, we use a minimal fallback
    size_t available_floats = (Size - 2) / sizeof(cmsFloat32Number);
    
    cmsToneCurve *Curve = NULL;
    cmsFloat32Number *fallback_values = NULL;

    if (available_floats < nEntries) {
        // If not enough data, generate a simple linear curve for testing
        fallback_values = (cmsFloat32Number *)malloc(nEntries * sizeof(cmsFloat32Number));
        if (!fallback_values) {
            cmsDeleteContext(ContextID);
            return 0;
        }
        for (uint32_t i = 0; i < nEntries; i++) {
            fallback_values[i] = (float)i / (nEntries > 1 ? nEntries - 1 : 1);
        }
        
        Curve = cmsBuildTabulatedToneCurveFloat(ContextID, nEntries, fallback_values);
        free(fallback_values);
    } else {
        // Parse values from input
        cmsFloat32Number *values = (cmsFloat32Number *)(Data + 2);
        Curve = cmsBuildTabulatedToneCurveFloat(ContextID, nEntries, values);
    }
    
    if (!Curve) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Test Dup
    cmsToneCurve *DupCurve = cmsDupToneCurve(Curve);

    // Test Eval with a value derived from input
    cmsFloat32Number v = 0.5f;
    if (Size > 2) {
        v = (float)(Data[2] & 0xFF) / 255.0f;
    }
    cmsFloat32Number eval_result = cmsEvalToneCurveFloat(Curve, v);
    (void)eval_result; // Suppress unused warning

    // Test Reverse
    cmsToneCurve *RevCurve = cmsReverseToneCurve(Curve);

    // Test ReverseEx
    uint32_t nResultSamples = 10; // Default
    if (Size > 3) {
        nResultSamples = (Data[3] & 0xFF) + 1;
    }
    if (nResultSamples > 10000) nResultSamples = 1000;
    cmsToneCurve *RevExCurve = cmsReverseToneCurveEx(nResultSamples, Curve);

    // Cleanup
    cleanup_curves(Curve, DupCurve, RevCurve, RevExCurve);
    cmsDeleteContext(ContextID);

    return 0;
}