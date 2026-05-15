// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
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
    if (size < 4) {
        return NULL;
    }
    
    cmsUInt32Number nEntries = 256;
    if (size >= 512) {
        nEntries = 256;
    }
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(nEntries * sizeof(cmsUInt16Number));
    if (!table) {
        return NULL;
    }
    
    size_t bytes_to_copy = nEntries * sizeof(cmsUInt16Number);
    if (bytes_to_copy > size) {
        bytes_to_copy = size;
    }
    
    memcpy(table, data, bytes_to_copy);
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, table);
    free(table);
    
    return curve;
}

static cmsToneCurve* create_parametric_curve_from_data(const uint8_t *data, size_t size) {
    if (size < 88) {
        return NULL;
    }
    
    cmsFloat64Number params[10];
    memcpy(params, data, sizeof(params));
    
    int type = data[0] % 6;
    
    return cmsBuildParametricToneCurve(NULL, type, params);
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    cmsToneCurve* curve1 = create_tone_curve_from_data(Data, Size);
    if (curve1) {
        cmsIsToneCurveMultisegment(curve1);
        cmsGetToneCurveParametricType(curve1);
        cmsIsToneCurveLinear(curve1);
        cmsIsToneCurveDescending(curve1);
        cmsIsToneCurveMonotonic(curve1);
        
        cmsFloat64Number lambda = 1.0;
        if (Size >= sizeof(cmsFloat64Number)) {
            memcpy(&lambda, Data, sizeof(cmsFloat64Number));
        }
        cmsSmoothToneCurve(curve1, lambda);
        
        cmsFreeToneCurve(curve1);
    }
    
    cmsToneCurve* curve2 = create_parametric_curve_from_data(Data, Size);
    if (curve2) {
        cmsIsToneCurveMultisegment(curve2);
        cmsGetToneCurveParametricType(curve2);
        cmsIsToneCurveLinear(curve2);
        cmsIsToneCurveDescending(curve2);
        cmsIsToneCurveMonotonic(curve2);
        
        cmsFloat64Number lambda = 1.0;
        if (Size >= sizeof(cmsFloat64Number)) {
            memcpy(&lambda, Data, sizeof(cmsFloat64Number));
        }
        cmsSmoothToneCurve(curve2, lambda);
        
        cmsFreeToneCurve(curve2);
    }
    
    return 0;
}