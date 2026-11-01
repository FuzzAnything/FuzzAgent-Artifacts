// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_curve(cmsToneCurve *curve) {
    if (curve) {
        cmsFreeToneCurve(curve);
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Create a context for the fuzzing session
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Extract parameters from input data safely
    uint32_t nEntries = 0;
    uint32_t nSegments = 0;
    uint32_t nPoints = 0;
    uint32_t nResultSamples = 0;
    uint8_t opCode = Data[0] % 7;

    // Ensure we have enough data for the parameters we want to read
    if (Size >= 6) {
        nEntries = *(uint32_t*)(Data + 1);
        nSegments = *(uint32_t*)(Data + 5);
        nPoints = *(uint32_t*)(Data + 1);
        nResultSamples = *(uint32_t*)(Data + 5);
    } else if (Size >= 2) {
        // Fallback if size is small but >= 2
        nEntries = Data[1];
        nSegments = Data[1];
        nPoints = Data[1];
        nResultSamples = Data[1];
    }

    // Limit sizes to prevent excessive memory allocation
    if (nEntries > 10000) nEntries = 10000;
    if (nSegments > 1000) nSegments = 1000;
    if (nPoints > 10000) nPoints = 10000;
    if (nResultSamples > 10000) nResultSamples = 10000;

    cmsToneCurve *curve1 = NULL;
    cmsToneCurve *curve2 = NULL;
    cmsToneCurve *result = NULL;

    // Allocate buffers for values
    cmsFloat32Number *floatValues = NULL;
    cmsUInt16Number *uint16Values = NULL;
    cmsCurveSegment *segments = NULL;

    // Calculate required size for float values
    size_t floatSize = nEntries * sizeof(cmsFloat32Number);
    if (nEntries > 0 && Size >= 6 + floatSize) {
        floatValues = (cmsFloat32Number*)malloc(floatSize);
        if (floatValues) {
            memcpy(floatValues, Data + 6, floatSize);
        }
    }

    // Calculate required size for uint16 values
    size_t uint16Size = nEntries * sizeof(cmsUInt16Number);
    if (nEntries > 0 && Size >= 6 + uint16Size) {
        uint16Values = (cmsUInt16Number*)malloc(uint16Size);
        if (uint16Values) {
            memcpy(uint16Values, Data + 6, uint16Size);
        }
    }

    // Calculate required size for segments
    size_t segSize = nSegments * sizeof(cmsCurveSegment);
    if (nSegments > 0 && Size >= 6 + segSize) {
        segments = (cmsCurveSegment*)malloc(segSize);
        if (segments) {
            memcpy(segments, Data + 6, segSize);
        }
    }

    switch (opCode) {
        case 0:
            // cmsBuildTabulatedToneCurveFloat
            if (floatValues) {
                result = cmsBuildTabulatedToneCurveFloat(ctx, nEntries, floatValues);
                cleanup_curve(result);
            }
            break;

        case 1:
            // cmsBuildTabulatedToneCurve16
            if (uint16Values) {
                result = cmsBuildTabulatedToneCurve16(ctx, nEntries, uint16Values);
                cleanup_curve(result);
            }
            break;

        case 2:
            // cmsBuildSegmentedToneCurve
            if (segments) {
                result = cmsBuildSegmentedToneCurve(ctx, nSegments, segments);
                cleanup_curve(result);
            }
            break;

        case 3:
            // cmsReverseToneCurveEx (needs a valid curve first)
            if (floatValues && nEntries > 0) {
                curve1 = cmsBuildTabulatedToneCurveFloat(ctx, nEntries, floatValues);
                if (curve1) {
                    result = cmsReverseToneCurveEx(nResultSamples, curve1);
                    cleanup_curve(result);
                    cleanup_curve(curve1);
                }
            }
            break;

        case 4:
            // cmsJoinToneCurve (needs two valid curves)
            if (floatValues && nEntries > 0) {
                curve1 = cmsBuildTabulatedToneCurveFloat(ctx, nEntries, floatValues);
                curve2 = cmsBuildTabulatedToneCurveFloat(ctx, nEntries, floatValues);
                if (curve1 && curve2) {
                    result = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
                    cleanup_curve(result);
                }
                cleanup_curve(curve1);
                cleanup_curve(curve2);
            }
            break;

        case 5:
            // cmsGetToneCurveEstimatedTableEntries (needs a valid curve)
            if (floatValues && nEntries > 0) {
                curve1 = cmsBuildTabulatedToneCurveFloat(ctx, nEntries, floatValues);
                if (curve1) {
                    cmsUInt32Number entries = cmsGetToneCurveEstimatedTableEntries(curve1);
                    (void)entries; // Use the value to avoid unused warning
                    cleanup_curve(curve1);
                }
            }
            break;

        case 6:
            // Test edge cases with valid but minimal inputs to avoid NULL assertions
            {
                cmsFloat32Number dummyFloat[2] = {0.0f, 1.0f};
                cmsUInt16Number dummyUInt16[2] = {0, 65535};
                
                curve1 = cmsBuildTabulatedToneCurveFloat(ctx, 2, dummyFloat);
                curve2 = cmsBuildTabulatedToneCurve16(ctx, 2, dummyUInt16);
                
                if (curve1 && curve2) {
                    // Test Join with valid curves
                    result = cmsJoinToneCurve(ctx, curve1, curve2, 100);
                    cleanup_curve(result);
                    
                    // Test Reverse
                    result = cmsReverseToneCurveEx(100, curve1);
                    cleanup_curve(result);
                    
                    // Test GetEntries
                    cmsUInt32Number entries = cmsGetToneCurveEstimatedTableEntries(curve1);
                    (void)entries;
                }
                
                cleanup_curve(curve1);
                cleanup_curve(curve2);
            }
            break;
    }

    // Cleanup allocated buffers
    free(floatValues);
    free(uint16Values);
    free(segments);

    // Cleanup context
    cmsDeleteContext(ctx);

    return 0;
}