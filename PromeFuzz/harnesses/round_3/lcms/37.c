// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
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

static cmsHPROFILE create_test_profile(void) {
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},
        {0.3000, 0.6000, 1.0},
        {0.1500, 0.0600, 1.0}
    };
    
    return cmsCreateRGBProfile(&whitePoint, &primaries, NULL);
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    cmsHPROFILE profile = create_test_profile();
    if (!profile) {
        return 0;
    }
    
    cmsUInt32Number value1 = *(cmsUInt32Number*)Data;
    
    cmsSetHeaderManufacturer(profile, value1);
    cmsUInt32Number retrieved_manufacturer = cmsGetHeaderManufacturer(profile);
    
    cmsSetHeaderRenderingIntent(profile, value1);
    
    cmsUInt32Number flags = cmsGetHeaderFlags(profile);
    
    cmsSetHeaderModel(profile, value1);
    
    cmsUInt32Number creator = cmsGetHeaderCreator(profile);
    
    cmsCloseProfile(profile);
    
    return 0;
}