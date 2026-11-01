// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
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

static void cleanup_curve(cmsToneCurve* curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

static cmsToneCurve* create_test_curve_from_data(const uint8_t *Data, size_t Size) {
    if (Size < 2) return NULL;
    
    // Use first two bytes to determine number of entries (min 2, max 1000)
    uint32_t nEntries = (Data[0] % 999) + 2;
    if (nEntries > Size) nEntries = Size;
    
    // Allocate values array
    cmsFloat32Number *values = (cmsFloat32Number*)malloc(nEntries * sizeof(cmsFloat32Number));
    if (values == NULL) return NULL;
    
    // Generate values from input data, ensuring they are in 0.0-1.0 range
    for (uint32_t i = 0; i < nEntries; i++) {
        if (i < Size) {
            values[i] = (float)(Data[i] & 0xFF) / 255.0f;
        } else {
            values[i] = (float)(i % 256) / 255.0f;
        }
    }
    
    cmsToneCurve *curve = cmsBuildTabulatedToneCurveFloat(NULL, nEntries, values);
    free(values);
    return curve;
}

int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsToneCurve *curve1 = NULL;
    cmsToneCurve *curve2 = NULL;
    cmsToneCurve *revCurve = NULL;
    cmsToneCurve *joinedCurve = NULL;
    cmsToneCurve *dupCurve = NULL;
    cmsToneCurve *revExCurve = NULL;
    
    // Create first curve from input data
    curve1 = create_test_curve_from_data(Data, Size);
    
    // If we couldn't create a curve, try with minimal data
    if (curve1 == NULL) {
        cmsFloat32Number simpleValues[] = {0.0f, 0.5f, 1.0f};
        curve1 = cmsBuildTabulatedToneCurveFloat(NULL, 3, simpleValues);
    }
    
    if (curve1 == NULL) return 0;
    
    // Test cmsEvalToneCurveFloat with various inputs
    if (Size > 0) {
        float v = (float)(Data[0] & 0xFF) / 255.0f;
        // Test normal range
        cmsEvalToneCurveFloat(curve1, v);
        // Test out of range (should saturate)
        cmsEvalToneCurveFloat(curve1, -0.5f);
        cmsEvalToneCurveFloat(curve1, 1.5f);
        cmsEvalToneCurveFloat(curve1, 0.0f);
        cmsEvalToneCurveFloat(curve1, 1.0f);
    }
    
    // Test cmsDupToneCurve
    dupCurve = cmsDupToneCurve(curve1);
    if (dupCurve != NULL) {
        if (Size > 0) {
            float v = (float)(Data[0] & 0xFF) / 255.0f;
            cmsEvalToneCurveFloat(dupCurve, v);
        }
        cleanup_curve(dupCurve);
    }
    
    // Test cmsReverseToneCurve
    revCurve = cmsReverseToneCurve(curve1);
    if (revCurve != NULL) {
        if (Size > 0) {
            float v = (float)(Data[0] & 0xFF) / 255.0f;
            cmsEvalToneCurveFloat(revCurve, v);
        }
        cleanup_curve(revCurve);
    }
    
    // Test cmsReverseToneCurveEx
    uint32_t nSamples = (Size > 0) ? (Data[0] % 1000) + 16 : 16;
    revExCurve = cmsReverseToneCurveEx(nSamples, curve1);
    if (revExCurve != NULL) {
        if (Size > 0) {
            float v = (float)(Data[0] & 0xFF) / 255.0f;
            cmsEvalToneCurveFloat(revExCurve, v);
        }
        cleanup_curve(revExCurve);
    }
    
    // Create second curve for joining
    if (Size > 1) {
        cmsToneCurve *tempCurve = create_test_curve_from_data(Data + 1, Size > 1 ? Size - 1 : 0);
        if (tempCurve != NULL) {
            curve2 = tempCurve;
            
            // Test cmsJoinToneCurve
            uint32_t nPoints = (Size > 2) ? (Data[2] % 1000) + 16 : 16;
            joinedCurve = cmsJoinToneCurve(NULL, curve1, curve2, nPoints);
            
            if (joinedCurve != NULL) {
                if (Size > 0) {
                    float v = (float)(Data[0] & 0xFF) / 255.0f;
                    cmsEvalToneCurveFloat(joinedCurve, v);
                }
                cleanup_curve(joinedCurve);
            }
            
            cleanup_curve(curve2);
        }
    }
    
    // Cleanup
    cleanup_curve(curve1);
    
    return 0;
}