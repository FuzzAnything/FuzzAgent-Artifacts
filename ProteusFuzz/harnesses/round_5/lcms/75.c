// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
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

static void cleanup_curves(cmsToneCurve *c1, cmsToneCurve *c2, cmsToneCurve *c3, cmsToneCurve *c4, cmsToneCurve *c5) {
    if (c1) cmsFreeToneCurve(c1);
    if (c2) cmsFreeToneCurve(c2);
    if (c3) cmsFreeToneCurve(c3);
    if (c4) cmsFreeToneCurve(c4);
    if (c5) cmsFreeToneCurve(c5);
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext Context = cmsCreateContext(NULL, NULL);
    if (!Context) return 0;

    // Extract parameters from input data
    uint16_t nEntries = Data[0];
    if (nEntries == 0) nEntries = 1;
    if (nEntries > 65536) nEntries = 65536;

    // Ensure we have enough data for the table
    size_t required_size = nEntries * sizeof(cmsUInt16Number);
    if (Size < 2 + required_size) {
        cmsDeleteContext(Context);
        return 0;
    }

    const cmsUInt16Number *raw_values = (const cmsUInt16Number *)(Data + 2);

    // 1. Build a tabulated tone curve
    cmsToneCurve *curve1 = cmsBuildTabulatedToneCurve16(Context, nEntries, raw_values);
    if (!curve1) {
        cmsDeleteContext(Context);
        return 0;
    }

    // 2. Duplicate the curve
    cmsToneCurve *curve2 = cmsDupToneCurve(curve1);

    // 3. Evaluate the curve
    cmsUInt16Number eval_input = (Data[1] % 65536);
    cmsUInt16Number eval_result = 0;
    if (curve1) {
        eval_result = cmsEvalToneCurve16(curve1, eval_input);
    }

    // 4. Get estimated table
    const cmsUInt16Number *table = NULL;
    if (curve1) {
        table = cmsGetToneCurveEstimatedTable(curve1);
        if (table) {
            // Read a few bytes to trigger state exploration
            volatile cmsUInt16Number dummy = table[0];
            (void)dummy;
        }
    }

    // 5. Reverse the curve (standard)
    cmsToneCurve *curve3 = NULL;
    if (curve1) {
        curve3 = cmsReverseToneCurve(curve1);
    }

    // 6. Reverse the curve (extended)
    cmsToneCurve *curve4 = NULL;
    cmsUInt32Number nResultSamples = (Data[1] % 100) + 1;
    if (nResultSamples > 65536) nResultSamples = 65536;
    if (curve1) {
        curve4 = cmsReverseToneCurveEx(nResultSamples, curve1);
    }

    // 7. Duplicate the reversed curve for extra state
    cmsToneCurve *curve5 = NULL;
    if (curve3) {
        curve5 = cmsDupToneCurve(curve3);
    }

    // Cleanup
    cleanup_curves(curve1, curve2, curve3, curve4, curve5);
    cmsDeleteContext(Context);

    return 0;
}