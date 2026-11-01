// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetEncodedCMMversion at cmserr.c:30:15 in lcms2.h
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

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number version;
    cmsBool supported;
    cmsUInt32Number formatter;
    cmsBool isClut;
    cmsUInt32Number model;
    int cmmVersion;

    // Attempt to load profile from fuzz input
    hProfile = cmsOpenProfileFromMem((const unsigned char *)Data, Size);

    if (hProfile != NULL) {
        // 1. cmsGetEncodedICCversion
        version = cmsGetEncodedICCversion(hProfile);
        (void)version;

        // 2. cmsIsIntentSupported
        // Try different intents (0-3) and directions (INPUT=0, OUTPUT=1, PROOF=2)
        for (cmsUInt32Number intent = 0; intent <= 3; intent++) {
            for (cmsUInt32Number direction = 0; direction <= 2; direction++) {
                supported = cmsIsIntentSupported(hProfile, intent, direction);
                (void)supported;
            }
        }

        // 3. cmsFormatterForPCSOfProfile
        // Try different byte sizes (1-8) and float flags
        for (cmsUInt32Number nBytes = 1; nBytes <= 8; nBytes++) {
            for (cmsBool lIsFloat = 0; lIsFloat <= 1; lIsFloat++) {
                formatter = cmsFormatterForPCSOfProfile(hProfile, nBytes, lIsFloat);
                (void)formatter;
            }
        }

        // 4. cmsIsCLUT
        for (cmsUInt32Number intent = 0; intent <= 3; intent++) {
            for (cmsUInt32Number direction = 0; direction <= 2; direction++) {
                isClut = cmsIsCLUT(hProfile, intent, direction);
                (void)isClut;
            }
        }

        // 6. cmsGetHeaderModel
        model = cmsGetHeaderModel(hProfile);
        (void)model;

        // Cleanup profile
        cleanup_profile(hProfile);
    }

    // 5. cmsGetEncodedCMMversion (no profile needed)
    cmmVersion = cmsGetEncodedCMMversion();
    (void)cmmVersion;

    return 0;
}