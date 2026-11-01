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

static void cleanup_uint16_array(cmsUInt16Number *arr) {
    if (arr) free(arr);
}

static void cleanup_float_array(cmsFloat32Number *arr) {
    if (arr) free(arr);
}

int LLVMFuzzerTestOneInput_218(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    uint8_t mode = Data[0] % 4;
    uint32_t nEntries = 0;
    uint32_t nSegments = 0;
    
    if (Size >= 6) {
        nEntries = *(uint32_t*)(Data + 1);
        nSegments = *(uint32_t*)(Data + 5);
    } else if (Size >= 2) {
        nEntries = Data[1];
        nSegments = Data[1];
    }

    // Limit sizes to prevent excessive memory allocation, but allow enough to test logic
    if (nEntries > 10000) nEntries = 10000;
    if (nSegments > 100) nSegments = 100;

    cmsToneCurve *curve = NULL;
    cmsFloat32Number *floatBuf = NULL;
    cmsUInt16Number *uint16Buf = NULL;
    cmsCurveSegment *segments = NULL;

    switch (mode) {
        case 0:
            // Test cmsBuildTabulatedToneCurve16 with nEntries=0 to hit Line 226
            if (nEntries == 0) {
                curve = cmsBuildTabulatedToneCurve16(ctx, 0, NULL);
                cleanup_curve(curve);
            } else {
                // Allocate buffer for values to hit Line 263
                size_t bufSize = nEntries * sizeof(cmsUInt16Number);
                if (Size >= 6 + bufSize) {
                    uint16Buf = (cmsUInt16Number*)malloc(bufSize);
                    if (uint16Buf) {
                        memcpy(uint16Buf, Data + 6, bufSize);
                        curve = cmsBuildTabulatedToneCurve16(ctx, nEntries, uint16Buf);
                        cleanup_curve(curve);
                    }
                }
            }
            break;

        case 1:
            // Test cmsBuildSegmentedToneCurve with Type 0 and SampledPoints to hit Line 286
            if (nSegments > 0 && Size >= 10) {
                size_t segSize = nSegments * sizeof(cmsCurveSegment);
                segments = (cmsCurveSegment*)malloc(segSize);
                if (segments) {
                    for (uint32_t i = 0; i < nSegments; i++) {
                        memset(&segments[i], 0, sizeof(cmsCurveSegment));
                        segments[i].Type = (Data[6 + i] % 2); // 0 or 1
                        segments[i].x0 = 0.0;
                        segments[i].x1 = 1.0;
                        
                        if (segments[i].Type == 0) {
                            segments[i].nGridPoints = 2;
                            cmsFloat32Number *points = (cmsFloat32Number*)malloc(2 * sizeof(cmsFloat32Number));
                            if (points) {
                                points[0] = 0.0f;
                                points[1] = 1.0f;
                                segments[i].SampledPoints = points;
                            }
                        }
                    }
                    curve = cmsBuildSegmentedToneCurve(ctx, nSegments, segments);
                    cleanup_curve(curve);
                    
                    for (uint32_t i = 0; i < nSegments; i++) {
                        if (segments[i].SampledPoints) {
                            free(segments[i].SampledPoints);
                        }
                    }
                    free(segments);
                }
            }
            break;

        case 2:
            // Test cmsBuildSegmentedToneCurve with Type 1 (Gamma) to hit Line 271, 299
            if (nSegments > 0) {
                size_t segSize = nSegments * sizeof(cmsCurveSegment);
                segments = (cmsCurveSegment*)malloc(segSize);
                if (segments) {
                    for (uint32_t i = 0; i < nSegments; i++) {
                        memset(&segments[i], 0, sizeof(cmsCurveSegment));
                        segments[i].Type = 1; // Gamma
                        segments[i].x0 = 0.0;
                        segments[i].x1 = 1.0;
                        segments[i].Params[0] = 1.0 + (float)(Data[6 + i] % 10) / 10.0;
                    }
                    curve = cmsBuildSegmentedToneCurve(ctx, nSegments, segments);
                    cleanup_curve(curve);
                    free(segments);
                }
            }
            break;

        case 3:
            // Test cmsBuildTabulatedToneCurve16 with nEntries > 0 or 0
            if (nEntries > 0) {
                size_t bufSize = nEntries * sizeof(cmsUInt16Number);
                if (Size >= 6 + bufSize) {
                    uint16Buf = (cmsUInt16Number*)malloc(bufSize);
                    if (uint16Buf) {
                        memcpy(uint16Buf, Data + 6, bufSize);
                        curve = cmsBuildTabulatedToneCurve16(ctx, nEntries, uint16Buf);
                        cleanup_curve(curve);
                    }
                }
            } else {
                curve = cmsBuildTabulatedToneCurve16(ctx, 0, NULL);
                cleanup_curve(curve);
            }
            break;
    }

    cleanup_uint16_array(uint16Buf);
    cleanup_float_array(floatBuf);
    cmsDeleteContext(ctx);

    return 0;
}