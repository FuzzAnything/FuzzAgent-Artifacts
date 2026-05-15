// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
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
    
    size_t nEntries = (data[0] << 8) | data[1];
    if (nEntries < 2) nEntries = 2;
    if (nEntries > 4096) nEntries = 4096;
    
    if (size < 2 + nEntries * 2) return NULL;
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(nEntries * sizeof(cmsUInt16Number));
    if (!table) return NULL;
    
    for (size_t i = 0; i < nEntries; i++) {
        table[i] = (data[2 + i * 2] << 8) | data[2 + i * 2 + 1];
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, table);
    free(table);
    return curve;
}

static cmsToneCurve* create_parametric_curve(const uint8_t* data, size_t size) {
    if (size < 44) return NULL;
    
    int type = data[0] % 6;
    cmsFloat64Number params[10];
    
    for (int i = 0; i < 10; i++) {
        uint32_t val;
        memcpy(&val, &data[4 + i * 4], 4);
        params[i] = (cmsFloat64Number)val / 1000.0;
    }
    
    return cmsBuildParametricToneCurve(NULL, type, params);
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    cmsToneCurve* curve1 = NULL;
    cmsToneCurve* curve2 = NULL;
    cmsToneCurve* curve3 = NULL;
    
    size_t offset = 0;
    
    if (Size > 100) {
        size_t curve1_size = Data[0] % (Size / 2);
        curve1 = create_test_tone_curve(Data, curve1_size);
        offset = curve1_size;
    }
    
    if (Size - offset > 50) {
        size_t curve2_size = Data[1] % (Size - offset);
        curve2 = create_parametric_curve(Data + offset, curve2_size);
        offset += curve2_size;
    }
    
    if (Size - offset > 30) {
        size_t curve3_size = Data[2] % (Size - offset);
        curve3 = create_test_tone_curve(Data + offset, curve3_size);
    }
    
    if (curve1) {
        cmsToneCurve* dup = cmsDupToneCurve(curve1);
        if (dup) {
            cmsFreeToneCurve(dup);
        }
        
        cmsToneCurve* rev1 = cmsReverseToneCurve(curve1);
        if (rev1) {
            cmsFreeToneCurve(rev1);
        }
        
        cmsUInt32Number samples = (Data[3] << 24) | (Data[4] << 16) | (Data[5] << 8) | Data[6];
        if (samples < 2) samples = 2;
        if (samples > 65536) samples = 65536;
        
        cmsToneCurve* rev2 = cmsReverseToneCurveEx(samples, curve1);
        if (rev2) {
            cmsFreeToneCurve(rev2);
        }
    }
    
    if (curve1 && curve2) {
        cmsUInt32Number nPoints = (Data[7] << 8) | Data[8];
        if (nPoints < 2) nPoints = 2;
        if (nPoints > 4096) nPoints = 4096;
        
        cmsToneCurve* joined = cmsJoinToneCurve(NULL, curve1, curve2, nPoints);
        if (joined) {
            cmsFreeToneCurve(joined);
        }
    }
    
    if (curve1 && curve2 && curve3) {
        cmsToneCurve* triple[3] = {curve1, curve2, curve3};
        cmsFreeToneCurveTriple(triple);
        curve1 = curve2 = curve3 = NULL;
    } else {
        if (curve1) cmsFreeToneCurve(curve1);
        if (curve2) cmsFreeToneCurve(curve2);
        if (curve3) cmsFreeToneCurve(curve3);
    }
    
    return 0;
}