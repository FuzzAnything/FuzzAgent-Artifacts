// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateXYZProfile at cmsvirt.c:617:23 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Create a dummy profile to work with
    cmsHPROFILE hProfile = cmsCreateXYZProfile();
    if (hProfile == NULL) {
        return 0;
    }

    // Extract values from fuzzer input for the various parameters
    cmsUInt32Number flags = 0;
    cmsUInt32Number manufacturer = 0;
    cmsUInt32Number model = 0;
    cmsUInt32Number renderingIntent = 0;

    // Use available data to set values, with fallbacks if not enough data
    if (Size >= sizeof(cmsUInt32Number)) {
        flags = *(cmsUInt32Number*)Data;
    }
    if (Size >= 2 * sizeof(cmsUInt32Number)) {
        manufacturer = *((cmsUInt32Number*)(Data + sizeof(cmsUInt32Number)));
    }
    if (Size >= 3 * sizeof(cmsUInt32Number)) {
        model = *((cmsUInt32Number*)(Data + 2 * sizeof(cmsUInt32Number)));
    }
    if (Size >= 4 * sizeof(cmsUInt32Number)) {
        renderingIntent = *((cmsUInt32Number*)(Data + 3 * sizeof(cmsUInt32Number)));
    }

    // Test all the target API functions
    cmsSetHeaderFlags(hProfile, flags);
    
    cmsUInt32Number retrievedModel = cmsGetHeaderModel(hProfile);
    
    cmsSetHeaderManufacturer(hProfile, manufacturer);
    
    cmsSetHeaderModel(hProfile, model);
    
    cmsSetHeaderRenderingIntent(hProfile, renderingIntent);
    
    cmsUInt32Number retrievedFlags = cmsGetHeaderFlags(hProfile);

    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}