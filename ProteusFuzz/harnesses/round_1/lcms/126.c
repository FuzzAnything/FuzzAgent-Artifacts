// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsGetEncodedCMMversion at cmserr.c:30:15 in lcms2.h
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

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number intent;
    cmsUInt32Number direction;
    cmsUInt32Number nBytes;
    cmsBool lIsFloat;

    // Initialize variables from input data
    intent = Size > 0 ? Data[0] % 4 : 0;
    direction = Size > 1 ? Data[1] % 3 : 0;
    nBytes = Size > 2 ? (Data[2] % 8) + 1 : 1;
    lIsFloat = Size > 3 ? (Data[3] % 2) : 0;

    // Try to open a profile from the input data if it's large enough
    if (Size >= 128) {
        hProfile = cmsOpenProfileFromMem((const unsigned char *)Data, Size);
    }

    if (hProfile == NULL) {
        // If opening from memory failed, try to create a dummy profile
        cmsHPROFILE hDummy = cmsCreate_sRGBProfile();
        if (hDummy != NULL) {
            hProfile = hDummy;
        }
    }

    if (hProfile == NULL) {
        return 0;
    }

    // Fuzz cmsIsCLUT
    cmsBool isClut = cmsIsCLUT(hProfile, intent, direction);
    (void)isClut;

    // Fuzz cmsGetHeaderFlags
    cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
    (void)flags;

    // Fuzz cmsFormatterForColorspaceOfProfile
    cmsUInt32Number formatterColorspace = cmsFormatterForColorspaceOfProfile(hProfile, nBytes, lIsFloat);
    (void)formatterColorspace;

    // Fuzz cmsGetEncodedICCversion
    cmsUInt32Number iccVersion = cmsGetEncodedICCversion(hProfile);
    (void)iccVersion;

    // Fuzz cmsFormatterForPCSOfProfile
    cmsUInt32Number formatterPCS = cmsFormatterForPCSOfProfile(hProfile, nBytes, lIsFloat);
    (void)formatterPCS;

    // Fuzz cmsGetEncodedCMMversion
    int cmmVersion = cmsGetEncodedCMMversion();
    (void)cmmVersion;

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}