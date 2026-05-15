// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// cmsGetHeaderAttributes at cmsio0.c:1038:16 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

static cmsHPROFILE create_test_profile(const uint8_t* data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE* file = fopen("./dummy_file", "wb");
    if (!file) {
        return cmsCreate_sRGBProfile();
    }
    
    fwrite(data, 1, size, file);
    fclose(file);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (profile) {
        return profile;
    }
    
    return cmsCreate_sRGBProfile();
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    cmsHPROFILE hProfile = create_test_profile(Data, Size);
    if (!hProfile) {
        return 0;
    }
    
    cmsUInt32Number renderingIntent = cmsGetHeaderRenderingIntent(hProfile);
    (void)renderingIntent;
    
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
    (void)manufacturer;
    
    cmsUInt32Number newManufacturer = 0;
    if (Size >= 4) {
        memcpy(&newManufacturer, Data, sizeof(newManufacturer));
    }
    cmsSetHeaderManufacturer(hProfile, newManufacturer);
    
    cmsUInt32Number version = 0;
    if (Size >= 8) {
        memcpy(&version, Data + 4, sizeof(version));
    }
    cmsSetEncodedICCversion(hProfile, version);
    
    cmsUInt64Number flags = 0;
    cmsGetHeaderAttributes(hProfile, &flags);
    
    cmsUInt64Number newFlags = 0;
    if (Size >= 16) {
        memcpy(&newFlags, Data + 8, sizeof(newFlags));
    }
    cmsSetHeaderAttributes(hProfile, newFlags);
    
    cmsCloseProfile(hProfile);
    return 0;
}