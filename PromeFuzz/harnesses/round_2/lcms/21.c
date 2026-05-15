// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Create a profile from the fuzzer input
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (hProfile == NULL) {
        // If we can't create from memory, create a default sRGB profile
        hProfile = cmsCreate_sRGBProfile();
        if (hProfile == NULL) {
            return 0;
        }
    }

    // Test cmsGetHeaderRenderingIntent
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(hProfile);
    (void)intent; // Use result to avoid compiler warnings

    // Test cmsGetHeaderManufacturer
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
    (void)manufacturer;

    // Test cmsSetHeaderModel - use data from input if available
    cmsUInt32Number model = 0;
    if (Size >= sizeof(cmsUInt32Number)) {
        memcpy(&model, Data, sizeof(cmsUInt32Number));
    }
    cmsSetHeaderModel(hProfile, model);

    // Test cmsSetEncodedICCversion - use data from input if available
    cmsUInt32Number version = 0;
    if (Size >= 2 * sizeof(cmsUInt32Number)) {
        memcpy(&version, Data + sizeof(cmsUInt32Number), sizeof(cmsUInt32Number));
    }
    cmsSetEncodedICCversion(hProfile, version);

    // Test cmsSetHeaderRenderingIntent - use data from input if available
    cmsUInt32Number new_intent = 0;
    if (Size >= 3 * sizeof(cmsUInt32Number)) {
        memcpy(&new_intent, Data + 2 * sizeof(cmsUInt32Number), sizeof(cmsUInt32Number));
    }
    cmsSetHeaderRenderingIntent(hProfile, new_intent);

    // Test cmsGetEncodedICCversion
    cmsUInt32Number current_version = cmsGetEncodedICCversion(hProfile);
    (void)current_version;

    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}