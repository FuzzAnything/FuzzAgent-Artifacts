// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// _cmsAdjustEndianess16 at cmsplugin.c:37:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_curve(cmsToneCurve* curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

static cmsToneCurve* create_curve_from_fuzz_data(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return NULL;
    }

    // Use first two bytes to determine number of entries (minimum 2)
    cmsUInt32Number nEntries = (Data[0] << 8) | Data[1];
    if (nEntries < 2) nEntries = 2;
    
    // Limit entries to prevent excessive memory allocation
    if (nEntries > 65536) nEntries = 65536;

    // Check if we have enough data for the entries
    size_t required_bytes = nEntries * sizeof(cmsUInt16Number);
    if (Size < 2 + required_bytes) {
        // If not enough data, create a minimal curve with random values
        cmsUInt16Number *values = (cmsUInt16Number*)malloc(2 * sizeof(cmsUInt16Number));
        if (!values) return NULL;
        values[0] = 0;
        values[1] = 65535;
        cmsToneCurve *curve = cmsBuildTabulatedToneCurve16(NULL, 2, values);
        free(values);
        return curve;
    }

    // Allocate and fill values from fuzz data
    cmsUInt16Number *values = (cmsUInt16Number*)malloc(nEntries * sizeof(cmsUInt16Number));
    if (!values) return NULL;

    for (cmsUInt32Number i = 0; i < nEntries; i++) {
        size_t offset = 2 + (i * sizeof(cmsUInt16Number));
        values[i] = (Data[offset] << 8) | Data[offset + 1];
    }

    cmsToneCurve *curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, values);
    free(values);
    return curve;
}

int LLVMFuzzerTestOneInput_169(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsToneCurve *curve = NULL;
    cmsToneCurve *curve2 = NULL;

    // Create primary curve from fuzz data
    curve = create_curve_from_fuzz_data(Data, Size);

    // Test cmsIsToneCurveLinear
    if (curve != NULL) {
        cmsBool linear = cmsIsToneCurveLinear(curve);
        (void)linear; // Suppress unused variable warning
    }

    // Test cmsGetToneCurveEstimatedTable
    if (curve != NULL) {
        const cmsUInt16Number *table = cmsGetToneCurveEstimatedTable(curve);
        if (table != NULL) {
            // Read first few entries to trigger memory access
            (void)table[0];
            if (cmsIsToneCurveDescending(curve)) {
                // Additional logic path
                (void)table[1];
            }
        }
    }

    // Test cmsIsToneCurveDescending
    if (curve != NULL) {
        cmsBool descending = cmsIsToneCurveDescending(curve);
        (void)descending;
    }

    // Test _cmsAdjustEndianess16 with various values from input
    if (Size >= 2) {
        cmsUInt16Number word = (Data[0] << 8) | Data[1];
        cmsUInt16Number swapped = _cmsAdjustEndianess16(word);
        (void)swapped;
    }

    // Test cmsEvalToneCurve16 with various input values
    if (curve != NULL) {
        cmsUInt16Number input_val = 0;
        if (Size >= 2) {
            input_val = (Data[0] << 8) | Data[1];
        }
        cmsUInt16Number output_val = cmsEvalToneCurve16(curve, input_val);
        (void)output_val;

        // Test with boundary values
        cmsUInt16Number output_val_0 = cmsEvalToneCurve16(curve, 0);
        cmsUInt16Number output_val_max = cmsEvalToneCurve16(curve, 65535);
        (void)output_val_0;
        (void)output_val_max;
    }

    // Create a second curve to test more states
    if (Size >= 4) {
        // Ensure divisor is never zero
        size_t data_len = Size - 4;
        if (data_len > 0) {
            cmsUInt32Number nEntries2 = ((Data[2] << 8) | Data[3]) % 1000 + 2;
            cmsUInt16Number *values2 = (cmsUInt16Number*)malloc(nEntries2 * sizeof(cmsUInt16Number));
            if (values2) {
                for (cmsUInt32Number i = 0; i < nEntries2; i++) {
                    // Safe modulo arithmetic
                    size_t idx1 = (i % data_len) + 4;
                    size_t idx2 = ((i + 1) % data_len) + 4;
                    values2[i] = (Data[idx1] << 8) | Data[idx2];
                }
                curve2 = cmsBuildTabulatedToneCurve16(NULL, nEntries2, values2);
                free(values2);
            }
        }
    }

    // Test with second curve if created
    if (curve2 != NULL) {
        cmsBool linear2 = cmsIsToneCurveLinear(curve2);
        cmsBool descending2 = cmsIsToneCurveDescending(curve2);
        const cmsUInt16Number *table2 = cmsGetToneCurveEstimatedTable(curve2);
        cmsUInt16Number eval2 = cmsEvalToneCurve16(curve2, 32768);
        
        (void)linear2;
        (void)descending2;
        (void)table2;
        (void)eval2;
    }

    // Cleanup
    cleanup_curve(curve);
    cleanup_curve(curve2);

    return 0;
}