// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lcms2.h"

static cmsHPROFILE create_dummy_profile(const uint8_t* data, size_t size) {
    if (size < 128) return NULL;
    
    FILE* file = fopen("./dummy_file", "wb");
    if (!file) return NULL;
    
    size_t write_size = size > 1024 ? 1024 : size;
    fwrite(data, 1, write_size, file);
    fclose(file);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    remove("./dummy_file");
    
    return profile;
}

int LLVMFuzzerTestOneInput_4(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    for (int i = 0; i < 12; i++) {
        cmsPipelineFree(NULL);
    }
    
    cmsHPROFILE profile = create_dummy_profile(Data, Size);
    if (!profile) return 0;
    
    cmsCIEXYZ blackPoint;
    cmsUInt32Number intents[] = {0, 1, 2, 3};
    cmsUInt32Number flags[] = {0, 1};
    
    for (int i = 0; i < 4; i++) {
        cmsDetectDestinationBlackPoint(&blackPoint, profile, intents[i % 4], flags[i % 2]);
    }
    
    cmsDetectTAC(profile);
    
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    return 0;
}