// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_curves(cmsToneCurve* c1, cmsToneCurve* c2, cmsToneCurve* c3, cmsToneCurve* c4, cmsToneCurve* c5) {
    if (c1) cmsFreeToneCurve(c1);
    if (c2) cmsFreeToneCurve(c2);
    if (c3) cmsFreeToneCurve(c3);
    if (c4) cmsFreeToneCurve(c4);
    if (c5) cmsFreeToneCurve(c5);
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    cmsToneCurve *curve1 = NULL;
    cmsToneCurve *curve2 = NULL;
    cmsToneCurve *curve3 = NULL;
    cmsToneCurve *curve4 = NULL;
    cmsToneCurve *curve5 = NULL;

    // Extract parameters from fuzz data
    cmsUInt32Number nEntries = Data[0] + 1; // Avoid 0 entries
    cmsUInt32Number nSegments = (Data[1] % 10) + 1;
    cmsUInt32Number nResultSamples = (Data[0] % 50) + 1;
    cmsUInt32Number nPoints = (Data[1] % 100) + 1;

    // Fuzz cmsBuildTabulatedToneCurveFloat
    if (Size >= 2 + nEntries * sizeof(cmsFloat32Number)) {
        cmsFloat32Number *floatValues = (cmsFloat32Number *)(Data + 2);
        curve1 = cmsBuildTabulatedToneCurveFloat(ContextID, nEntries, floatValues);
    }

    // Fuzz cmsBuildSegmentedToneCurve
    if (Size >= 2 + nSegments * sizeof(cmsCurveSegment)) {
        cmsCurveSegment *segments = (cmsCurveSegment *)(Data + 2);
        // Ensure segments are somewhat valid to avoid immediate crashes in internal logic if possible, 
        // but let the fuzzer explore invalid states too.
        curve2 = cmsBuildSegmentedToneCurve(ContextID, nSegments, segments);
    }

    // Fuzz cmsBuildTabulatedToneCurve16
    if (Size >= 2 + nEntries * sizeof(cmsUInt16Number)) {
        cmsUInt16Number *uint16Values = (cmsUInt16Number *)(Data + 2);
        curve4 = cmsBuildTabulatedToneCurve16(ContextID, nEntries, uint16Values);
    }

    // Fuzz cmsReverseToneCurveEx
    // Try reversing curve1 if it exists
    if (curve1) {
        curve3 = cmsReverseToneCurveEx(nResultSamples, curve1);
    }
    // Also try reversing curve2 if it exists
    if (curve2) {
        cmsToneCurve *temp = cmsReverseToneCurveEx(nResultSamples, curve2);
        if (curve3) cmsFreeToneCurve(curve3); // Free previous if exists
        curve3 = temp;
    }

    // Fuzz cmsJoinToneCurve
    // Join curve1 and curve2
    if (curve1 && curve2) {
        curve5 = cmsJoinToneCurve(ContextID, curve1, curve2, nPoints);
    }
    // Join curve4 and curve3
    if (curve4 && curve3) {
        cmsToneCurve *temp = cmsJoinToneCurve(ContextID, curve4, curve3, nPoints);
        if (curve5) cmsFreeToneCurve(curve5);
        curve5 = temp;
    }

    // Fuzz cmsGetToneCurveEstimatedTableEntries
    // Test on all valid curves
    cmsUInt32Number entries = 0;
    if (curve1) entries = cmsGetToneCurveEstimatedTableEntries(curve1);
    if (curve2) entries = cmsGetToneCurveEstimatedTableEntries(curve2);
    if (curve3) entries = cmsGetToneCurveEstimatedTableEntries(curve3);
    if (curve4) entries = cmsGetToneCurveEstimatedTableEntries(curve4);
    if (curve5) entries = cmsGetToneCurveEstimatedTableEntries(curve5);

    // Cleanup
    cleanup_curves(curve1, curve2, curve3, curve4, curve5);
    cmsDeleteContext(ContextID);

    return 0;
}