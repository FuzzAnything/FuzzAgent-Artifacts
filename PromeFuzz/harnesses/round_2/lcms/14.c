// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
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

static cmsToneCurve* create_test_tone_curve(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 256) return NULL;
    
    cmsUInt16Number table[256];
    for (int i = 0; i < 256; i++) {
        table[i] = (cmsUInt16Number)((data[i] << 8) | data[(i + 128) % size]);
    }
    
    return cmsBuildTabulatedToneCurve16(ctx, 256, table);
}

static cmsToneCurve* create_parametric_curve(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 10) return NULL;
    
    cmsFloat64Number params[10];
    for (int i = 0; i < 10 && i < size; i++) {
        params[i] = (cmsFloat64Number)data[i] / 255.0;
    }
    
    return cmsBuildParametricToneCurve(ctx, 1, params);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsToneCurve* curve1 = NULL;
    cmsToneCurve* curve2 = NULL;
    cmsToneCurve* curve3 = NULL;
    
    if (Data[0] % 2 == 0) {
        curve1 = create_test_tone_curve(ctx, Data, Size);
    } else {
        curve1 = create_parametric_curve(ctx, Data, Size);
    }
    
    if (curve1) {
        cmsToneCurve* dup_curve = cmsDupToneCurve(curve1);
        if (dup_curve) {
            cmsFreeToneCurve(dup_curve);
        }
        
        cmsUInt32Number n_samples = (Data[1] << 8) | Data[2];
        if (n_samples < 2) n_samples = 256;
        
        cmsToneCurve* rev_curve_ex = cmsReverseToneCurveEx(n_samples, curve1);
        if (rev_curve_ex) {
            cmsFreeToneCurve(rev_curve_ex);
        }
        
        cmsToneCurve* rev_curve = cmsReverseToneCurve(curve1);
        if (rev_curve) {
            cmsFreeToneCurve(rev_curve);
        }
        
        if (Size > 512) {
            curve2 = create_test_tone_curve(ctx, Data + 256, Size - 256);
        } else if (Size > 128) {
            curve2 = create_parametric_curve(ctx, Data + 128, Size - 128);
        }
        
        if (curve2) {
            cmsUInt32Number n_points = (Data[3] << 8) | Data[4];
            if (n_points < 2) n_points = 256;
            
            cmsToneCurve* joined_curve = cmsJoinToneCurve(ctx, curve1, curve2, n_points);
            if (joined_curve) {
                cmsFreeToneCurve(joined_curve);
            }
            
            if (Size > 768) {
                curve3 = create_test_tone_curve(ctx, Data + 512, Size - 512);
            } else if (Size > 256) {
                curve3 = create_parametric_curve(ctx, Data + 256, Size - 256);
            }
            
            if (curve3) {
                cmsToneCurve* triple[3] = {curve1, curve2, curve3};
                cmsFreeToneCurveTriple(triple);
                curve1 = curve2 = curve3 = NULL;
            } else {
                cmsFreeToneCurve(curve2);
                curve2 = NULL;
            }
        }
        
        if (curve1) {
            cmsFreeToneCurve(curve1);
        }
    }
    
    if (curve2) {
        cmsFreeToneCurve(curve2);
    }
    
    if (curve3) {
        cmsFreeToneCurve(curve3);
    }
    
    cmsDeleteContext(ctx);
    return 0;
}