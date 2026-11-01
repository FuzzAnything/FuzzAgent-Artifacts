// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    
    // Try to open the profile from the fuzzing input
    hProfile = cmsOpenProfileFromMem((const unsigned char *)Data, Size);

    if (hProfile != NULL) {
        // Test cmsGetHeaderRenderingIntent
        cmsUInt32Number intent = cmsGetHeaderRenderingIntent(hProfile);
        (void)intent; // Suppress unused variable warning

        // Test cmsGetHeaderCreator
        cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
        (void)creator;

        // Test cmsGetHeaderFlags
        cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
        (void)flags;

        // Test cmsGetHeaderModel
        cmsUInt32Number model = cmsGetHeaderModel(hProfile);
        (void)model;

        // Test cmsGetEncodedICCversion
        cmsUInt32Number version = cmsGetEncodedICCversion(hProfile);
        (void)version;

        // Test cmsGetHeaderManufacturer
        cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
        (void)manufacturer;

        // Cleanup
        cleanup_profile(hProfile);
    }

    return 0;
}