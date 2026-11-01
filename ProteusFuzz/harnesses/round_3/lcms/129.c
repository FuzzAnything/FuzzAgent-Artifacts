// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
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

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number manufacturer;
    cmsUInt32Number creator;
    cmsUInt32Number flags;
    cmsUInt32Number intent;
    cmsUInt32Number model;

    // Attempt to open a profile from the fuzzed data
    hProfile = cmsOpenProfileFromMem((const cmsUInt8Number *)Data, Size);

    if (hProfile == NULL) {
        return 0;
    }

    // Test cmsGetHeaderManufacturer
    manufacturer = cmsGetHeaderManufacturer(hProfile);
    (void)manufacturer; // Suppress unused variable warning

    // Test cmsGetHeaderCreator
    creator = cmsGetHeaderCreator(hProfile);
    (void)creator; // Suppress unused variable warning

    // Test cmsGetHeaderFlags
    flags = cmsGetHeaderFlags(hProfile);
    (void)flags; // Suppress unused variable warning

    // Test cmsGetHeaderRenderingIntent
    intent = cmsGetHeaderRenderingIntent(hProfile);
    (void)intent; // Suppress unused variable warning

    // Test cmsSetHeaderRenderingIntent with various intents (0-3)
    cmsSetHeaderRenderingIntent(hProfile, 0);
    cmsSetHeaderRenderingIntent(hProfile, 1);
    cmsSetHeaderRenderingIntent(hProfile, 2);
    cmsSetHeaderRenderingIntent(hProfile, 3);
    // Also try some out-of-range values to test robustness
    cmsSetHeaderRenderingIntent(hProfile, 4);
    cmsSetHeaderRenderingIntent(hProfile, 255);
    cmsSetHeaderRenderingIntent(hProfile, 0xFFFFFFFF);

    // Test cmsGetHeaderModel
    model = cmsGetHeaderModel(hProfile);
    (void)model; // Suppress unused variable warning

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}