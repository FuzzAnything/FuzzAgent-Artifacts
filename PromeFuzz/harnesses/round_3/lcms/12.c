// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_test_curve_from_data(const uint8_t* data, size_t size) {
    if (size < 256) {
        return NULL;
    }
    
    cmsUInt16Number table[256];
    for (int i = 0; i < 256; i++) {
        table[i] = (data[i] << 8) | data[(i + 1) % size];
    }
    
    return cmsBuildTabulatedToneCurve16(NULL, 256, table);
}

static cmsToneCurve* create_parametric_curve_from_data(const uint8_t* data, size_t size) {
    if (size < 10) {
        return NULL;
    }
    
    cmsFloat64Number params[10];
    for (int i = 0; i < 10; i++) {
        params[i] = (cmsFloat64Number)data[i % size] / 255.0;
    }
    
    int type = data[0] % 6;
    return cmsBuildParametricToneCurve(NULL, type, params);
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    cmsToneCurve* curve1 = NULL;
    cmsToneCurve* curve2 = NULL;
    cmsToneCurve* curve3 = NULL;
    
    switch (Data[0] % 3) {
        case 0:
            curve1 = create_test_curve_from_data(Data, Size);
            curve2 = create_test_curve_from_data(Data + 128, Size > 128 ? Size - 128 : 0);
            break;
        case 1:
            curve1 = create_parametric_curve_from_data(Data, Size);
            curve2 = create_parametric_curve_from_data(Data + 64, Size > 64 ? Size - 64 : 0);
            break;
        case 2:
            curve1 = create_test_curve_from_data(Data, Size);
            curve2 = create_parametric_curve_from_data(Data + 128, Size > 128 ? Size - 128 : 0);
            break;
    }
    
    if (curve1) {
        cmsFreeToneCurve(curve1);
    }
    
    if (curve1 && curve2) {
        cmsToneCurve* reversed = cmsReverseToneCurve(curve1);
        if (reversed) {
            cmsFreeToneCurve(reversed);
        }
        
        cmsUInt32Number samples = (Data[1] % 4096) + 2;
        cmsToneCurve* reversed_ex = cmsReverseToneCurveEx(samples, curve1);
        if (reversed_ex) {
            cmsFreeToneCurve(reversed_ex);
        }
        
        cmsUInt32Number points = (Data[2] % 1024) + 2;
        cmsToneCurve* joined = cmsJoinToneCurve(context, curve1, curve2, points);
        if (joined) {
            cmsFreeToneCurve(joined);
        }
        
        cmsToneCurve* dup = cmsDupToneCurve(curve1);
        if (dup) {
            cmsFreeToneCurve(dup);
        }
        
        curve3 = create_test_curve_from_data(Data + 256, Size > 256 ? Size - 256 : 0);
        if (curve3) {
            cmsToneCurve* triple[3] = {curve1, curve2, curve3};
            cmsFreeToneCurveTriple(triple);
            curve1 = curve2 = curve3 = NULL;
        } else {
            if (curve1) cmsFreeToneCurve(curve1);
            if (curve2) cmsFreeToneCurve(curve2);
        }
    } else {
        if (curve1) cmsFreeToneCurve(curve1);
        if (curve2) cmsFreeToneCurve(curve2);
    }
    
    cmsDeleteContext(context);
    return 0;
}