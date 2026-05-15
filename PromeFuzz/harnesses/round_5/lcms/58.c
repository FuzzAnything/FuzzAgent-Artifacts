// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    write_dummy_file(Data, Size);
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!hProfile) {
        remove_dummy_file();
        return 0;
    }
    
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
    cmsUInt32Number model = cmsGetHeaderModel(hProfile);
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
    
    if (Size >= 4) {
        cmsUInt32Number intent_value = *(cmsUInt32Number*)Data;
        cmsSetHeaderRenderingIntent(hProfile, intent_value);
        
        cmsUInt32Number manufacturer_value = *(cmsUInt32Number*)(Data + (Size >= 8 ? 4 : 0));
        cmsSetHeaderManufacturer(hProfile, manufacturer_value);
        
        cmsUInt32Number updated_manufacturer = cmsGetHeaderManufacturer(hProfile);
        cmsUInt32Number updated_intent = cmsGetHeaderRenderingIntent(hProfile);
    }
    
    cmsCloseProfile(hProfile);
    remove_dummy_file();
    
    return 0;
}