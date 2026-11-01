// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetToneCurveSegment at cmsgamma.c:1507:34 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
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

static void cleanup_curve_triple(cmsToneCurve* curves[3]) {
    if (curves) {
        cmsFreeToneCurveTriple(curves);
    }
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Extract data for curve creation parameters
    cmsUInt32Number nSegments = 0;
    cmsUInt32Number nPoints = 0;
    cmsUInt32Number nResultSamples = 0;
    cmsInt32Number segmentIndex = 0;

    if (Size >= 4) {
        nSegments = *(const cmsUInt32Number*)(Data);
        nPoints = *(const cmsUInt32Number*)(Data + 4);
        if (Size >= 8) {
            nResultSamples = *(const cmsUInt32Number*)(Data + 8);
        }
        if (Size >= 12) {
            segmentIndex = *(const cmsInt32Number*)(Data + 12);
        }
    }

    // Limit values to prevent excessive allocation or invalid states
    if (nSegments > 100) nSegments = 100;
    if (nPoints > 1000) nPoints = 1000;
    if (nResultSamples > 10000) nResultSamples = 10000;
    if (segmentIndex < 0 || segmentIndex > 100) segmentIndex = 0;

    // Build a segmented tone curve
    cmsCurveSegment* segments = NULL;
    if (nSegments > 0 && Size >= 12 + (size_t)nSegments * sizeof(cmsCurveSegment)) {
        segments = (cmsCurveSegment*)(Data + 12);
    } else if (nSegments > 0) {
        // Generate dummy segments if not enough data
        segments = (cmsCurveSegment*)malloc(nSegments * sizeof(cmsCurveSegment));
        if (segments) {
            for (cmsUInt32Number i = 0; i < nSegments; i++) {
                segments[i].x0 = (cmsFloat64Number)(i % 10) / 10.0;
                segments[i].x1 = (cmsFloat64Number)((i + 1) % 10) / 10.0;
                // cmsCurveSegment does not have y0/y1 members in lcms2 public API.
                // The segment is defined by x0, x1, Type, and Params.
                // We set Type to a known valid type (e.g., CMS_CURVE_LINEAR) if available,
                // or 0. For this fuzzer, we assume the structure allows setting params.
                segments[i].Type = 0; 
                memset(&segments[i].Params, 0, sizeof(segments[i].Params));
            }
        }
    }

    cmsToneCurve* curve1 = NULL;
    if (segments && nSegments > 0) {
        curve1 = cmsBuildSegmentedToneCurve(ctx, nSegments, segments);
    }

    if (segments && segments != (cmsCurveSegment*)(Data + 12)) {
        free(segments);
    }

    // Duplicate the curve
    cmsToneCurve* curve2 = NULL;
    if (curve1) {
        curve2 = cmsDupToneCurve(curve1);
    }

    // Join tone curves
    cmsToneCurve* joinedCurve = NULL;
    if (curve1 && curve2) {
        joinedCurve = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
    }

    // Reverse tone curve
    cmsToneCurve* reversedCurve = NULL;
    if (curve1) {
        reversedCurve = cmsReverseToneCurveEx(nResultSamples, curve1);
    }

    // Get tone curve segment
    const cmsCurveSegment* segment = NULL;
    if (curve1) {
        segment = cmsGetToneCurveSegment(segmentIndex, curve1);
        (void)segment; // Use the result to avoid unused variable warning
    }

    // Free tone curve triple
    cmsToneCurve* triple[3] = {curve1, curve2, joinedCurve};
    cmsFreeToneCurveTriple(triple);

    // Cleanup remaining curves
    cleanup_curve(reversedCurve);

    // Cleanup context
    cmsDeleteContext(ctx);

    return 0;
}