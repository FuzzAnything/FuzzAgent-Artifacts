// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsCreateLinearizationDeviceLink at cmsvirt.c:340:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
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

static cmsToneCurve* create_test_tone_curve(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 256) return NULL;
    
    cmsUInt16Number table[256];
    for (int i = 0; i < 256; i++) {
        table[i] = (cmsUInt16Number)(((uint16_t)data[i % size] << 8) | data[(i + 1) % size]);
    }
    
    return cmsBuildTabulatedToneCurve16(ctx, 256, table);
}

static void cleanup_tone_curve(cmsToneCurve* curve) {
    if (curve) {
        cmsFreeToneCurve(curve);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create test tone curves using fuzzer data
    cmsToneCurve* curve1 = create_test_tone_curve(ctx, Data, Size);
    cmsToneCurve* curve2 = create_test_tone_curve(ctx, Data + 128, Size > 128 ? Size - 128 : 0);
    cmsToneCurve* curve3 = create_test_tone_curve(ctx, Data + 256, Size > 256 ? Size - 256 : 0);
    
    // Test cmsReverseToneCurve
    if (curve1) {
        cmsToneCurve* reversed = cmsReverseToneCurve(curve1);
        cleanup_tone_curve(reversed);
    }
    
    // Test cmsDupToneCurve
    if (curve1) {
        cmsToneCurve* duplicated = cmsDupToneCurve(curve1);
        cleanup_tone_curve(duplicated);
    }
    
    // Test cmsJoinToneCurve
    if (curve1 && curve2) {
        cmsUInt32Number nPoints = (cmsUInt32Number)(Data[0] * 256 + Data[1]);
        if (nPoints < 2) nPoints = 2;
        if (nPoints > 4096) nPoints = 4096;
        
        cmsToneCurve* joined = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
        cleanup_tone_curve(joined);
    }
    
    // Test cmsFreeToneCurveTriple
    if (curve1 && curve2 && curve3) {
        cmsToneCurve* triple[3] = {cmsDupToneCurve(curve1), 
                                   cmsDupToneCurve(curve2), 
                                   cmsDupToneCurve(curve3)};
        cmsFreeToneCurveTriple(triple);
    }
    
    // Test cmsCreateLinearizationDeviceLink
    if (curve1 && curve2 && curve3) {
        cmsToneCurve* transferFuncs[3] = {curve1, curve2, curve3};
        cmsColorSpaceSignature colorspace = (cmsColorSpaceSignature)(
            (Data[2] << 24) | (Data[3] << 16) | (Data[4] << 8) | Data[5]);
        
        cmsHPROFILE profile = cmsCreateLinearizationDeviceLink(colorspace, transferFuncs);
        if (profile) {
            cmsCloseProfile(profile);
        }
    }
    
    // Test cmsCreateLinearizationDeviceLinkTHR
    if (curve1 && curve2 && curve3) {
        cmsToneCurve* transferFuncs[3] = {curve1, curve2, curve3};
        cmsColorSpaceSignature colorspace = (cmsColorSpaceSignature)(
            (Data[6] << 24) | (Data[7] << 16) | (Data[8] << 8) | Data[9]);
        
        cmsHPROFILE profile = cmsCreateLinearizationDeviceLinkTHR(ctx, colorspace, transferFuncs);
        if (profile) {
            cmsCloseProfile(profile);
        }
    }
    
    // Cleanup
    cleanup_tone_curve(curve1);
    cleanup_tone_curve(curve2);
    cleanup_tone_curve(curve3);
    cmsDeleteContext(ctx);
    
    return 0;
}