// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "lcms2.h"

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Fuzz cmsOpenProfileFromMem
    cmsHPROFILE hProfile = cmsOpenProfileFromMem((const void *)Data, (cmsUInt32Number)Size);
    
    if (hProfile == NULL) {
        // Profile failed to load, try to create a dummy profile for testing other APIs
        // Since we cannot easily create a valid profile from scratch without more complex setup,
        // we will skip testing APIs that require a valid profile handle in this case.
        return 0;
    }

    // Fuzz cmsGetProfileVersion
    cmsFloat64Number version = cmsGetProfileVersion(hProfile);
    (void)version; // Use the return value to suppress unused variable warnings

    // Fuzz cmsGetTagCount
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    (void)tagCount; // Use the return value

    // Fuzz cmsSetHeaderRenderingIntent with various intents (0-3)
    cmsUInt32Number intent = (Size % 4);
    cmsSetHeaderRenderingIntent(hProfile, intent);

    // Fuzz cmsDetectTAC
    cmsFloat64Number tac = cmsDetectTAC(hProfile);
    (void)tac; // Use the return value

    // Fuzz cmsSetProfileVersion with various versions
    cmsFloat64Number newVersion = (cmsFloat64Number)(Size % 100) / 10.0 + 2.0; // Generate version between 2.0 and 11.9
    cmsSetProfileVersion(hProfile, newVersion);

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}