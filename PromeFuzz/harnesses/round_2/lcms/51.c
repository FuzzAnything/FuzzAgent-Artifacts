// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
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
#include "/promefuzz/database/lcms/latest/code/include/lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_test_curve_from_data(const uint8_t* data, size_t size, cmsContext ctx) {
    if (size < 8) return NULL;
    
    cmsUInt32Number nEntries = *(const cmsUInt32Number*)data % 256;
    if (nEntries < 2) nEntries = 2;
    
    size_t floats_needed = nEntries * sizeof(cmsFloat32Number);
    if (floats_needed > size - 4) return NULL;
    
    const cmsFloat32Number* values = (const cmsFloat32Number*)(data + 4);
    return cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
}

static cmsToneCurve* create_parametric_curve(cmsContext ctx) {
    cmsFloat64Number params[10] = {2.4, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    return cmsBuildParametricToneCurve(ctx, 1, params);
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsToneCurve* curve1 = NULL;
    cmsToneCurve* curve2 = NULL;
    
    curve1 = create_test_curve_from_data(Data, Size, ctx);
    if (!curve1) {
        curve1 = create_parametric_curve(ctx);
    }
    
    if (curve1) {
        cmsFloat32Number eval_input = *(const cmsFloat32Number*)Data;
        cmsEvalToneCurveFloat(curve1, eval_input);
        
        cmsToneCurve* dup_curve = cmsDupToneCurve(curve1);
        if (dup_curve) {
            cmsEvalToneCurveFloat(dup_curve, eval_input);
            cmsFreeToneCurve(dup_curve);
        }
        
        cmsUInt32Number nSamples = *(const cmsUInt32Number*)Data % 4096;
        if (nSamples < 2) nSamples = 2;
        
        cmsToneCurve* rev_curve_ex = cmsReverseToneCurveEx(nSamples, curve1);
        if (rev_curve_ex) {
            cmsEvalToneCurveFloat(rev_curve_ex, eval_input);
            cmsFreeToneCurve(rev_curve_ex);
        }
        
        cmsToneCurve* rev_curve = cmsReverseToneCurve(curve1);
        if (rev_curve) {
            cmsEvalToneCurveFloat(rev_curve, eval_input);
            cmsFreeToneCurve(rev_curve);
        }
        
        if (Size > 20) {
            curve2 = create_test_curve_from_data(Data + Size/2, Size/2, ctx);
            if (!curve2) {
                curve2 = create_parametric_curve(ctx);
            }
            
            if (curve2) {
                cmsUInt32Number nPoints = *(const cmsUInt32Number*)(Data + 4) % 4096;
                if (nPoints < 2) nPoints = 2;
                
                cmsToneCurve* joined = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
                if (joined) {
                    cmsEvalToneCurveFloat(joined, eval_input);
                    cmsFreeToneCurve(joined);
                }
                cmsFreeToneCurve(curve2);
            }
        }
        
        cmsFreeToneCurve(curve1);
    }
    
    cmsDeleteContext(ctx);
    return 0;
}