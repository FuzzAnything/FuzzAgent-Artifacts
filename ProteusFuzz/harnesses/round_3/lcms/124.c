// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
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
#include <math.h>
#include <lcms2.h>

static void cleanup_curve(cmsToneCurve *curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

static cmsToneCurve* create_random_curve(cmsContext ctx, const uint8_t *data, size_t size) {
    if (size < 4) return NULL;
    
    cmsUInt32Number nEntries = *(const uint32_t*)data;
    if (nEntries == 0) nEntries = 1;
    if (nEntries > 10000) nEntries = 10000;
    
    cmsFloat32Number *values = (cmsFloat32Number*)malloc(nEntries * sizeof(cmsFloat32Number));
    if (!values) return NULL;
    
    size_t offset = 4;
    for (uint32_t i = 0; i < nEntries; i++) {
        if (offset + sizeof(float) <= size) {
            values[i] = *(const float*)(data + offset);
            offset += sizeof(float);
        } else {
            // Generate pseudo-random value based on seed if data runs out
            values[i] = (float)(i / (float)nEntries);
        }
    }
    
    cmsToneCurve *curve = cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
    free(values);
    return curve;
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;
    
    cmsToneCurve *curve1 = NULL;
    cmsToneCurve *curve2 = NULL;
    cmsToneCurve *curve3 = NULL;
    cmsToneCurve *result = NULL;
    
    // Create first curve
    curve1 = create_random_curve(ctx, Data, Size);
    if (curve1 == NULL) {
        // Try with minimal data if creation failed
        cmsFloat32Number simple_vals[] = {0.0f, 0.5f, 1.0f};
        curve1 = cmsBuildTabulatedToneCurveFloat(ctx, 3, simple_vals);
    }
    
    // Create second curve if we have enough data
    if (Size > 4) {
        curve2 = create_random_curve(ctx, Data + 4, Size - 4);
        if (curve2 == NULL) {
            cmsFloat32Number simple_vals[] = {1.0f, 0.5f, 0.0f};
            curve2 = cmsBuildTabulatedToneCurveFloat(ctx, 3, simple_vals);
        }
    }
    
    // Test cmsReverseToneCurve
    if (curve1 != NULL) {
        result = cmsReverseToneCurve(curve1);
        cleanup_curve(result);
        result = NULL;
    }
    
    // Test cmsReverseToneCurveEx
    if (curve1 != NULL) {
        cmsUInt32Number samples = 1;
        if (Size > 0) samples = Data[0] % 4096;
        if (samples == 0) samples = 1;
        
        result = cmsReverseToneCurveEx(samples, curve1);
        cleanup_curve(result);
        result = NULL;
    }
    
    // Test cmsEvalToneCurveFloat
    if (curve1 != NULL) {
        cmsFloat32Number v = 0.0f;
        if (Size > 1) {
            v = (float)(Data[1] / 255.0f);
        }
        // Clamp to [0, 1]
        if (v < 0.0f) v = 0.0f;
        if (v > 1.0f) v = 1.0f;
        
        cmsFloat32Number eval_result = cmsEvalToneCurveFloat(curve1, v);
        (void)eval_result; // Use result to avoid unused variable warning
    }
    
    // Test cmsJoinToneCurve
    if (curve1 != NULL && curve2 != NULL) {
        cmsUInt32Number nPoints = 1;
        if (Size > 2) {
            nPoints = (Data[2] * 256 + Data[3]) % 4096;
        }
        if (nPoints == 0) nPoints = 1;
        
        result = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
        cleanup_curve(result);
        result = NULL;
    }
    
    // Test cmsDupToneCurve
    if (curve1 != NULL) {
        result = cmsDupToneCurve(curve1);
        cleanup_curve(result);
        result = NULL;
    }
    
    // Cleanup
    cleanup_curve(curve1);
    cleanup_curve(curve2);
    cleanup_curve(curve3);
    cleanup_curve(result);
    
    cmsDeleteContext(ctx);
    
    return 0;
}