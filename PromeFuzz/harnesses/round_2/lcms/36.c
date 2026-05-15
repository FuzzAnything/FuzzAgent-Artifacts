// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>
#include <math.h>

static cmsToneCurve* create_tone_curve_from_data(const uint8_t* data, size_t size) {
    if (size < 2) {
        return NULL;
    }
    
    size_t num_entries = 256;
    if (size < num_entries * sizeof(cmsUInt16Number)) {
        num_entries = size / sizeof(cmsUInt16Number);
        if (num_entries < 2) {
            return NULL;
        }
    }
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(num_entries * sizeof(cmsUInt16Number));
    if (!table) {
        return NULL;
    }
    
    for (size_t i = 0; i < num_entries; i++) {
        table[i] = (cmsUInt16Number)((data[i % size] << 8) | data[(i + 1) % size]);
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, (cmsUInt32Number)num_entries, table);
    free(table);
    return curve;
}

static cmsToneCurve* create_parametric_curve_from_data(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsInt32Number) + 10 * sizeof(cmsFloat64Number)) {
        return NULL;
    }
    
    cmsInt32Number type;
    memcpy(&type, data, sizeof(cmsInt32Number));
    type = type % 10;
    
    cmsFloat64Number params[10];
    for (int i = 0; i < 10; i++) {
        memcpy(&params[i], data + sizeof(cmsInt32Number) + i * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
    }
    
    return cmsBuildParametricToneCurve(NULL, type, params);
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    cmsToneCurve* curve = NULL;
    
    if (Data[0] % 2 == 0) {
        curve = create_tone_curve_from_data(Data, Size);
    } else {
        curve = create_parametric_curve_from_data(Data, Size);
    }
    
    if (!curve) {
        return 0;
    }
    
    cmsBool is_multisegment = cmsIsToneCurveMultisegment(curve);
    (void)is_multisegment;
    
    cmsInt32Number param_type = cmsGetToneCurveParametricType(curve);
    (void)param_type;
    
    cmsBool is_monotonic = cmsIsToneCurveMonotonic(curve);
    (void)is_monotonic;
    
    cmsBool is_descending = cmsIsToneCurveDescending(curve);
    (void)is_descending;
    
    cmsBool is_linear = cmsIsToneCurveLinear(curve);
    (void)is_linear;
    
    cmsFloat64Number lambda = 0.1;
    if (Size >= sizeof(cmsFloat64Number)) {
        memcpy(&lambda, Data, sizeof(cmsFloat64Number));
        lambda = fabs(lambda);
    }
    
    cmsBool smooth_success = cmsSmoothToneCurve(curve, lambda);
    (void)smooth_success;
    
    cmsFreeToneCurve(curve);
    
    return 0;
}