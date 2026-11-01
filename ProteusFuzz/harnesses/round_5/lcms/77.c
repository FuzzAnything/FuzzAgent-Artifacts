// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
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

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size < 128) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number version, creator, manufacturer, model, intent, flags;

    // Attempt to create a profile from the fuzzed data
    // cmsOpenProfileFromMem is robust and will return NULL if data is invalid
    hProfile = cmsOpenProfileFromMem((const unsigned char *)Data, Size);

    if (hProfile == NULL) {
        // If profile creation fails, we can't test the getter functions safely
        // as they require a valid handle. We return early.
        return 0;
    }

    // Call the target API functions to explore program states
    // These functions are expected to crash or behave undefinedly if hProfile is NULL,
    // but here we ensure hProfile is valid before calling.
    version = cmsGetEncodedICCversion(hProfile);
    creator = cmsGetHeaderCreator(hProfile);
    manufacturer = cmsGetHeaderManufacturer(hProfile);
    model = cmsGetHeaderModel(hProfile);
    intent = cmsGetHeaderRenderingIntent(hProfile);
    flags = cmsGetHeaderFlags(hProfile);

    // Use the return values to prevent compiler optimizations
    if (version == 0 && creator == 0 && manufacturer == 0 && 
        model == 0 && intent == 0 && flags == 0) {
        // Dummy check
    }

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}