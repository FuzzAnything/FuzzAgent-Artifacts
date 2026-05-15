// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
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
    if (size < 2) {
        return NULL;
    }
    
    size_t n_entries = (size / 2) % 256;
    if (n_entries < 2) {
        n_entries = 2;
    }
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(n_entries * sizeof(cmsUInt16Number));
    if (!table) {
        return NULL;
    }
    
    for (size_t i = 0; i < n_entries; i++) {
        if (i * 2 + 1 < size) {
            table[i] = (data[i * 2] << 8) | data[i * 2 + 1];
        } else {
            table[i] = (uint16_t)(i * 65535 / (n_entries - 1));
        }
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, (cmsUInt32Number)n_entries, table);
    free(table);
    
    return curve;
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    cmsToneCurve* curve = create_tone_curve_from_data(Data, Size);
    if (!curve) {
        return 0;
    }
    
    cmsBool monotonic = cmsIsToneCurveMonotonic(curve);
    cmsBool multisegment = cmsIsToneCurveMultisegment(curve);
    cmsInt32Number param_type = cmsGetToneCurveParametricType(curve);
    cmsBool linear = cmsIsToneCurveLinear(curve);
    cmsBool descending = cmsIsToneCurveDescending(curve);
    
    double lambda = 1.0;
    if (Size >= sizeof(double)) {
        memcpy(&lambda, Data, sizeof(double));
        if (lambda < 0.0) lambda = 0.0;
        if (lambda > 100.0) lambda = 100.0;
    }
    
    cmsBool smooth_result = cmsSmoothToneCurve(curve, lambda);
    
    cmsFreeToneCurve(curve);
    
    return 0;
}