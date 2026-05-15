// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsGetToneCurveSegment at cmsgamma.c:1507:34 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
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
    // Try to create a tone curve from the fuzzer input
    // We'll attempt to create both table-based and parametric curves
    
    if (size < 2) return NULL;
    
    // First, try to create a table-based curve
    // Use first 2 bytes to determine table size (up to 256 entries for safety)
    size_t table_size = (data[0] % 256) + 1;
    if (table_size * 2 > size - 2) {
        table_size = (size - 2) / 2;
        if (table_size < 1) return NULL;
    }
    
    // Allocate table
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(table_size * sizeof(cmsUInt16Number));
    if (!table) return NULL;
    
    // Fill table with data
    for (size_t i = 0; i < table_size; i++) {
        if (i * 2 + 2 < size) {
            table[i] = (data[i * 2 + 1] << 8) | data[i * 2 + 2];
        } else {
            table[i] = i * 65535 / (table_size - 1); // Linear ramp as fallback
        }
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, (cmsUInt32Number)table_size, table);
    free(table);
    
    if (curve) return curve;
    
    // If table-based failed, try parametric curve
    if (size >= 2) {
        int type = data[0] % 6; // 0-5 parametric types
        double params[10] = {0};
        
        // Fill params with data (convert bytes to doubles in some way)
        for (int i = 0; i < 10 && i * 8 + 1 < (int)size; i++) {
            // Simple conversion: use byte value as parameter (scaled)
            params[i] = (double)data[i + 1] / 255.0 * 10.0;
        }
        
        return cmsBuildParametricToneCurve(NULL, type, params);
    }
    
    return NULL;
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsToneCurve* curve = create_tone_curve_from_data(Data, Size);
    if (!curve) return 0;
    
    // Test all target functions
    cmsBool monotonic = cmsIsToneCurveMonotonic(curve);
    cmsBool multisegment = cmsIsToneCurveMultisegment(curve);
    
    // Try to get segments (up to 4 segments)
    for (cmsInt32Number i = 0; i < 4; i++) {
        const cmsCurveSegment* segment = cmsGetToneCurveSegment(i, curve);
        (void)segment; // Use segment to avoid unused variable warning
    }
    
    cmsInt32Number param_type = cmsGetToneCurveParametricType(curve);
    cmsBool linear = cmsIsToneCurveLinear(curve);
    cmsBool descending = cmsIsToneCurveDescending(curve);
    
    // Use variables to avoid compiler warnings
    (void)monotonic;
    (void)multisegment;
    (void)param_type;
    (void)linear;
    (void)descending;
    
    // Clean up
    cmsFreeToneCurve(curve);
    
    return 0;
}