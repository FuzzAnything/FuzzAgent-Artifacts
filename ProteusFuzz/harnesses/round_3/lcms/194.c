#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_curve(cmsToneCurve* curve) {
    if (curve) {
        cmsFreeToneCurve(curve);
    }
}

int LLVMFuzzerTestOneInput_194(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    uint8_t op = Data[0];
    uint8_t mode = Data[1];
    
    cmsToneCurve* curve = NULL;
    cmsToneCurve* dup_curve = NULL;
    cmsToneCurve* seg_curve = NULL;
    cmsToneCurve* dup_seg = NULL;
    
    cmsUInt32Number nEntries = 0;
    if (Size >= 6) {
        nEntries = ((uint32_t)Data[2] << 24) | ((uint32_t)Data[3] << 16) | 
                   ((uint32_t)Data[4] << 8) | (uint32_t)Data[5];
        if (nEntries > 65536) nEntries = 65536;
    } else {
        nEntries = 2;
    }

    cmsUInt32Number nSegments = 0;
    if (Size >= 10) {
        nSegments = ((uint32_t)Data[6] << 24) | ((uint32_t)Data[7] << 16) | 
                    ((uint32_t)Data[8] << 8) | (uint32_t)Data[9];
        if (nSegments > 100) nSegments = 100;
    } else {
        nSegments = 1;
    }

    cmsUInt16Number* table_data = NULL;
    if (nEntries > 0 && Size >= 6 + (nEntries * 2)) {
        table_data = (cmsUInt16Number*)malloc(nEntries * sizeof(cmsUInt16Number));
        if (table_data) {
            memcpy(table_data, &Data[6], nEntries * sizeof(cmsUInt16Number));
            for (uint32_t i = 0; i < nEntries; i++) {
                if (table_data[i] > 65535) table_data[i] = 65535;
            }
        }
    }

    cmsCurveSegment* segments = NULL;
    if (nSegments > 0 && Size >= 10 + (nSegments * sizeof(cmsCurveSegment))) {
        segments = (cmsCurveSegment*)malloc(nSegments * sizeof(cmsCurveSegment));
        if (segments) {
            memcpy(segments, &Data[10], nSegments * sizeof(cmsCurveSegment));
        }
    }

    switch (op % 6) {
        case 0: {
            // Hit line 226: nEntries == 0 && nSegments == 0
            curve = cmsBuildTabulatedToneCurve16(ctx, 0, NULL);
            cleanup_curve(curve);
            break;
        }
        case 1: {
            // Hit lines 255, 257, 263: nEntries > 0, nSegments = 0
            if (table_data && nEntries > 0) {
                curve = cmsBuildTabulatedToneCurve16(ctx, nEntries, table_data);
                cleanup_curve(curve);
            }
            break;
        }
        case 2: {
            // Hit lines 240-246, 271-296: nSegments > 0
            if (segments && nSegments > 0) {
                seg_curve = cmsBuildSegmentedToneCurve(ctx, nSegments, segments);
                cleanup_curve(seg_curve);
            }
            break;
        }
        case 3: {
            // Duplicate tabulated curve: Hits 263, 255, 257 via cmsDupToneCurve
            if (table_data && nEntries > 0) {
                curve = cmsBuildTabulatedToneCurve16(ctx, nEntries, table_data);
                if (curve) {
                    dup_curve = cmsDupToneCurve(curve);
                    cleanup_curve(dup_curve);
                }
                cleanup_curve(curve);
            }
            break;
        }
        case 4: {
            // Duplicate segmented curve: Hits 240-246, 271-296 via cmsDupToneCurve
            if (segments && nSegments > 0) {
                seg_curve = cmsBuildSegmentedToneCurve(ctx, nSegments, segments);
                if (seg_curve) {
                    dup_seg = cmsDupToneCurve(seg_curve);
                    cleanup_curve(dup_seg);
                }
                cleanup_curve(seg_curve);
            }
            break;
        }
        case 5: {
            // Retry edge case
            curve = cmsBuildTabulatedToneCurve16(ctx, 0, NULL);
            cleanup_curve(curve);
            break;
        }
    }

    free(table_data);
    free(segments);
    cmsDeleteContext(ctx);

    return 0;
}