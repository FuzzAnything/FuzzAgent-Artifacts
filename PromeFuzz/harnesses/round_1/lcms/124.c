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
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsCreateLinearizationDeviceLink at cmsvirt.c:340:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
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
    if (size < 2) return NULL;
    
    size_t table_size = (size / 2) & ~1;
    if (table_size < 2) table_size = 2;
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(table_size * sizeof(cmsUInt16Number));
    if (!table) return NULL;
    
    for (size_t i = 0; i < table_size && i * 2 < size; i++) {
        table[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, table_size, table);
    free(table);
    return curve;
}

static cmsToneCurve* create_parametric_curve(const uint8_t* data, size_t size) {
    if (size < 4) return NULL;
    
    int type = data[0] % 6;
    cmsFloat64Number params[10];
    
    for (int i = 0; i < 10; i++) {
        params[i] = (i * 2 < size) ? (data[i * 2] / 255.0) : 1.0;
    }
    
    return cmsBuildParametricToneCurve(NULL, type, params);
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    cmsToneCurve* curve1 = create_tone_curve_from_data(Data, Size);
    cmsToneCurve* curve2 = create_parametric_curve(Data + Size/2, Size/2);
    
    if (curve1) {
        cmsToneCurve* dup_curve = cmsDupToneCurve(curve1);
        if (dup_curve) {
            cmsFreeToneCurve(dup_curve);
        }
        
        cmsToneCurve* rev_curve = cmsReverseToneCurve(curve1);
        if (rev_curve) {
            cmsFreeToneCurve(rev_curve);
        }
    }
    
    if (curve1 && curve2) {
        cmsUInt32Number nPoints = (Data[0] % 4096) + 2;
        cmsToneCurve* joined = cmsJoinToneCurve(context, curve1, curve2, nPoints);
        if (joined) {
            cmsFreeToneCurve(joined);
        }
    }
    
    cmsToneCurve* triple[3] = {NULL, NULL, NULL};
    if (curve1) triple[0] = curve1;
    if (curve2) triple[1] = curve2;
    
    if (Size > 100) {
        cmsToneCurve* curve3 = create_tone_curve_from_data(Data + Size/3, Size/3);
        if (curve3) triple[2] = curve3;
    }
    
    cmsFreeToneCurveTriple(triple);
    
    cmsToneCurve* transfer_funcs[3] = {NULL, NULL, NULL};
    if (Size > 50) {
        transfer_funcs[0] = create_tone_curve_from_data(Data, Size/3);
        transfer_funcs[1] = create_tone_curve_from_data(Data + Size/6, Size/3);
        transfer_funcs[2] = create_tone_curve_from_data(Data + Size/3, Size/3);
    }
    
    cmsColorSpaceSignature colorspace = (Data[1] % 2) ? cmsSigRgbData : cmsSigCmykData;
    
    cmsHPROFILE profile1 = cmsCreateLinearizationDeviceLink(colorspace, transfer_funcs);
    if (profile1) {
        cmsCloseProfile(profile1);
    }
    
    cmsHPROFILE profile2 = cmsCreateLinearizationDeviceLinkTHR(context, colorspace, transfer_funcs);
    if (profile2) {
        cmsCloseProfile(profile2);
    }
    
    for (int i = 0; i < 3; i++) {
        if (transfer_funcs[i]) {
            cmsFreeToneCurve(transfer_funcs[i]);
        }
    }
    
    if (curve1) cmsFreeToneCurve(curve1);
    if (curve2) cmsFreeToneCurve(curve2);
    
    cmsDeleteContext(context);
    
    return 0;
}