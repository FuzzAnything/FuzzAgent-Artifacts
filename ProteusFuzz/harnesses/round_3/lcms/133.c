// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_curve(cmsToneCurve* curve) {
    if (curve) {
        cmsFreeToneCurve(curve);
    }
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsToneCurve* curve = NULL;
    cmsToneCurve* reversed = NULL;
    cmsToneCurve* reversed_ex = NULL;
    cmsToneCurve* dup_curve = NULL;

    uint16_t* table_data = NULL;
    uint32_t n_entries = 0;

    // Parse input to determine operations and parameters
    uint8_t op = Data[0];
    uint8_t mode = Data[1];

    // Extract n_entries from input if possible
    if (Size >= 6) {
        n_entries = ((uint32_t)Data[2] << 24) | ((uint32_t)Data[3] << 16) | 
                    ((uint32_t)Data[4] << 8) | (uint32_t)Data[5];
        // Limit n_entries to prevent huge allocations
        if (n_entries > 65536) n_entries = 65536;
        if (n_entries < 2) n_entries = 2;
    } else {
        n_entries = 2;
    }

    // Allocate table data if we have enough size
    if (Size >= 6 + (n_entries * 2)) {
        table_data = (uint16_t*)malloc(n_entries * sizeof(uint16_t));
        if (table_data) {
            memcpy(table_data, &Data[6], n_entries * sizeof(uint16_t));
            // Normalize values to 16-bit range if needed (fuzzing may provide out-of-range)
            for (uint32_t i = 0; i < n_entries; i++) {
                if (table_data[i] > 65535) table_data[i] = 65535;
            }
        }
    }

    // Create a curve if we have table data
    if (table_data && n_entries > 0) {
        curve = cmsBuildTabulatedToneCurve16(ctx, n_entries, table_data);
    }

    // Fuzzing operations based on op byte
    switch (op % 6) {
        case 0: {
            // cmsReverseToneCurve
            if (curve) {
                reversed = cmsReverseToneCurve(curve);
                cleanup_curve(reversed);
            } else {
                // Test with NULL to ensure no crash (though it asserts, we catch via sanitizer)
                // We skip explicit NULL call to avoid immediate abort in non-sanitized builds
                // but in fuzzing with sanitizers, this is expected to be caught.
            }
            break;
        }
        case 1: {
            // cmsReverseToneCurveEx
            if (curve) {
                uint32_t samples = (Size > 7) ? (uint32_t)Data[7] : 256;
                if (samples < 2) samples = 2;
                if (samples > 65536) samples = 65536;
                reversed_ex = cmsReverseToneCurveEx(samples, curve);
                cleanup_curve(reversed_ex);
            }
            break;
        }
        case 2: {
            // cmsGetToneCurveEstimatedTable
            if (curve) {
                const cmsUInt16Number* table = cmsGetToneCurveEstimatedTable(curve);
                if (table) {
                    // Just read a few values to ensure no crash
                    if (n_entries > 0) {
                        (void)table[0];
                        if (n_entries > 1) (void)table[n_entries - 1];
                    }
                }
            }
            break;
        }
        case 3: {
            // cmsEvalToneCurve16
            if (curve) {
                uint16_t val = (Size > 8) ? (uint16_t)Data[8] : 32768;
                if (val > 65535) val = 65535;
                cmsUInt16Number result = cmsEvalToneCurve16(curve, val);
                (void)result;
            }
            break;
        }
        case 4: {
            // cmsDupToneCurve
            if (curve) {
                dup_curve = cmsDupToneCurve(curve);
                cleanup_curve(dup_curve);
            }
            break;
        }
        case 5: {
            // Combined operations
            if (curve) {
                dup_curve = cmsDupToneCurve(curve);
                if (dup_curve) {
                    reversed = cmsReverseToneCurve(dup_curve);
                    if (reversed) {
                        cmsUInt16Number* est_table = (cmsUInt16Number*)cmsGetToneCurveEstimatedTable(reversed);
                        if (est_table) {
                            (void)est_table[0];
                        }
                        cleanup_curve(reversed);
                    }
                    cleanup_curve(dup_curve);
                }
            }
            break;
        }
    }

    // Cleanup
    cleanup_curve(curve);
    free(table_data);
    cmsDeleteContext(ctx);

    return 0;
}