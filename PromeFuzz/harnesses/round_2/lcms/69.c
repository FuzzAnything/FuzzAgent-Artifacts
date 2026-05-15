// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
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

static cmsHPROFILE create_test_profile(const uint8_t *data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return cmsCreate_sRGBProfile();
    
    fwrite(data, 1, size > 4096 ? 4096 : size, f);
    fclose(f);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (profile) return profile;
    
    return cmsCreate_sRGBProfile();
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsHPROFILE profile = create_test_profile(Data, Size);
    if (!profile) return 0;
    
    cmsUInt32Number original_manufacturer = cmsGetHeaderManufacturer(profile);
    cmsUInt32Number original_creator = cmsGetHeaderCreator(profile);
    cmsUInt32Number original_flags = cmsGetHeaderFlags(profile);
    cmsUInt32Number original_model = cmsGetHeaderModel(profile);
    
    cmsUInt32Number manufacturer_val = *(const cmsUInt32Number*)Data;
    cmsSetHeaderManufacturer(profile, manufacturer_val);
    
    cmsUInt32Number new_manufacturer = cmsGetHeaderManufacturer(profile);
    
    cmsUInt32Number intent_val = *(const cmsUInt32Number*)(Data + (Size >= 8 ? 4 : 0));
    cmsSetHeaderRenderingIntent(profile, intent_val & 3);
    
    cmsUInt32Number flags_val = *(const cmsUInt32Number*)(Data + (Size >= 12 ? 8 : 0));
    cmsUInt32Number model_val = *(const cmsUInt32Number*)(Data + (Size >= 16 ? 12 : 0));
    
    cmsGetHeaderManufacturer(profile);
    cmsGetHeaderCreator(profile);
    cmsGetHeaderFlags(profile);
    cmsGetHeaderModel(profile);
    
    cmsCloseProfile(profile);
    return 0;
}