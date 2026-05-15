// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

static cmsToneCurve* create_test_curve_from_fuzz(const uint8_t *data, size_t size, cmsContext ctx) {
    if (size < 4) return NULL;
    
    cmsUInt32Number nEntries = data[0] % 256;
    if (nEntries == 0) nEntries = 1;
    
    if (nEntries * sizeof(cmsFloat32Number) > size - 1) {
        nEntries = (size - 1) / sizeof(cmsFloat32Number);
        if (nEntries == 0) nEntries = 1;
    }
    
    cmsFloat32Number *values = (cmsFloat32Number*)malloc(nEntries * sizeof(cmsFloat32Number));
    if (!values) return NULL;
    
    for (cmsUInt32Number i = 0; i < nEntries; i++) {
        cmsFloat32Number val;
        memcpy(&val, data + 1 + i * sizeof(cmsFloat32Number), sizeof(cmsFloat32Number));
        values[i] = val;
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
    free(values);
    return curve;
}

static cmsToneCurve* create_second_curve_from_fuzz(const uint8_t *data, size_t size, cmsContext ctx) {
    if (size < 8) return NULL;
    
    size_t offset = size / 2;
    cmsUInt32Number nEntries = data[offset] % 128;
    if (nEntries == 0) nEntries = 1;
    
    if (nEntries * sizeof(cmsFloat32Number) > size - offset - 1) {
        nEntries = (size - offset - 1) / sizeof(cmsFloat32Number);
        if (nEntries == 0) nEntries = 1;
    }
    
    cmsFloat32Number *values = (cmsFloat32Number*)malloc(nEntries * sizeof(cmsFloat32Number));
    if (!values) return NULL;
    
    for (cmsUInt32Number i = 0; i < nEntries; i++) {
        cmsFloat32Number val;
        memcpy(&val, data + offset + 1 + i * sizeof(cmsFloat32Number), sizeof(cmsFloat32Number));
        values[i] = val;
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
    free(values);
    return curve;
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsToneCurve* curve1 = create_test_curve_from_fuzz(Data, Size, ctx);
    if (curve1) {
        cmsToneCurve* reversed = cmsReverseToneCurve(curve1);
        if (reversed) {
            cmsFloat32Number test_val = 0.5f;
            if (Size > sizeof(cmsFloat32Number)) {
                memcpy(&test_val, Data, sizeof(cmsFloat32Number));
            }
            cmsEvalToneCurveFloat(reversed, test_val);
            cmsFreeToneCurve(reversed);
        }
        
        cmsToneCurve* duplicated = cmsDupToneCurve(curve1);
        if (duplicated) {
            cmsFloat32Number test_val = 0.3f;
            if (Size > 2 * sizeof(cmsFloat32Number)) {
                memcpy(&test_val, Data + sizeof(cmsFloat32Number), sizeof(cmsFloat32Number));
            }
            cmsEvalToneCurveFloat(duplicated, test_val);
            cmsFreeToneCurve(duplicated);
        }
        
        cmsUInt32Number nSamples = 256;
        if (Size > 4) {
            nSamples = Data[1] % 1024;
            if (nSamples == 0) nSamples = 1;
        }
        cmsToneCurve* reversed_ex = cmsReverseToneCurveEx(nSamples, curve1);
        if (reversed_ex) {
            cmsFloat32Number test_val = 0.7f;
            if (Size > 3 * sizeof(cmsFloat32Number)) {
                memcpy(&test_val, Data + 2 * sizeof(cmsFloat32Number), sizeof(cmsFloat32Number));
            }
            cmsEvalToneCurveFloat(reversed_ex, test_val);
            cmsFreeToneCurve(reversed_ex);
        }
        
        cmsToneCurve* curve2 = create_second_curve_from_fuzz(Data, Size, ctx);
        if (curve2) {
            cmsUInt32Number nPoints = 256;
            if (Size > 5) {
                nPoints = Data[2] % 1024;
                if (nPoints == 0) nPoints = 1;
            }
            cmsToneCurve* joined = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
            if (joined) {
                cmsFloat32Number test_val = 0.2f;
                if (Size > 4 * sizeof(cmsFloat32Number)) {
                    memcpy(&test_val, Data + 3 * sizeof(cmsFloat32Number), sizeof(cmsFloat32Number));
                }
                cmsEvalToneCurveFloat(joined, test_val);
                cmsFreeToneCurve(joined);
            }
            cmsFreeToneCurve(curve2);
        }
        
        cmsFreeToneCurve(curve1);
    }
    
    cmsDeleteContext(ctx);
    return 0;
}