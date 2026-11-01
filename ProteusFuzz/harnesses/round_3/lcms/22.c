// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_curves(cmsToneCurve *c1, cmsToneCurve *c2, cmsToneCurve *c3, cmsToneCurve *c4, cmsToneCurve *c5) {
    if (c1) cmsFreeToneCurve(c1);
    if (c2) cmsFreeToneCurve(c2);
    if (c3) cmsFreeToneCurve(c3);
    if (c4) cmsFreeToneCurve(c4);
    if (c5) cmsFreeToneCurve(c5);
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    cmsToneCurve *curve1 = NULL;
    cmsToneCurve *curve2 = NULL;
    cmsToneCurve *curve3 = NULL;
    cmsToneCurve *curve4 = NULL;
    cmsToneCurve *curve5 = NULL;
    cmsToneCurve *reversed = NULL;
    cmsToneCurve *joined = NULL;

    // Parse input data to generate parameters safely
    uint32_t nEntries = 1;
    uint32_t nSegments = 1;
    uint32_t nResultSamples = 1;
    uint32_t nPoints = 1;

    // Ensure we have enough data to read 4 uint32_t values (16 bytes)
    if (Size >= 16) {
        nEntries = *(const uint32_t*)(Data);
        nSegments = *(const uint32_t*)(Data + 4);
        nResultSamples = *(const uint32_t*)(Data + 8);
        nPoints = *(const uint32_t*)(Data + 12);
    }

    // Clamp values to reasonable limits to prevent OOM or crashes
    if (nEntries == 0) nEntries = 1;
    if (nEntries > 65536) nEntries = 65536;
    if (nSegments == 0) nSegments = 1;
    if (nSegments > 1000) nSegments = 1000;
    if (nResultSamples == 0) nResultSamples = 1;
    if (nResultSamples > 65536) nResultSamples = 65536;
    if (nPoints == 0) nPoints = 1;
    if (nPoints > 65536) nPoints = 65536;

    // 1. cmsBuildTabulatedToneCurve16
    // Need 16 bytes header + nEntries * 2 bytes
    size_t required16 = 16 + (size_t)nEntries * 2;
    if (Size >= required16) {
        const cmsUInt16Number *values16 = (const cmsUInt16Number*)(Data + 16);
        curve1 = cmsBuildTabulatedToneCurve16(ContextID, nEntries, values16);
    }

    // 2. cmsBuildTabulatedToneCurveFloat
    // Need 16 bytes header + nEntries * 4 bytes
    size_t requiredFloat = 16 + (size_t)nEntries * 4;
    if (Size >= requiredFloat) {
        const cmsFloat32Number *valuesFloat = (const cmsFloat32Number*)(Data + 16);
        curve2 = cmsBuildTabulatedToneCurveFloat(ContextID, nEntries, valuesFloat);
    }

    // 3. cmsBuildSegmentedToneCurve
    // Need 16 bytes header + nSegments * sizeof(cmsCurveSegment)
    size_t requiredSeg = 16 + (size_t)nSegments * sizeof(cmsCurveSegment);
    if (Size >= requiredSeg) {
        const cmsCurveSegment *segments = (const cmsCurveSegment*)(Data + 16);
        curve3 = cmsBuildSegmentedToneCurve(ContextID, nSegments, segments);
    }

    // 4. cmsReverseToneCurveEx
    if (curve1) {
        reversed = cmsReverseToneCurveEx(nResultSamples, curve1);
        if (reversed) {
            cmsGetToneCurveEstimatedTableEntries(reversed);
            cmsFreeToneCurve(reversed);
            reversed = NULL;
        }
    }
    if (curve2) {
        reversed = cmsReverseToneCurveEx(nResultSamples, curve2);
        if (reversed) {
            cmsGetToneCurveEstimatedTableEntries(reversed);
            cmsFreeToneCurve(reversed);
            reversed = NULL;
        }
    }
    if (curve3) {
        reversed = cmsReverseToneCurveEx(nResultSamples, curve3);
        if (reversed) {
            cmsGetToneCurveEstimatedTableEntries(reversed);
            cmsFreeToneCurve(reversed);
            reversed = NULL;
        }
    }

    // 5. cmsJoinToneCurve
    // Try joining curve1 with curve2
    if (curve1 && curve2) {
        joined = cmsJoinToneCurve(ContextID, curve1, curve2, nPoints);
        if (joined) {
            cmsGetToneCurveEstimatedTableEntries(joined);
            cmsFreeToneCurve(joined);
            joined = NULL;
        }
    }
    // Try joining curve2 with curve1
    if (curve2 && curve1) {
        joined = cmsJoinToneCurve(ContextID, curve2, curve1, nPoints);
        if (joined) {
            cmsGetToneCurveEstimatedTableEntries(joined);
            cmsFreeToneCurve(joined);
            joined = NULL;
        }
    }
    // Try joining curve3 with curve1
    if (curve3 && curve1) {
        joined = cmsJoinToneCurve(ContextID, curve3, curve1, nPoints);
        if (joined) {
            cmsGetToneCurveEstimatedTableEntries(joined);
            cmsFreeToneCurve(joined);
            joined = NULL;
        }
    }

    // Test cmsGetToneCurveEstimatedTableEntries on created curves
    if (curve1) cmsGetToneCurveEstimatedTableEntries(curve1);
    if (curve2) cmsGetToneCurveEstimatedTableEntries(curve2);
    if (curve3) cmsGetToneCurveEstimatedTableEntries(curve3);

    // Cleanup
    cleanup_curves(curve1, curve2, curve3, curve4, curve5);
    cmsDeleteContext(ContextID);

    return 0;
}