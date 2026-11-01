// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number intent = 0;
    cmsUInt32Number direction = 0;
    cmsUInt32Number nBytes = 0;
    cmsBool isFloat = 0;

    // Attempt to create a profile from the fuzzed data
    hProfile = cmsOpenProfileFromMem(Data, Size);
    if (hProfile == NULL) {
        return 0;
    }

    // Extract values from input data if available
    if (Size >= 4) {
        intent = Data[0] % 4; // Valid intents are 0-3
        direction = Data[1] % 3; // INPUT, OUTPUT, PROOF (0-2)
        nBytes = (Data[2] % 8) + 1; // 1-8 bytes
        isFloat = (Data[3] % 2); // 0 or 1
    }

    // Call cmsGetHeaderModel
    cmsUInt32Number model = cmsGetHeaderModel(hProfile);
    (void)model; // Suppress unused variable warning

    // Call cmsIsCLUT
    cmsBool isClut = cmsIsCLUT(hProfile, intent, direction);
    (void)isClut;

    // Call cmsGetHeaderFlags
    cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
    (void)flags;

    // Call cmsGetEncodedICCversion
    cmsUInt32Number version = cmsGetEncodedICCversion(hProfile);
    (void)version;

    // Call cmsFormatterForPCSOfProfile
    cmsUInt32Number formatter = cmsFormatterForPCSOfProfile(hProfile, nBytes, isFloat);
    (void)formatter;

    // Call cmsIsIntentSupported
    cmsBool intentSupported = cmsIsIntentSupported(hProfile, intent, direction);
    (void)intentSupported;

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}