// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
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
    
    size_t max_entries = 256;
    if (size/4 < max_entries) {
        max_entries = size/4;
    }
    
    cmsFloat32Number* values = (cmsFloat32Number*)malloc(max_entries * sizeof(cmsFloat32Number));
    if (!values) return NULL;
    
    for (size_t i = 0; i < max_entries; i++) {
        uint32_t temp;
        memcpy(&temp, data + (i % (size/4)) * 4, 4);
        values[i] = (cmsFloat32Number)(temp % 10000) / 10000.0f;
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurveFloat(ctx, max_entries, values);
    free(values);
    return curve;
}

static cmsToneCurve* create_simple_curve(cmsContext ctx) {
    cmsFloat32Number values[5] = {0.0f, 0.25f, 0.5f, 0.75f, 1.0f};
    return cmsBuildTabulatedToneCurveFloat(ctx, 5, values);
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    uint32_t seed;
    memcpy(&seed, Data, 4);
    srand(seed);
    
    cmsToneCurve* curve1 = create_test_curve_from_data(Data, Size, ctx);
    cmsToneCurve* curve2 = create_test_curve_from_data(Data + 4, Size > 4 ? Size - 4 : 0, ctx);
    cmsToneCurve* simple_curve = create_simple_curve(ctx);
    
    if (curve1) {
        cmsToneCurve* reversed = cmsReverseToneCurve(curve1);
        if (reversed) {
            cmsFloat32Number test_val = (cmsFloat32Number)(seed % 10000) / 10000.0f;
            cmsEvalToneCurveFloat(reversed, test_val);
            cmsFreeToneCurve(reversed);
        }
        
        cmsToneCurve* dup = cmsDupToneCurve(curve1);
        if (dup) {
            cmsFreeToneCurve(dup);
        }
        
        uint32_t nResultSamples = (seed % 1000) + 1;
        cmsToneCurve* reversed_ex = cmsReverseToneCurveEx(nResultSamples, curve1);
        if (reversed_ex) {
            cmsFreeToneCurve(reversed_ex);
        }
    }
    
    if (curve1 && curve2) {
        uint32_t nPoints = (seed % 1000) + 2;
        cmsToneCurve* joined = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
        if (joined) {
            cmsFloat32Number test_val = (cmsFloat32Number)((seed >> 8) % 10000) / 10000.0f;
            cmsEvalToneCurveFloat(joined, test_val);
            cmsFreeToneCurve(joined);
        }
    }
    
    if (simple_curve) {
        cmsFloat32Number test_val = (cmsFloat32Number)((seed >> 16) % 10000) / 10000.0f;
        cmsEvalToneCurveFloat(simple_curve, test_val);
        cmsFreeToneCurve(simple_curve);
    }
    
    if (curve1) cmsFreeToneCurve(curve1);
    if (curve2) cmsFreeToneCurve(curve2);
    
    cmsDeleteContext(ctx);
    return 0;
}