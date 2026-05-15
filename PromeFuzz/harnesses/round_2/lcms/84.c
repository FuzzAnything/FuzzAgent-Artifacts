// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsGetToneCurveSegment at cmsgamma.c:1507:34 in lcms2.h
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

static cmsToneCurve* create_test_tone_curve(const uint8_t* data, size_t size) {
    if (size < 4) return NULL;
    
    cmsUInt32Number nEntries = (cmsUInt32Number)(data[0] | (data[1] << 8));
    if (nEntries == 0) nEntries = 256;
    if (nEntries > 65535) nEntries = 65535;
    
    size_t needed_bytes = nEntries * sizeof(cmsUInt16Number);
    if (needed_bytes == 0) return NULL;
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(needed_bytes);
    if (!table) return NULL;
    
    size_t bytes_to_copy = (size - 2) < needed_bytes ? (size - 2) : needed_bytes;
    memcpy(table, data + 2, bytes_to_copy);
    
    for (size_t i = bytes_to_copy / sizeof(cmsUInt16Number); i < nEntries; i++) {
        table[i] = (cmsUInt16Number)(i * 65535 / (nEntries - 1));
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, table);
    free(table);
    return curve;
}

static void test_tone_curve_functions(cmsToneCurve* curve) {
    if (!curve) return;
    
    cmsBool is_multi = cmsIsToneCurveMultisegment(curve);
    cmsInt32Number param_type = cmsGetToneCurveParametricType(curve);
    cmsBool is_monotonic = cmsIsToneCurveMonotonic(curve);
    cmsBool is_descending = cmsIsToneCurveDescending(curve);
    cmsBool is_linear = cmsIsToneCurveLinear(curve);
    
    for (cmsInt32Number i = 0; i < 10; i++) {
        const cmsCurveSegment* segment = cmsGetToneCurveSegment(i, curve);
        if (!segment) break;
    }
    
    (void)is_multi;
    (void)param_type;
    (void)is_monotonic;
    (void)is_descending;
    (void)is_linear;
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsToneCurve* curve = create_test_tone_curve(Data, Size);
    if (curve) {
        test_tone_curve_functions(curve);
        cmsFreeToneCurve(curve);
    }
    
    return 0;
}