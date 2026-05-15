// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
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

static cmsToneCurve* create_tone_curve_from_data(const uint8_t* data, size_t size) {
    if (size < 4) return NULL;
    
    // Use first 2 bytes to determine number of entries (minimum 2)
    cmsUInt32Number nEntries = (data[0] << 8) | data[1];
    if (nEntries < 2) nEntries = 2;
    if (nEntries > 65535) nEntries = 65535;
    
    // Calculate how many bytes we need for the table
    size_t table_bytes_needed = nEntries * sizeof(cmsUInt16Number);
    
    // If we don't have enough data, create a simple linear curve
    if (size < 2 + table_bytes_needed) {
        // Create a simple parametric curve (type 1: linear)
        cmsFloat64Number params[10] = {1.0};
        return cmsBuildParametricToneCurve(NULL, 1, params);
    }
    
    // Allocate and fill table
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(table_bytes_needed);
    if (!table) return NULL;
    
    // Copy data to table, ensuring values are within valid range
    for (cmsUInt32Number i = 0; i < nEntries; i++) {
        size_t offset = 2 + i * sizeof(cmsUInt16Number);
        if (offset + 1 < size) {
            table[i] = (data[offset] << 8) | data[offset + 1];
        } else {
            // Fill with linear ramp if we run out of data
            table[i] = (cmsUInt16Number)((i * 65535.0) / (nEntries - 1));
        }
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, table);
    free(table);
    return curve;
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a tone curve from fuzz data
    cmsToneCurve* curve = create_tone_curve_from_data(Data, Size);
    if (!curve) return 0;
    
    // Test all target functions
    cmsBool monotonic = cmsIsToneCurveMonotonic(curve);
    cmsBool multisegment = cmsIsToneCurveMultisegment(curve);
    cmsInt32Number param_type = cmsGetToneCurveParametricType(curve);
    cmsBool linear = cmsIsToneCurveLinear(curve);
    cmsBool descending = cmsIsToneCurveDescending(curve);
    
    // Test smoothing with lambda derived from fuzz data
    cmsFloat64Number lambda = 0.1; // Default value
    if (Size >= 9) {
        // Use a byte from data to influence lambda (range 0.0 to 1.0)
        lambda = (Data[Size - 1] / 255.0);
    }
    cmsBool smooth_result = cmsSmoothToneCurve(curve, lambda);
    
    // Clean up
    cmsFreeToneCurve(curve);
    
    // Use the results to avoid dead code elimination
    (void)monotonic;
    (void)multisegment;
    (void)param_type;
    (void)linear;
    (void)descending;
    (void)smooth_result;
    
    return 0;
}