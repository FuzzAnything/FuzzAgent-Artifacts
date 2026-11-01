// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
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

static void cleanup_curves(cmsToneCurve *c1, cmsToneCurve *c2, cmsToneCurve *c3, cmsToneCurve *c4) {
    if (c1) cmsFreeToneCurve(c1);
    if (c2) cmsFreeToneCurve(c2);
    if (c3) cmsFreeToneCurve(c3);
    if (c4) cmsFreeToneCurve(c4);
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    uint32_t nEntries = (Data[0] << 16) | (Data[1] << 8) | Data[2];
    uint32_t nSegments = Data[3];

    // Cap values to prevent excessive allocation or invalid memory access
    if (nEntries > 10000) nEntries = 10000;
    if (nSegments > 100) nSegments = 100;

    cmsToneCurve *curve16 = NULL;
    cmsToneCurve *curveFloat = NULL;
    cmsToneCurve *curveSegmented = NULL;
    cmsToneCurve *curveReversed = NULL;
    cmsToneCurve *curveJoined = NULL;

    // 1. Test cmsBuildTabulatedToneCurve16
    if (nEntries > 0 && Size >= 4 + nEntries * 2) {
        cmsUInt16Number *values16 = (cmsUInt16Number *)(Data + 4);
        curve16 = cmsBuildTabulatedToneCurve16(ContextID, nEntries, values16);
    }

    // 2. Test cmsGetToneCurveEstimatedTableEntries
    if (curve16) {
        cmsUInt32Number entries = cmsGetToneCurveEstimatedTableEntries(curve16);
        (void)entries; // Use the value to avoid unused warning
    }

    // 3. Test cmsBuildSegmentedToneCurve
    if (nSegments > 0 && Size >= 4 + nSegments * sizeof(cmsCurveSegment)) {
        cmsCurveSegment *segments = (cmsCurveSegment *)(Data + 4);
        curveSegmented = cmsBuildSegmentedToneCurve(ContextID, nSegments, segments);
    }

    // 4. Test cmsReverseToneCurveEx
    cmsToneCurve *sourceForReverse = curve16 ? curve16 : curveSegmented;
    if (sourceForReverse) {
        uint32_t nResultSamples = (Data[0] % 100) + 1; // Ensure at least 1 sample
        curveReversed = cmsReverseToneCurveEx(nResultSamples, sourceForReverse);
    }

    // 5. Test cmsBuildTabulatedToneCurveFloat
    if (nEntries > 0 && Size >= 4 + nEntries * sizeof(cmsFloat32Number)) {
        cmsFloat32Number *valuesFloat = (cmsFloat32Number *)(Data + 4);
        curveFloat = cmsBuildTabulatedToneCurveFloat(ContextID, nEntries, valuesFloat);
    }

    // 6. Test cmsJoinToneCurve
    cmsToneCurve *curveX = curve16 ? curve16 : curveFloat;
    cmsToneCurve *curveY = curveSegmented ? curveSegmented : curveReversed;
    if (curveX && curveY) {
        uint32_t nPoints = (Data[0] % 100) + 1;
        curveJoined = cmsJoinToneCurve(ContextID, curveX, curveY, nPoints);
    }

    // Cleanup
    cleanup_curves(curve16, curveFloat, curveSegmented, curveReversed);
    if (curveJoined) cmsFreeToneCurve(curveJoined);
    cmsDeleteContext(ContextID);

    return 0;
}