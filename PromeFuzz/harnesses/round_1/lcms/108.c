// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsGetToneCurveSegment at cmsgamma.c:1507:34 in lcms2.h
// cmsGetToneCurveSegment at cmsgamma.c:1507:34 in lcms2.h
// cmsGetToneCurveSegment at cmsgamma.c:1507:34 in lcms2.h
// cmsGetToneCurveSegment at cmsgamma.c:1507:34 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcms2.h"

static cmsToneCurve* create_tone_curve_from_data(const uint8_t *Data, size_t Size) {
    // Try to create a tone curve from the fuzzer input
    // We'll attempt multiple strategies to create valid curves
    
    if (Size < 2) {
        // Not enough data for even a simple curve
        return NULL;
    }
    
    // Strategy 1: Try to create a parametric tone curve
    if (Size >= sizeof(cmsFloat64Number) * 3) {
        // Use first few bytes to determine parametric type
        cmsInt32Number param_type = Data[0] % 8; // 0-7 valid parametric types
        cmsFloat64Number params[10] = {0};
        
        // Fill params with data from input (scaled appropriately)
        size_t params_to_fill = (Size - 1) / sizeof(cmsFloat64Number);
        if (params_to_fill > 10) params_to_fill = 10;
        
        for (size_t i = 0; i < params_to_fill; i++) {
            // Convert bytes to double (simplified - actual conversion would be more careful)
            double val = 0.0;
            size_t offset = 1 + i * sizeof(cmsFloat64Number);
            if (offset + sizeof(cmsFloat64Number) <= Size) {
                memcpy(&val, Data + offset, sizeof(cmsFloat64Number));
                // Keep values in reasonable range
                params[i] = val < 0.0 ? -val : val;
                if (params[i] > 1000.0) params[i] = fmod(params[i], 1000.0);
            }
        }
        
        cmsToneCurve* curve = cmsBuildParametricToneCurve(NULL, param_type, params);
        if (curve != NULL) {
            return curve;
        }
    }
    
    // Strategy 2: Try to create a tabulated tone curve
    if (Size >= 2 * sizeof(cmsUInt16Number)) {
        size_t max_entries = 256; // Reasonable limit for fuzzing
        size_t available_entries = Size / sizeof(cmsUInt16Number);
        size_t nEntries = available_entries < max_entries ? available_entries : max_entries;
        
        if (nEntries >= 2) {
            cmsUInt16Number* table = (cmsUInt16Number*)malloc(nEntries * sizeof(cmsUInt16Number));
            if (table == NULL) {
                return NULL;
            }
            
            // Fill table with data from input
            for (size_t i = 0; i < nEntries; i++) {
                cmsUInt16Number val = 0;
                size_t offset = i * sizeof(cmsUInt16Number);
                if (offset + sizeof(cmsUInt16Number) <= Size) {
                    memcpy(&val, Data + offset, sizeof(cmsUInt16Number));
                }
                table[i] = val;
            }
            
            cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, table);
            free(table);
            return curve;
        }
    }
    
    // Strategy 3: Create a simple gamma curve as fallback
    cmsFloat64Number gamma = 1.0 + ((double)(Data[0] % 100) / 100.0); // 1.0-2.0
    return cmsBuildGamma(NULL, gamma);
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create a tone curve from the fuzzer input
    cmsToneCurve* curve = create_tone_curve_from_data(Data, Size);
    if (curve == NULL) {
        return 0;
    }
    
    // Test all the target API functions
    cmsBool result1 = cmsIsToneCurveDescending(curve);
    (void)result1; // Use result to avoid unused variable warning
    
    cmsBool result2 = cmsIsToneCurveLinear(curve);
    (void)result2;
    
    cmsInt32Number result3 = cmsGetToneCurveParametricType(curve);
    (void)result3;
    
    cmsBool result4 = cmsIsToneCurveMonotonic(curve);
    (void)result4;
    
    cmsBool result5 = cmsIsToneCurveMultisegment(curve);
    (void)result5;
    
    // Test cmsGetToneCurveSegment with various indices
    const cmsCurveSegment* segment = NULL;
    
    // Try index 0 (should work if curve has segments)
    segment = cmsGetToneCurveSegment(0, curve);
    (void)segment;
    
    // Try index 1 (might work for multi-segment curves)
    segment = cmsGetToneCurveSegment(1, curve);
    (void)segment;
    
    // Try a larger index (likely out of bounds)
    segment = cmsGetToneCurveSegment(100, curve);
    (void)segment;
    
    // Try negative index (should return NULL)
    segment = cmsGetToneCurveSegment(-1, curve);
    (void)segment;
    
    // Clean up
    cmsFreeToneCurve(curve);
    
    return 0;
}