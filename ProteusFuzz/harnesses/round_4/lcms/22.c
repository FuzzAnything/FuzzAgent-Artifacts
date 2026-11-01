// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_curves(cmsToneCurve *c1, cmsToneCurve *c2, cmsToneCurve *c3) {
    if (c1) cmsFreeToneCurve(c1);
    if (c2) cmsFreeToneCurve(c2);
    if (c3) cmsFreeToneCurve(c3);
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsToneCurve *curve = NULL;
    cmsToneCurve *reversed = NULL;
    cmsToneCurve *duplicated = NULL;
    const cmsUInt16Number *table = NULL;
    cmsUInt16Number eval_result = 0;

    // Parse input to determine entry count and values
    // Ensure nEntries is within reasonable bounds to avoid huge allocations
    uint16_t nEntriesRaw = Data[0] | (Data[1] << 8);
    cmsUInt32Number nEntries = (nEntriesRaw % 4096) + 1; 

    // Allocate buffer for values if we have enough data
    cmsUInt16Number *values = NULL;
    size_t needed_size = nEntries * sizeof(cmsUInt16Number);
    
    if (Size >= 2 + needed_size) {
        values = (cmsUInt16Number *)malloc(needed_size);
        if (values) {
            memcpy(values, Data + 2, needed_size);
            // Build the tone curve
            curve = cmsBuildTabulatedToneCurve16(ctx, nEntries, values);
            free(values);
        }
    } else if (Size >= 2) {
        // Fallback: generate simple values if input is too short for requested nEntries
        values = (cmsUInt16Number *)malloc(needed_size);
        if (values) {
            for (uint32_t i = 0; i < nEntries; i++) {
                values[i] = (i * 65535) / (nEntries - 1);
            }
            curve = cmsBuildTabulatedToneCurve16(ctx, nEntries, values);
            free(values);
        }
    }

    // Test cmsDupToneCurve
    if (curve) {
        duplicated = cmsDupToneCurve(curve);
    }

    // Test cmsReverseToneCurve
    if (curve) {
        reversed = cmsReverseToneCurve(curve);
    }

    // Test cmsGetToneCurveEstimatedTable
    if (curve) {
        table = cmsGetToneCurveEstimatedTable(curve);
        // Just read a value if table exists to exercise the path
        if (table && nEntries > 0) {
            (void)table[0];
        }
    }
    if (reversed) {
        table = cmsGetToneCurveEstimatedTable(reversed);
        if (table) {
            (void)table[0];
        }
    }
    if (duplicated) {
        table = cmsGetToneCurveEstimatedTable(duplicated);
        if (table) {
            (void)table[0];
        }
    }

    // Test cmsEvalToneCurve16
    // Use input data to generate evaluation points
    cmsUInt16Number eval_input = 0;
    if (Size >= 2) {
        eval_input = Data[0] | (Data[1] << 8);
    }

    if (curve) {
        eval_result = cmsEvalToneCurve16(curve, eval_input);
        (void)eval_result;
    }
    if (reversed) {
        eval_result = cmsEvalToneCurve16(reversed, eval_input);
        (void)eval_result;
    }
    if (duplicated) {
        eval_result = cmsEvalToneCurve16(duplicated, eval_input);
        (void)eval_result;
    }

    // Cleanup
    cleanup_curves(curve, reversed, duplicated);
    cmsDeleteContext(ctx);

    return 0;
}