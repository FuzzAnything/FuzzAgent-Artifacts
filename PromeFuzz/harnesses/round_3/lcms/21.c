// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"

static cmsHPROFILE create_test_profile(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cmsCreate_sRGBProfile();
    }
    
    cmsHPROFILE profile = cmsOpenProfileFromMem(data, size);
    if (profile == NULL) {
        profile = cmsCreate_sRGBProfile();
    }
    
    return profile;
}

static void extract_uint32(const uint8_t *data, size_t size, size_t offset, cmsUInt32Number *value) {
    if (offset + sizeof(cmsUInt32Number) <= size) {
        memcpy(value, data + offset, sizeof(cmsUInt32Number));
    } else {
        *value = 0;
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    cmsHPROFILE profile = create_test_profile(Data, Size);
    if (profile == NULL) {
        return 0;
    }
    
    cmsUInt32Number value1, value2, value3, value4, value5, value6;
    
    extract_uint32(Data, Size, 0, &value1);
    extract_uint32(Data, Size, 4, &value2);
    extract_uint32(Data, Size, 8, &value3);
    extract_uint32(Data, Size, 12, &value4);
    extract_uint32(Data, Size, 16, &value5);
    extract_uint32(Data, Size, 20, &value6);
    
    cmsGetHeaderModel(profile);
    cmsGetHeaderRenderingIntent(profile);
    
    cmsSetHeaderManufacturer(profile, value1);
    cmsSetEncodedICCversion(profile, value2);
    cmsSetHeaderFlags(profile, value3);
    cmsSetHeaderModel(profile, value4);
    
    cmsCloseProfile(profile);
    
    return 0;
}