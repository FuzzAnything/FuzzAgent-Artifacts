// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_tabulated_curve_from_data(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsFloat32Number)) return NULL;
    
    size_t max_entries = size / sizeof(cmsFloat32Number);
    if (max_entries > 4096) max_entries = 4096;
    
    size_t nEntries = (data[0] % 256) + 1;
    if (nEntries > max_entries) nEntries = max_entries;
    
    cmsFloat32Number* values = (cmsFloat32Number*)malloc(nEntries * sizeof(cmsFloat32Number));
    if (!values) return NULL;
    
    for (size_t i = 0; i < nEntries; i++) {
        uint32_t val;
        if (i * sizeof(cmsFloat32Number) + sizeof(cmsFloat32Number) <= size) {
            memcpy(&val, data + i * sizeof(cmsFloat32Number), sizeof(uint32_t));
        } else {
            val = i * 1000;
        }
        values[i] = (cmsFloat32Number)(val % 10000) / 10000.0f;
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
    free(values);
    return curve;
}

static cmsToneCurve* create_parametric_curve_from_data(cmsContext ctx, const uint8_t* data, size_t size) {
    cmsInt32Number type = (data[0] % 5) + 1;
    cmsFloat64Number params[10];
    
    for (int i = 0; i < 10; i++) {
        if (i * sizeof(cmsFloat64Number) + sizeof(cmsFloat64Number) <= size) {
            memcpy(&params[i], data + i * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
        } else {
            params[i] = (cmsFloat64Number)i * 0.1;
        }
    }
    
    return cmsBuildParametricToneCurve(ctx, type, params);
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsToneCurve* curve1 = create_tabulated_curve_from_data(ctx, Data, Size);
    cmsToneCurve* curve2 = create_parametric_curve_from_data(ctx, Data, Size);
    
    if (curve1) {
        cmsToneCurve* reversed = cmsReverseToneCurve(curve1);
        if (reversed) {
            cmsUInt32Number entries = cmsGetToneCurveEstimatedTableEntries(reversed);
            (void)entries;
            cmsFreeToneCurve(reversed);
        }
        
        cmsUInt32Number nResultSamples = (Data[0] % 1024) + 1;
        cmsToneCurve* reversedEx = cmsReverseToneCurveEx(nResultSamples, curve1);
        if (reversedEx) {
            cmsFreeToneCurve(reversedEx);
        }
        
        if (curve2) {
            cmsUInt32Number nPoints = (Data[1] % 1024) + 1;
            cmsToneCurve* joined = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
            if (joined) {
                cmsFreeToneCurve(joined);
            }
        }
        
        cmsUInt32Number estimated = cmsGetToneCurveEstimatedTableEntries(curve1);
        (void)estimated;
        
        cmsFreeToneCurve(curve1);
    }
    
    if (curve2) {
        cmsUInt32Number estimated = cmsGetToneCurveEstimatedTableEntries(curve2);
        (void)estimated;
        cmsFreeToneCurve(curve2);
    }
    
    cmsDeleteContext(ctx);
    return 0;
}