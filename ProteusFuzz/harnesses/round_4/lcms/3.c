// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsContext createContextFromData(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return NULL;
    
    if (Size < 128) {
        return ctx;
    }
    
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
        return ctx;
    }
    
    return ctx;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsPipeline *pipelines[12] = {NULL};

    for (int i = 0; i < 12; i++) {
        cmsPipelineFree(pipelines[i]);
    }

    cmsCIEXYZ BlackPoint = {0.0, 0.0, 0.0};
    cmsHPROFILE hProfile = NULL;
    
    if (Size > 0) {
        hProfile = cmsOpenProfileFromMem(Data, Size);
    }

    cmsUInt32Number intents[] = {
        0, // cmsIntentPerceptual
        1, // cmsIntentRelative
        2, // cmsIntentSaturation
        3  // cmsIntentAbsolute
    };
    
    for (int i = 0; i < 4; i++) {
        if (hProfile != NULL) {
            cmsDetectDestinationBlackPoint(&BlackPoint, hProfile, intents[i], 0);
        } else {
            cmsDetectDestinationBlackPoint(&BlackPoint, NULL, intents[i], 0);
        }
    }

    if (hProfile != NULL) {
        cmsDetectTAC(hProfile);
    } else {
        cmsDetectTAC(NULL);
    }

    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
    
    cmsDeleteContext(ctx);

    return 0;
}