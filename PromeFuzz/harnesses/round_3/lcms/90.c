// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
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

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    // Create a dummy profile from memory for testing
    cmsHPROFILE hProfile = NULL;
    
    // Try to create profile from input data
    if (Size > 0) {
        hProfile = cmsOpenProfileFromMem(Data, Size);
    }
    
    // If profile creation failed, create a simple sRGB profile
    if (!hProfile) {
        hProfile = cmsCreate_sRGBProfile();
    }
    
    if (!hProfile) {
        return 0; // Failed to create any profile
    }
    
    // Test cmsGetEncodedICCversion
    cmsUInt32Number encodedVersion = cmsGetEncodedICCversion(hProfile);
    (void)encodedVersion; // Use result to avoid unused variable warning
    
    // Test cmsGetProfileVersion
    cmsFloat64Number profileVersion = cmsGetProfileVersion(hProfile);
    (void)profileVersion;
    
    // Test cmsSetHeaderRenderingIntent
    // Generate rendering intent from input data if available
    cmsUInt32Number renderingIntent = 0;
    if (Size >= sizeof(cmsUInt32Number)) {
        renderingIntent = *(const cmsUInt32Number*)Data;
    }
    cmsSetHeaderRenderingIntent(hProfile, renderingIntent);
    
    // Test cmsSetProfileVersion
    // Generate version from input data if available
    cmsFloat64Number newVersion = 2.0; // Default
    if (Size >= sizeof(cmsFloat64Number)) {
        // Use first 8 bytes as double value
        memcpy(&newVersion, Data, sizeof(cmsFloat64Number));
    }
    cmsSetProfileVersion(hProfile, newVersion);
    
    // Test cmsGetHeaderManufacturer
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
    (void)manufacturer;
    
    // Test cmsGetHeaderCreator
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    (void)creator;
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}