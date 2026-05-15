// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
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
    if (size < 2) return NULL;
    
    cmsUInt32Number nEntries = 256;
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(nEntries * sizeof(cmsUInt16Number));
    if (!table) return NULL;
    
    for (cmsUInt32Number i = 0; i < nEntries; i++) {
        if (i * 2 + 1 < size) {
            table[i] = (data[i * 2] << 8) | data[i * 2 + 1];
        } else {
            table[i] = (cmsUInt16Number)(i * 65535 / (nEntries - 1));
        }
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, table);
    free(table);
    return curve;
}

static cmsToneCurve* create_parametric_curve(const uint8_t* data, size_t size) {
    if (size < 10) return NULL;
    
    cmsFloat64Number params[10];
    for (int i = 0; i < 10; i++) {
        params[i] = (cmsFloat64Number)data[i % size] / 255.0;
    }
    
    return cmsBuildParametricToneCurve(NULL, 1, params);
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    cmsToneCurve* curve1 = create_test_tone_curve(Data, Size);
    cmsToneCurve* curve2 = create_parametric_curve(Data, Size);
    
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
        cmsUInt32Number nPoints = (Size > 4) ? *((cmsUInt32Number*)Data) % 4096 : 256;
        if (nPoints < 2) nPoints = 2;
        
        cmsToneCurve* joined = cmsJoinToneCurve(context, curve1, curve2, nPoints);
        if (joined) {
            cmsFreeToneCurve(joined);
        }
    }
    
    cmsToneCurve* triple[3] = {NULL, NULL, NULL};
    if (curve1) triple[0] = curve1;
    if (curve2) triple[1] = curve2;
    if (Size > 10) {
        triple[2] = create_test_tone_curve(Data + 10, Size - 10);
    }
    
    cmsFreeToneCurveTriple(triple);
    
    if (curve1 && curve2) {
        cmsToneCurve* transfer_funcs[3] = {curve1, curve2, NULL};
        if (triple[2]) {
            transfer_funcs[2] = triple[2];
        }
        
        cmsColorSpaceSignature colorspace = cmsSigRgbData;
        if (Size > 0) {
            switch (Data[0] % 4) {
                case 0: colorspace = cmsSigRgbData; break;
                case 1: colorspace = cmsSigCmykData; break;
                case 2: colorspace = cmsSigGrayData; break;
                case 3: colorspace = cmsSigLabData; break;
            }
        }
        
        cmsHPROFILE devlink = cmsCreateLinearizationDeviceLink(colorspace, transfer_funcs);
        if (devlink) {
            cmsCloseProfile(devlink);
        }
        
        cmsHPROFILE devlink_thr = cmsCreateLinearizationDeviceLinkTHR(context, colorspace, transfer_funcs);
        if (devlink_thr) {
            cmsCloseProfile(devlink_thr);
        }
    }
    
    if (curve1 && !triple[0]) cmsFreeToneCurve(curve1);
    if (curve2 && !triple[1]) cmsFreeToneCurve(curve2);
    if (triple[2] && triple[2] != curve1 && triple[2] != curve2) {
        cmsFreeToneCurve(triple[2]);
    }
    
    cmsDeleteContext(context);
    return 0;
}