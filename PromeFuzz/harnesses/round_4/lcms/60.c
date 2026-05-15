// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// cmsGetHeaderAttributes at cmsio0.c:1038:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    // Create a dummy sRGB profile to work with
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) {
        return 0;
    }

    // Use fuzz data for various operations
    if (Size >= 16) {
        // Use first 16 bytes for profile ID operations
        cmsUInt8Number profileID[16];
        memcpy(profileID, Data, 16);
        
        // Set and get profile ID
        cmsSetHeaderProfileID(hProfile, profileID);
        
        cmsUInt8Number retrievedID[16];
        cmsGetHeaderProfileID(hProfile, retrievedID);
    }

    if (Size >= 4) {
        // Use next 4 bytes for flags
        cmsUInt32Number flags = 0;
        memcpy(&flags, Data, 4);
        cmsSetHeaderFlags(hProfile, flags);
    }

    if (Size >= 8) {
        // Use next 8 bytes for attributes
        cmsUInt64Number attributes = 0;
        memcpy(&attributes, Data, 8);
        
        // Set and get attributes
        cmsSetHeaderAttributes(hProfile, attributes);
        
        cmsUInt64Number retrievedAttributes = 0;
        cmsGetHeaderAttributes(hProfile, &retrievedAttributes);
    }

    if (Size >= 4) {
        // Use next 4 bytes for rendering intent
        cmsUInt32Number renderingIntent = 0;
        memcpy(&renderingIntent, Data, 4);
        cmsSetHeaderRenderingIntent(hProfile, renderingIntent);
    }

    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}