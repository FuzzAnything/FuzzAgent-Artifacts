// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"

static void init_dummy_profile(cmsHPROFILE *hProfile) {
    // Create a dummy profile for testing using cmsCreateProfilePlaceholder
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE profile = cmsCreateProfilePlaceholder(ctx);
    if (profile) {
        *hProfile = profile;
    } else {
        *hProfile = NULL;
    }
    cmsDeleteContext(ctx);
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    init_dummy_profile(&hProfile);

    cmsPipeline *pipelines[12] = {NULL};
    cmsCIEXYZ blackPoint = {0.0, 0.0, 0.0};
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;

    // Use input data to diversify parameters
    if (Size > 0) intent = Data[0] % 4; // Valid intents: 0-3
    if (Size > 1) flags = Data[1];

    // Create some dummy pipelines for testing cmsPipelineFree
    for (int i = 0; i < 12; i++) {
        pipelines[i] = cmsPipelineAlloc(ctx, 3, 3);
        if (!pipelines[i]) {
            break;
        }
    }

    // Call cmsPipelineFree 12 times as specified
    for (int i = 0; i < 12; i++) {
        cmsPipelineFree(pipelines[i]);
    }

    // Call cmsDetectDestinationBlackPoint 4 times
    for (int i = 0; i < 4; i++) {
        cmsDetectDestinationBlackPoint(&blackPoint, hProfile, intent, flags);
    }

    // Call cmsDetectTAC once
    cmsFloat64Number tac = cmsDetectTAC(hProfile);

    // Cleanup
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    cmsDeleteContext(ctx);

    return 0;
}