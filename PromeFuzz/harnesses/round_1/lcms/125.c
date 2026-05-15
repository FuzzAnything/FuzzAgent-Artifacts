// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateLinearizationDeviceLink at cmsvirt.c:340:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
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

static cmsToneCurve* create_tone_curve_from_data(const uint8_t* data, size_t size, size_t* consumed) {
    if (size < 2) return NULL;
    
    cmsUInt32Number nEntries = data[0] * 256 + data[1];
    if (nEntries < 2) nEntries = 2;
    if (nEntries > 4096) nEntries = 4096;
    
    size_t needed = nEntries * sizeof(cmsUInt16Number);
    if (needed > size - 2) {
        nEntries = (size - 2) / sizeof(cmsUInt16Number);
        if (nEntries < 2) return NULL;
    }
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(needed);
    if (!table) return NULL;
    
    for (cmsUInt32Number i = 0; i < nEntries; i++) {
        size_t offset = 2 + i * sizeof(cmsUInt16Number);
        if (offset + 1 < size) {
            table[i] = (data[offset] << 8) | data[offset + 1];
        } else {
            table[i] = (cmsUInt16Number)((i * 65535) / (nEntries - 1));
        }
    }
    
    *consumed = 2 + nEntries * sizeof(cmsUInt16Number);
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, table);
    free(table);
    return curve;
}

static cmsToneCurve* create_parametric_curve_from_data(const uint8_t* data, size_t size, size_t* consumed) {
    if (size < 1) return NULL;
    
    cmsInt32Number type = data[0] % 6;
    if (type < 0) type = 0;
    
    cmsFloat64Number params[10] = {0};
    size_t param_bytes = sizeof(cmsFloat64Number) * 10;
    
    if (size >= 1 + param_bytes) {
        memcpy(params, data + 1, param_bytes);
    } else {
        for (int i = 0; i < 10; i++) {
            params[i] = 1.0 + (data[(i % (size-1)) + 1] / 255.0);
        }
    }
    
    *consumed = 1 + param_bytes;
    return cmsBuildParametricToneCurve(NULL, type, params);
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    size_t offset = 0;
    cmsToneCurve* curves[3] = {NULL, NULL, NULL};
    
    for (int i = 0; i < 3 && offset < Size; i++) {
        size_t consumed = 0;
        if (Data[offset] % 2 == 0) {
            curves[i] = create_tone_curve_from_data(Data + offset, Size - offset, &consumed);
        } else {
            curves[i] = create_parametric_curve_from_data(Data + offset, Size - offset, &consumed);
        }
        if (consumed > 0) offset += consumed;
        else offset += 1;
    }
    
    if (curves[0]) {
        cmsToneCurve* dup = cmsDupToneCurve(curves[0]);
        if (dup) cmsFreeToneCurve(dup);
    }
    
    if (curves[1]) {
        cmsToneCurve* rev = cmsReverseToneCurve(curves[1]);
        if (rev) cmsFreeToneCurve(rev);
    }
    
    if (curves[0] && curves[1]) {
        cmsUInt32Number nPoints = 256;
        if (Size > offset) {
            nPoints = Data[offset] * 256;
            if (nPoints < 2) nPoints = 2;
            if (nPoints > 65536) nPoints = 65536;
        }
        cmsToneCurve* joined = cmsJoinToneCurve(NULL, curves[0], curves[1], nPoints);
        if (joined) cmsFreeToneCurve(joined);
    }
    
    if (curves[0] && curves[1] && curves[2]) {
        cmsColorSpaceSignature colorSpace = (cmsColorSpaceSignature)(Data[0] % 20);
        cmsToneCurve* transferFunctions[3] = {curves[0], curves[1], curves[2]};
        
        cmsHPROFILE profile1 = cmsCreateLinearizationDeviceLink(colorSpace, transferFunctions);
        if (profile1) cmsCloseProfile(profile1);
        
        cmsContext context = cmsCreateContext(NULL, NULL);
        if (context) {
            cmsHPROFILE profile2 = cmsCreateLinearizationDeviceLinkTHR(context, colorSpace, transferFunctions);
            if (profile2) cmsCloseProfile(profile2);
            cmsDeleteContext(context);
        }
    }
    
    cmsFreeToneCurveTriple(curves);
    
    return 0;
}