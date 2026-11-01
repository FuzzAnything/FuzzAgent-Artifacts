// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = cmsOpenProfileFromMem((const unsigned char *)Data, Size);
    if (hProfile == NULL) {
        return 0;
    }

    // Fuzz cmsGetHeaderModel
    cmsUInt32Number model = cmsGetHeaderModel(hProfile);
    (void)model;

    // Fuzz cmsIsCLUT with various intents and directions
    for (int intent = 0; intent <= 3; intent++) {
        for (int direction = 0; direction <= 2; direction++) {
            cmsBool isClut = cmsIsCLUT(hProfile, (cmsUInt32Number)intent, (cmsUInt32Number)direction);
            (void)isClut;
        }
    }

    // Fuzz cmsGetHeaderFlags
    cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
    (void)flags;

    // Fuzz cmsFormatterForColorspaceOfProfile with various nBytes and lIsFloat
    for (int nBytes = 1; nBytes <= 2; nBytes++) {
        for (int isFloat = 0; isFloat <= 1; isFloat++) {
            cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(hProfile, (cmsUInt32Number)nBytes, (cmsBool)isFloat);
            (void)formatter;
        }
    }

    // Fuzz cmsGetEncodedICCversion
    cmsUInt32Number version = cmsGetEncodedICCversion(hProfile);
    (void)version;

    // Fuzz cmsFormatterForPCSOfProfile with various nBytes and lIsFloat
    for (int nBytes = 1; nBytes <= 8; nBytes++) {
        for (int isFloat = 0; isFloat <= 1; isFloat++) {
            cmsUInt32Number pcsFormatter = cmsFormatterForPCSOfProfile(hProfile, (cmsUInt32Number)nBytes, (cmsBool)isFloat);
            (void)pcsFormatter;
        }
    }

    cleanup_profile(hProfile);
    return 0;
}