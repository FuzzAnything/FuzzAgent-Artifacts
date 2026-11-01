// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_curves(cmsToneCurve* curves[3]) {
    if (curves) {
        for (int i = 0; i < 3; i++) {
            if (curves[i]) {
                cmsFreeToneCurve(curves[i]);
                curves[i] = NULL;
            }
        }
    }
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    cmsToneCurve *curves[3] = {NULL, NULL, NULL};
    cmsToneCurve *joined = NULL;
    cmsToneCurve *reversed = NULL;
    cmsToneCurve *segmented = NULL;
    cmsToneCurve *tabulated = NULL;

    // Extract parameters from fuzz data
    cmsUInt32Number nSegments = Data[0];
    cmsUInt32Number nEntries = Data[1];
    cmsUInt32Number nPoints = (Size > 2) ? Data[2] : 10;
    cmsUInt32Number nResultSamples = (Size > 3) ? Data[3] : 10;

    // Ensure reasonable bounds to prevent allocation failures or crashes
    if (nSegments > 100) nSegments = 100;
    if (nEntries > 1000) nEntries = 1000;
    if (nPoints > 1000) nPoints = 1000;
    if (nResultSamples > 1000) nResultSamples = 1000;

    // Test cmsBuildSegmentedToneCurve
    // Note: cmsCurveSegment structure is opaque in public headers. 
    // We cannot directly access members like x, y, a, b, c, d.
    // Instead, we rely on the fact that the function expects a valid array.
    // Since we cannot construct a valid segment without internal knowledge,
    // we skip this specific construction or pass a zeroed buffer if the API tolerates it.
    // However, to avoid undefined behavior, we will skip creating segmented curves 
    // unless we can safely construct them. For fuzzing, we might try passing a zeroed array.
    if (nSegments > 0 && Size >= 4) {
        cmsCurveSegment *segments = (cmsCurveSegment *)calloc(nSegments, sizeof(cmsCurveSegment));
        if (segments) {
            // We cannot safely populate members without knowing the struct layout.
            // Passing zeroed memory might trigger internal checks or create identity curves.
            segmented = cmsBuildSegmentedToneCurve(ContextID, nSegments, segments);
            free(segments);
        }
    }

    // Test cmsBuildTabulatedToneCurveFloat
    if (nEntries > 0 && Size >= 4) {
        cmsFloat32Number *values = (cmsFloat32Number *)malloc(nEntries * sizeof(cmsFloat32Number));
        if (values) {
            for (uint32_t i = 0; i < nEntries && i < Size - 4; i++) {
                values[i] = (float)(Data[4 + i] / 255.0);
            }
            tabulated = cmsBuildTabulatedToneCurveFloat(ContextID, nEntries, values);
            free(values);
        }
    }

    // Test cmsGetToneCurveEstimatedTableEntries
    if (segmented) {
        cmsGetToneCurveEstimatedTableEntries(segmented);
    }
    if (tabulated) {
        cmsGetToneCurveEstimatedTableEntries(tabulated);
    }

    // Test cmsJoinToneCurve
    if (segmented && tabulated) {
        joined = cmsJoinToneCurve(ContextID, segmented, tabulated, nPoints);
    } else if (segmented) {
        joined = cmsJoinToneCurve(ContextID, segmented, segmented, nPoints);
    } else if (tabulated) {
        joined = cmsJoinToneCurve(ContextID, tabulated, tabulated, nPoints);
    }

    // Test cmsReverseToneCurveEx
    if (segmented) {
        reversed = cmsReverseToneCurveEx(nResultSamples, segmented);
    } else if (tabulated) {
        reversed = cmsReverseToneCurveEx(nResultSamples, tabulated);
    } else if (joined) {
        reversed = cmsReverseToneCurveEx(nResultSamples, joined);
    }

    // Test cmsFreeToneCurveTriple
    if (segmented) curves[0] = segmented;
    if (tabulated) curves[1] = tabulated;
    if (joined) curves[2] = joined;

    cmsFreeToneCurveTriple(curves);
    cleanup_curves(curves); // Ensure cleanup even if triple didn't free everything

    if (reversed) {
        cmsFreeToneCurve(reversed);
    }

    cmsDeleteContext(ContextID);
    return 0;
}