// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_tone_curve_from_data(const uint8_t *data, size_t size) {
    if (size < 2) return NULL;
    
    // Determine number of entries from first 2 bytes (max 65535, but limit for sanity)
    size_t nEntries = (data[0] << 8) | data[1];
    if (nEntries == 0 || nEntries > 65535) nEntries = 256;
    
    // Ensure we have enough data for the table
    size_t needed_bytes = nEntries * sizeof(cmsUInt16Number);
    if (size < 2 + needed_bytes) {
        // Not enough data, create a simple parametric curve instead
        cmsFloat64Number params[10] = {0};
        for (int i = 0; i < 10 && (i * sizeof(cmsFloat64Number)) < (size - 2); i++) {
            memcpy(&params[i], data + 2 + i * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
        }
        return cmsBuildParametricToneCurve(NULL, 1, params);
    }
    
    // Create table-based tone curve
    cmsUInt16Number *table = (cmsUInt16Number*)malloc(needed_bytes);
    if (!table) return NULL;
    
    for (size_t i = 0; i < nEntries; i++) {
        size_t offset = 2 + i * sizeof(cmsUInt16Number);
        if (offset + sizeof(cmsUInt16Number) <= size) {
            table[i] = (data[offset] << 8) | data[offset + 1];
        } else {
            table[i] = (cmsUInt16Number)((i * 65535) / (nEntries - 1));
        }
    }
    
    cmsToneCurve *curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, table);
    free(table);
    return curve;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create tone curve from fuzzer input
    cmsToneCurve *curve = create_tone_curve_from_data(Data, Size);
    if (!curve) return 0;
    
    // Test all target API functions
    cmsBool result1 = cmsIsToneCurveLinear(curve);
    cmsBool result2 = cmsIsToneCurveDescending(curve);
    cmsInt32Number result3 = cmsGetToneCurveParametricType(curve);
    cmsBool result4 = cmsIsToneCurveMonotonic(curve);
    cmsBool result5 = cmsIsToneCurveMultisegment(curve);
    
    // Test cmsSmoothToneCurve with lambda from input data
    cmsFloat64Number lambda = 1.0;
    if (Size >= sizeof(cmsFloat64Number)) {
        memcpy(&lambda, Data, sizeof(cmsFloat64Number));
        // Clamp lambda to reasonable range
        if (lambda < 0.0) lambda = 0.0;
        if (lambda > 100.0) lambda = 100.0;
    }
    cmsBool result6 = cmsSmoothToneCurve(curve, lambda);
    
    // Use results to avoid dead code elimination
    (void)result1;
    (void)result2;
    (void)result3;
    (void)result4;
    (void)result5;
    (void)result6;
    
    // Clean up
    cmsFreeToneCurve(curve);
    
    return 0;
}