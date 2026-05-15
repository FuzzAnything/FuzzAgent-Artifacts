// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
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

static cmsToneCurve* create_tone_curve_from_data(const uint8_t* data, size_t size) {
    if (size < 4) return NULL;
    
    cmsUInt32Number nEntries = (data[0] << 8) | data[1];
    if (nEntries == 0) nEntries = 256;
    if (nEntries > 65535) nEntries = 65535;
    
    size_t table_size = nEntries * sizeof(cmsUInt16Number);
    if (table_size > size - 2) return NULL;
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(table_size);
    if (!table) return NULL;
    
    for (cmsUInt32Number i = 0; i < nEntries; i++) {
        size_t offset = 2 + i * 2;
        if (offset + 1 < size) {
            table[i] = (data[offset] << 8) | data[offset + 1];
        } else {
            table[i] = i * 65535 / (nEntries - 1);
        }
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, table);
    free(table);
    return curve;
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    cmsToneCurve* curve = create_tone_curve_from_data(Data, Size);
    if (!curve) return 0;
    
    cmsBool is_multi = cmsIsToneCurveMultisegment(curve);
    cmsInt32Number param_type = cmsGetToneCurveParametricType(curve);
    cmsBool is_linear = cmsIsToneCurveLinear(curve);
    cmsBool is_descending = cmsIsToneCurveDescending(curve);
    cmsBool is_monotonic = cmsIsToneCurveMonotonic(curve);
    
    for (cmsInt32Number i = 0; i < 10; i++) {
        const cmsCurveSegment* segment = cmsGetToneCurveSegment(i, curve);
        (void)segment;
    }
    
    cmsFreeToneCurve(curve);
    
    return 0;
}