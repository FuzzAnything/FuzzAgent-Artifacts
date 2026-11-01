// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
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

static void cleanup_curves(cmsToneCurve **curves, int count) {
    for (int i = 0; i < count; i++) {
        if (curves[i] != NULL) {
            cmsFreeToneCurve(curves[i]);
            curves[i] = NULL;
        }
    }
}

static cmsToneCurve* create_curve_from_fuzz_data(const uint8_t *Data, size_t Size, cmsContext ContextID) {
    if (Size < 2) return NULL;

    // Extract number of entries from first 4 bytes (little endian)
    cmsUInt32Number nEntries = 0;
    if (Size >= 4) {
        nEntries = (cmsUInt32Number)Data[0] | ((cmsUInt32Number)Data[1] << 8) | 
                   ((cmsUInt32Number)Data[2] << 16) | ((cmsUInt32Number)Data[3] << 24);
    }

    // Clamp entries to reasonable range to avoid memory exhaustion
    if (nEntries == 0 || nEntries > 10000) {
        nEntries = 256; // Default to 256 entries if invalid
    }

    // Ensure we have enough data for the table
    size_t required_size = nEntries * sizeof(cmsUInt16Number);
    if (Size < 4 + required_size) {
        // Generate synthetic data if input is too short
        cmsUInt16Number *values = (cmsUInt16Number*)malloc(required_size);
        if (!values) return NULL;
        for (cmsUInt32Number i = 0; i < nEntries; i++) {
            values[i] = (cmsUInt16Number)((i * 65535) / (nEntries - 1));
        }
        cmsToneCurve *curve = cmsBuildTabulatedToneCurve16(ContextID, nEntries, values);
        free(values);
        return curve;
    }

    // Use provided data
    const cmsUInt16Number *values = (const cmsUInt16Number*)(Data + 4);
    return cmsBuildTabulatedToneCurve16(ContextID, nEntries, values);
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    cmsToneCurve *curves[5] = {NULL, NULL, NULL, NULL, NULL};
    int curve_count = 0;

    // Create initial curve from fuzz data
    cmsToneCurve *initial_curve = create_curve_from_fuzz_data(Data, Size, ContextID);
    if (initial_curve != NULL) {
        curves[curve_count++] = initial_curve;
    }

    // Test cmsDupToneCurve
    if (curve_count > 0) {
        cmsToneCurve *dup_curve = cmsDupToneCurve(curves[0]);
        if (dup_curve != NULL) {
            curves[curve_count++] = dup_curve;
        }
    }

    // Test cmsReverseToneCurve
    if (curve_count > 0) {
        cmsToneCurve *rev_curve = cmsReverseToneCurve(curves[0]);
        if (rev_curve != NULL) {
            curves[curve_count++] = rev_curve;
        }
    }

    // Test cmsReverseToneCurveEx with different sample counts
    if (curve_count > 0) {
        cmsUInt32Number sample_counts[] = {256, 1024, 4096};
        for (int i = 0; i < 3; i++) {
            cmsToneCurve *rev_ex_curve = cmsReverseToneCurveEx(sample_counts[i], curves[0]);
            if (rev_ex_curve != NULL && curve_count < 5) {
                curves[curve_count++] = rev_ex_curve;
            }
        }
    }

    // Test cmsGetToneCurveEstimatedTable
    if (curve_count > 0) {
        const cmsUInt16Number *table = cmsGetToneCurveEstimatedTable(curves[0]);
        if (table != NULL) {
            // Just access the table to ensure it's valid
            (void)table[0];
        }
    }

    // Test cmsEvalToneCurve16 with various input values
    if (curve_count > 0) {
        cmsUInt16Number test_values[] = {0, 1, 32768, 65535};
        for (int i = 0; i < 4; i++) {
            cmsUInt16Number result = cmsEvalToneCurve16(curves[0], test_values[i]);
            (void)result; // Use the result to prevent unused variable warning
        }
    }

    // Cleanup
    cleanup_curves(curves, curve_count);
    cmsDeleteContext(ContextID);

    return 0;
}