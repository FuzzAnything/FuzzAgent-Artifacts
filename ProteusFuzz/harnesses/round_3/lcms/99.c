// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
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

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number flags, version, model;
    cmsBool isSupported, isClut;
    cmsUInt32Number formatter;
    cmsUInt32Number intent, direction;

    // Create a dummy file to load a profile if needed
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Attempt to open the profile from the dummy file
    hProfile = cmsOpenProfileFromFile(dummy_file, "r");

    // If profile loading fails, try to create a simple RGB profile for testing
    if (hProfile == NULL) {
        cmsHPROFILE hRGB = cmsCreate_sRGBProfile();
        if (hRGB != NULL) {
            hProfile = hRGB;
        }
    }

    if (hProfile == NULL) {
        return 0;
    }

    // Extract intent and direction from input data if available
    if (Size >= 2) {
        intent = Data[0] % 4; // Valid intents are 0-3
        direction = Data[1] % 3; // Valid directions are 0-2
    } else {
        intent = 0;
        direction = 0;
    }

    // Fuzz cmsIsIntentSupported
    isSupported = cmsIsIntentSupported(hProfile, intent, direction);
    (void)isSupported;

    // Fuzz cmsGetHeaderFlags
    flags = cmsGetHeaderFlags(hProfile);
    (void)flags;

    // Fuzz cmsGetEncodedICCversion
    version = cmsGetEncodedICCversion(hProfile);
    (void)version;

    // Fuzz cmsIsCLUT
    isClut = cmsIsCLUT(hProfile, intent, direction);
    (void)isClut;

    // Fuzz cmsFormatterForPCSOfProfile
    cmsUInt32Number nBytes = (Size > 0) ? (Data[0] % 8) + 1 : 1;
    cmsBool lIsFloat = (Size > 1) ? (Data[1] % 2) : 0;
    formatter = cmsFormatterForPCSOfProfile(hProfile, nBytes, lIsFloat);
    (void)formatter;

    // Fuzz cmsGetHeaderModel
    model = cmsGetHeaderModel(hProfile);
    (void)model;

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}