// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
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

static cmsToneCurve* create_tone_curve_from_fuzzed_data(const uint8_t* data, size_t size) {
    if (size < 4) {
        return NULL;
    }
    
    size_t table_entries = (size / 2) % 4096;
    if (table_entries < 2) {
        table_entries = 2;
    }
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(table_entries * sizeof(cmsUInt16Number));
    if (!table) {
        return NULL;
    }
    
    for (size_t i = 0; i < table_entries && i * 2 < size; i++) {
        table[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, table_entries, table);
    free(table);
    
    return curve;
}

int LLVMFuzzerTestOneInput_121(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    cmsToneCurve* curve = create_tone_curve_from_fuzzed_data(Data, Size);
    if (!curve) {
        return 0;
    }
    
    cmsBool result;
    
    result = cmsIsToneCurveDescending(curve);
    result = cmsIsToneCurveMultisegment(curve);
    result = cmsIsToneCurveLinear(curve);
    result = cmsIsToneCurveMonotonic(curve);
    
    cmsInt32Number param_type = cmsGetToneCurveParametricType(curve);
    
    for (cmsInt32Number i = 0; i < 5; i++) {
        const cmsCurveSegment* segment = cmsGetToneCurveSegment(i, curve);
        (void)segment;
    }
    
    cmsFreeToneCurve(curve);
    
    return 0;
}