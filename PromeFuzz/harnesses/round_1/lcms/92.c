// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
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

static cmsFloat32Number readFloat32(const uint8_t** data, size_t* size) {
    if (*size < sizeof(cmsFloat32Number)) {
        *size = 0;
        return 0.0f;
    }
    cmsFloat32Number value;
    memcpy(&value, *data, sizeof(cmsFloat32Number));
    *data += sizeof(cmsFloat32Number);
    *size -= sizeof(cmsFloat32Number);
    return value;
}

static cmsUInt32Number readUInt32(const uint8_t** data, size_t* size) {
    if (*size < sizeof(cmsUInt32Number)) {
        *size = 0;
        return 0;
    }
    cmsUInt32Number value;
    memcpy(&value, *data, sizeof(cmsUInt32Number));
    *data += sizeof(cmsUInt32Number);
    *size -= sizeof(cmsUInt32Number);
    return value;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    const uint8_t* ptr = Data;
    size_t remaining = Size;

    // Create first tone curve
    cmsUInt32Number nEntries1 = readUInt32(&ptr, &remaining) % 256;
    if (nEntries1 == 0) nEntries1 = 1;
    
    cmsFloat32Number* values1 = (cmsFloat32Number*)malloc(nEntries1 * sizeof(cmsFloat32Number));
    if (!values1) {
        cmsDeleteContext(context);
        return 0;
    }
    
    for (cmsUInt32Number i = 0; i < nEntries1 && remaining >= sizeof(cmsFloat32Number); i++) {
        values1[i] = readFloat32(&ptr, &remaining);
    }
    
    cmsToneCurve* curve1 = cmsBuildTabulatedToneCurveFloat(context, nEntries1, values1);
    free(values1);
    
    if (curve1) {
        // Test cmsEvalToneCurveFloat with various inputs
        for (int i = 0; i < 10 && remaining >= sizeof(cmsFloat32Number); i++) {
            cmsFloat32Number input = readFloat32(&ptr, &remaining);
            cmsEvalToneCurveFloat(curve1, input);
        }
        
        // Test cmsDupToneCurve
        cmsToneCurve* dupCurve = cmsDupToneCurve(curve1);
        if (dupCurve) {
            cmsFreeToneCurve(dupCurve);
        }
        
        // Test cmsReverseToneCurve
        cmsToneCurve* revCurve = cmsReverseToneCurve(curve1);
        if (revCurve) {
            cmsFreeToneCurve(revCurve);
        }
        
        // Test cmsReverseToneCurveEx
        cmsUInt32Number nSamples = readUInt32(&ptr, &remaining) % 1024;
        if (nSamples == 0) nSamples = 256;
        cmsToneCurve* revCurveEx = cmsReverseToneCurveEx(nSamples, curve1);
        if (revCurveEx) {
            cmsFreeToneCurve(revCurveEx);
        }
        
        // Create second tone curve for joining
        cmsUInt32Number nEntries2 = readUInt32(&ptr, &remaining) % 256;
        if (nEntries2 == 0) nEntries2 = 1;
        
        cmsFloat32Number* values2 = (cmsFloat32Number*)malloc(nEntries2 * sizeof(cmsFloat32Number));
        if (values2) {
            for (cmsUInt32Number i = 0; i < nEntries2 && remaining >= sizeof(cmsFloat32Number); i++) {
                values2[i] = readFloat32(&ptr, &remaining);
            }
            
            cmsToneCurve* curve2 = cmsBuildTabulatedToneCurveFloat(context, nEntries2, values2);
            free(values2);
            
            if (curve2) {
                // Test cmsJoinToneCurve
                cmsUInt32Number nPoints = readUInt32(&ptr, &remaining) % 1024;
                if (nPoints == 0) nPoints = 256;
                cmsToneCurve* joinedCurve = cmsJoinToneCurve(context, curve1, curve2, nPoints);
                if (joinedCurve) {
                    cmsFreeToneCurve(joinedCurve);
                }
                
                cmsFreeToneCurve(curve2);
            }
        }
        
        cmsFreeToneCurve(curve1);
    }
    
    cmsDeleteContext(context);
    return 0;
}