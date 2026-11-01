// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_curve(cmsToneCurve *curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

static cmsToneCurve* create_test_curve_from_data(const uint8_t *data, size_t size, cmsContext ctx) {
    if (size < 2) return NULL;
    
    // Use first two bytes to determine number of entries (min 2, max 65535)
    cmsUInt32Number nEntries = 2;
    if (size >= 2) {
        nEntries = (data[0] << 8) | data[1];
        if (nEntries < 2) nEntries = 2;
        if (nEntries > 65535) nEntries = 65535;
    }
    
    // Ensure we don't read past buffer
    if (nEntries > size - 2) {
        nEntries = size - 2;
        if (nEntries < 2) return NULL;
    }
    
    cmsUInt16Number *values = (cmsUInt16Number*)malloc(nEntries * sizeof(cmsUInt16Number));
    if (!values) return NULL;
    
    // Fill values from input data
    for (cmsUInt32Number i = 0; i < nEntries; i++) {
        size_t offset = 2 + (i * 2);
        if (offset + 2 <= size) {
            values[i] = (data[offset] << 8) | data[offset + 1];
        } else {
            values[i] = 0;
        }
    }
    
    cmsToneCurve *curve = cmsBuildTabulatedToneCurve16(ctx, nEntries, values);
    free(values);
    return curve;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create a tone curve from input data
    cmsToneCurve *originalCurve = create_test_curve_from_data(Data, Size, ctx);
    
    if (!originalCurve) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Test cmsDupToneCurve
    cmsToneCurve *dupCurve = cmsDupToneCurve(originalCurve);
    if (dupCurve) {
        // Test cmsEvalToneCurve16 on duplicated curve
        if (Size >= 2) {
            cmsUInt16Number val = (Data[0] << 8) | Data[1];
            cmsEvalToneCurve16(dupCurve, val);
        }
        
        // Test cmsGetToneCurveEstimatedTable on duplicated curve
        const cmsUInt16Number *table = cmsGetToneCurveEstimatedTable(dupCurve);
        if (table) {
            // Just access first few elements to ensure validity
            if (Size > 0) {
                (void)table[0];
                if (Size > 1) (void)table[1];
            }
        }
        
        // Test cmsReverseToneCurve on duplicated curve
        cmsToneCurve *reversedDup = cmsReverseToneCurve(dupCurve);
        if (reversedDup) {
            if (Size >= 2) {
                cmsUInt16Number val = (Data[0] << 8) | Data[1];
                cmsEvalToneCurve16(reversedDup, val);
            }
            cmsFreeToneCurve(reversedDup);
        }
        
        cmsFreeToneCurve(dupCurve);
    }
    
    // Test cmsEvalToneCurve16 on original curve
    if (Size >= 2) {
        cmsUInt16Number val = (Data[0] << 8) | Data[1];
        cmsEvalToneCurve16(originalCurve, val);
    }
    
    // Test cmsGetToneCurveEstimatedTable on original curve
    const cmsUInt16Number *table = cmsGetToneCurveEstimatedTable(originalCurve);
    if (table) {
        if (Size > 0) {
            (void)table[0];
            if (Size > 1) (void)table[1];
        }
    }
    
    // Test cmsReverseToneCurve on original curve
    cmsToneCurve *reversedCurve = cmsReverseToneCurve(originalCurve);
    if (reversedCurve) {
        if (Size >= 2) {
            cmsUInt16Number val = (Data[0] << 8) | Data[1];
            cmsEvalToneCurve16(reversedCurve, val);
        }
        cmsFreeToneCurve(reversedCurve);
    }
    
    // Test cmsDupToneCurve with NULL (should return NULL)
    cmsToneCurve *nullDup = cmsDupToneCurve(NULL);
    (void)nullDup; // Suppress unused warning
    
    // Test cmsFreeToneCurve with NULL (should be safe)
    cmsFreeToneCurve(NULL);
    
    // Cleanup
    cmsFreeToneCurve(originalCurve);
    cmsDeleteContext(ctx);
    
    return 0;
}