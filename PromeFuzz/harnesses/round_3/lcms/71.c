// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
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

static cmsToneCurve* create_tone_curve_from_input(const uint8_t *data, size_t size) {
    if (size < 2) return NULL;
    
    // Try to create a parametric tone curve first
    if (size >= 1 + 10 * sizeof(cmsFloat64Number)) {
        cmsInt32Number type = data[0] % 8;  // Common parametric types are 0-7
        cmsFloat64Number params[10];
        
        // Copy parameters from input data
        for (int i = 0; i < 10; i++) {
            if (i * sizeof(cmsFloat64Number) + 1 < size) {
                memcpy(&params[i], data + 1 + i * sizeof(cmsFloat64Number), 
                       sizeof(cmsFloat64Number));
            } else {
                params[i] = 1.0;  // Default value
            }
        }
        
        return cmsBuildParametricToneCurve(NULL, type, params);
    }
    
    // Fall back to tabulated tone curve
    size_t table_size = size / sizeof(cmsUInt16Number);
    if (table_size < 2) return NULL;
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(table_size * sizeof(cmsUInt16Number));
    if (!table) return NULL;
    
    for (size_t i = 0; i < table_size; i++) {
        table[i] = (cmsUInt16Number)((data[i * 2] << 8) | data[i * 2 + 1]);
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, table_size, table);
    free(table);
    return curve;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create tone curve from fuzzer input
    cmsToneCurve* tone_curve = create_tone_curve_from_input(Data, Size);
    if (!tone_curve) return 0;
    
    // Test all target functions
    cmsBool result1 = cmsIsToneCurveDescending(tone_curve);
    cmsBool result2 = cmsIsToneCurveMultisegment(tone_curve);
    cmsBool result3 = cmsIsToneCurveLinear(tone_curve);
    cmsBool result4 = cmsIsToneCurveMonotonic(tone_curve);
    cmsInt32Number result5 = cmsGetToneCurveParametricType(tone_curve);
    
    // Test smoothing with lambda from input
    cmsFloat64Number lambda = 0.1;
    if (Size >= sizeof(cmsFloat64Number)) {
        memcpy(&lambda, Data, sizeof(cmsFloat64Number));
        // Ensure lambda is in reasonable range
        if (lambda < 0.0) lambda = 0.0;
        if (lambda > 100.0) lambda = 100.0;
    }
    
    cmsBool result6 = cmsSmoothToneCurve(tone_curve, lambda);
    
    // Test functions again after smoothing
    cmsBool result7 = cmsIsToneCurveDescending(tone_curve);
    cmsBool result8 = cmsIsToneCurveMultisegment(tone_curve);
    cmsBool result9 = cmsIsToneCurveLinear(tone_curve);
    cmsBool result10 = cmsIsToneCurveMonotonic(tone_curve);
    cmsInt32Number result11 = cmsGetToneCurveParametricType(tone_curve);
    
    // Clean up
    cmsFreeToneCurve(tone_curve);
    
    // Use results to avoid optimization
    volatile cmsBool dummy1 = result1;
    volatile cmsBool dummy2 = result2;
    volatile cmsBool dummy3 = result3;
    volatile cmsBool dummy4 = result4;
    volatile cmsInt32Number dummy5 = result5;
    volatile cmsBool dummy6 = result6;
    volatile cmsBool dummy7 = result7;
    volatile cmsBool dummy8 = result8;
    volatile cmsBool dummy9 = result9;
    volatile cmsBool dummy10 = result10;
    volatile cmsInt32Number dummy11 = result11;
    
    return 0;
}