// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
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
#include <stdio.h>

static cmsHPROFILE create_test_profile_from_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return NULL;
    
    fwrite(data, 1, size, f);
    fclose(f);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreate_sRGBProfile();
    }
    
    remove("./dummy_file");
    return profile;
}

static void test_cmsDetectDestinationBlackPoint(cmsHPROFILE profile, const uint8_t *data, size_t size) {
    cmsCIEXYZ blackPoint;
    cmsUInt32Number intent = (size > 0) ? (data[0] % 4) : 0;
    cmsUInt32Number flags = (size > 1) ? data[1] : 0;
    
    cmsDetectDestinationBlackPoint(&blackPoint, profile, intent, flags);
}

static void test_cmsIsCLUT(cmsHPROFILE profile, const uint8_t *data, size_t size) {
    cmsUInt32Number intent = (size > 0) ? (data[0] % 4) : 0;
    cmsUInt32Number direction = (size > 1) ? (data[1] % 4) : 0;
    
    cmsIsCLUT(profile, intent, direction);
}

static void test_cmsIsIntentSupported(cmsHPROFILE profile, const uint8_t *data, size_t size) {
    cmsUInt32Number intent = (size > 0) ? (data[0] % 4) : 0;
    cmsUInt32Number direction = (size > 1) ? (data[1] % 4) : 0;
    
    cmsIsIntentSupported(profile, intent, direction);
}

static void test_cmsGetHeaderRenderingIntent(cmsHPROFILE profile) {
    cmsGetHeaderRenderingIntent(profile);
}

static void test_cmsDetectBlackPoint(cmsHPROFILE profile, const uint8_t *data, size_t size) {
    cmsCIEXYZ blackPoint;
    cmsUInt32Number intent = (size > 0) ? (data[0] % 4) : 0;
    cmsUInt32Number flags = (size > 1) ? data[1] : 0;
    
    cmsDetectBlackPoint(&blackPoint, profile, intent, flags);
}

static void test_cmsGetPostScriptCSA(cmsHPROFILE profile, const uint8_t *data, size_t size) {
    cmsUInt32Number intent = (size > 0) ? (data[0] % 4) : 0;
    cmsUInt32Number flags = (size > 1) ? data[1] : 0;
    cmsUInt32Number bufferSize = (size > 4) ? *((cmsUInt32Number*)(data + 2)) : 1024;
    
    void *buffer = malloc(bufferSize);
    if (!buffer) return;
    
    cmsGetPostScriptCSA(NULL, profile, intent, flags, buffer, bufferSize);
    
    free(buffer);
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsHPROFILE profile = create_test_profile_from_data(Data, Size);
    if (!profile) return 0;
    
    test_cmsDetectDestinationBlackPoint(profile, Data, Size);
    test_cmsIsCLUT(profile, Data, Size);
    test_cmsIsIntentSupported(profile, Data, Size);
    test_cmsGetHeaderRenderingIntent(profile);
    test_cmsDetectBlackPoint(profile, Data, Size);
    test_cmsGetPostScriptCSA(profile, Data, Size);
    
    cmsCloseProfile(profile);
    return 0;
}