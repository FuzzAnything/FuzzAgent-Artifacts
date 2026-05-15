// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
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

static cmsToneCurve* create_test_curve_from_data(const uint8_t* data, size_t size, cmsContext ctx) {
    if (size < 8) return NULL;
    
    size_t max_entries = (size - 4) / sizeof(cmsFloat32Number);
    if (max_entries < 2) return NULL;
    
    cmsUInt32Number nEntries = data[0] % (max_entries + 1);
    if (nEntries < 2) nEntries = 2;
    
    cmsFloat32Number* values = (cmsFloat32Number*)malloc(nEntries * sizeof(cmsFloat32Number));
    if (!values) return NULL;
    
    for (cmsUInt32Number i = 0; i < nEntries; i++) {
        uint32_t temp = 0;
        size_t offset = (i * 4) % (size - 4);
        memcpy(&temp, data + 4 + offset, 4);
        values[i] = (cmsFloat32Number)(temp % 1001) / 1000.0f;
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
    free(values);
    return curve;
}

static cmsToneCurve* create_simple_gamma_curve(cmsContext ctx) {
    cmsFloat32Number values[2] = {0.0f, 1.0f};
    return cmsBuildTabulatedToneCurveFloat(ctx, 2, values);
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsToneCurve* curve1 = create_test_curve_from_data(Data, Size, ctx);
    if (!curve1) {
        curve1 = create_simple_gamma_curve(ctx);
        if (!curve1) {
            cmsDeleteContext(ctx);
            return 0;
        }
    }
    
    cmsToneCurve* curve2 = NULL;
    if (Size >= 12) {
        curve2 = create_test_curve_from_data(Data + Size/2, Size/2, ctx);
    }
    if (!curve2) {
        curve2 = create_simple_gamma_curve(ctx);
    }
    
    cmsUInt32Number table_entries = cmsGetToneCurveEstimatedTableEntries(curve1);
    (void)table_entries;
    
    cmsToneCurve* reversed = cmsReverseToneCurve(curve1);
    if (reversed) {
        cmsFreeToneCurve(reversed);
    }
    
    cmsUInt32Number nSamples = (Data[1] % 256) + 2;
    cmsToneCurve* reversed_ex = cmsReverseToneCurveEx(nSamples, curve1);
    if (reversed_ex) {
        cmsFreeToneCurve(reversed_ex);
    }
    
    cmsUInt32Number nPoints = (Data[2] % 256) + 2;
    cmsToneCurve* joined = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
    if (joined) {
        cmsFreeToneCurve(joined);
    }
    
    if (curve1) cmsFreeToneCurve(curve1);
    if (curve2) cmsFreeToneCurve(curve2);
    cmsDeleteContext(ctx);
    
    return 0;
}