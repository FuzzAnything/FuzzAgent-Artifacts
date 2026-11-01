// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 100) return NULL;
    // Create a temporary file for the profile data
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return NULL;
    fwrite(data, 1, size, f);
    fclose(f);

    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    return hProfile;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsHPROFILE hProfile = create_dummy_profile(Data, Size);
    if (!hProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Fuzz cmsPipelineFree multiple times with NULL to test safety
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);
    cmsPipelineFree(NULL);

    cmsCIEXYZ blackPoint = {0, 0, 0};
    cmsUInt32Number intents[] = {0, 1, 2, 3}; // PERCEPTUAL, SATURATION, RELATIVE, ABSOLUTE
    cmsUInt32Number flags[] = {0, 1, 2, 3};

    // Fuzz cmsDetectDestinationBlackPoint multiple times
    for (int i = 0; i < 4; i++) {
        cmsDetectDestinationBlackPoint(&blackPoint, hProfile, intents[i % 4], flags[i % 4]);
    }

    // Fuzz cmsDetectTAC
    cmsFloat64Number tac = cmsDetectTAC(hProfile);
    (void)tac; // Suppress unused variable warning

    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}