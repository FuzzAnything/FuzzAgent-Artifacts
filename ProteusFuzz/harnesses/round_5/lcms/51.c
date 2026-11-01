// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void safe_free_curves(cmsToneCurve* curves[3]) {
    if (curves != NULL) {
        cmsFreeToneCurveTriple(curves);
    }
}

static cmsToneCurve* build_segmented_curve_from_fuzz(cmsContext ctx, const uint8_t *Data, size_t Size) {
    if (Size < 4) return NULL;
    
    // Interpret first 4 bytes as number of segments
    cmsUInt32Number nSegments = 0;
    memcpy(&nSegments, Data, sizeof(cmsUInt32Number));
    
    // Limit segments to prevent excessive allocation
    if (nSegments == 0 || nSegments > 100) {
        nSegments = 1;
    }
    
    // Calculate required size for segments
    size_t required = nSegments * sizeof(cmsCurveSegment);
    if (Size < 4 + required) {
        return NULL;
    }
    
    cmsCurveSegment* segments = (cmsCurveSegment*)(Data + 4);
    return cmsBuildSegmentedToneCurve(ctx, nSegments, segments);
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsToneCurve* curve1 = NULL;
    cmsToneCurve* curve2 = NULL;
    cmsToneCurve* joined = NULL;
    cmsToneCurve* reversed = NULL;
    cmsToneCurve* duplicated = NULL;
    cmsToneCurve* curves[3] = {NULL, NULL, NULL};

    // Try to build a segmented tone curve from fuzz data
    curve1 = build_segmented_curve_from_fuzz(ctx, Data, Size);
    
    // If curve1 is valid, try other operations
    if (curve1 != NULL) {
        // Test cmsDupToneCurve
        duplicated = cmsDupToneCurve(curve1);
        
        // Test cmsGetToneCurveEstimatedTableEntries
        if (duplicated != NULL) {
            cmsGetToneCurveEstimatedTableEntries(duplicated);
            cmsGetToneCurveEstimatedTableEntries(curve1);
        }
        
        // Test cmsReverseToneCurveEx
        if (Size > 0) {
            cmsUInt32Number nSamples = 0;
            memcpy(&nSamples, Data, sizeof(cmsUInt32Number));
            if (nSamples == 0 || nSamples > 10000) nSamples = 256;
            reversed = cmsReverseToneCurveEx(nSamples, curve1);
        }
        
        // Build a second curve for joining
        if (Size >= 8) {
            curve2 = build_segmented_curve_from_fuzz(ctx, Data + 4, Size - 4);
        }
        
        // Test cmsJoinToneCurve
        if (curve1 != NULL && curve2 != NULL) {
            cmsUInt32Number nPoints = 0;
            if (Size >= 12) {
                memcpy(&nPoints, Data + 8, sizeof(cmsUInt32Number));
                if (nPoints == 0 || nPoints > 10000) nPoints = 256;
            } else {
                nPoints = 256;
            }
            joined = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
        }
        
        // Prepare triple for freeing
        curves[0] = duplicated;
        curves[1] = reversed;
        curves[2] = joined;
    }
    
    // Cleanup using cmsFreeToneCurveTriple for the array
    if (curves[0] != NULL || curves[1] != NULL || curves[2] != NULL) {
        cmsFreeToneCurveTriple(curves);
    }
    
    // Free individual curves that were not added to the triple
    if (curve1 != NULL) cmsFreeToneCurve(curve1);
    if (curve2 != NULL) cmsFreeToneCurve(curve2);
    
    cmsDeleteContext(ctx);
    
    return 0;
}