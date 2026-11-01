// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
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

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number intent = 0;
    cmsUInt32Number direction = 0;
    cmsUInt32Number nBytes = 0;
    cmsBool lIsFloat = 0;

    // Initialize variables from fuzz input
    if (Size >= 1) intent = Data[0] % 4; // 0-3
    if (Size >= 2) direction = Data[1] % 3; // 0-2 (INPUT, OUTPUT, PROOF)
    if (Size >= 3) nBytes = (Data[2] % 8) + 1; // 1-8
    if (Size >= 4) lIsFloat = (Data[3] % 2) != 0;

    // Try to create a profile from the input data
    // If the data is not a valid ICC profile, cmsOpenProfileFromMem will return NULL
    hProfile = cmsOpenProfileFromMem((const uint8_t *)Data, Size);

    if (hProfile == NULL) {
        // If profile creation fails, we can't test the APIs that require a valid profile
        return 0;
    }

    // Test cmsIsCLUT
    cmsBool isClut = cmsIsCLUT(hProfile, intent, direction);
    (void)isClut; // Suppress unused variable warning

    // Test cmsGetHeaderFlags
    cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
    (void)flags; // Suppress unused variable warning

    // Test cmsGetHeaderModel
    cmsUInt32Number model = cmsGetHeaderModel(hProfile);
    (void)model; // Suppress unused variable warning

    // Test cmsGetEncodedICCversion
    cmsUInt32Number version = cmsGetEncodedICCversion(hProfile);
    (void)version; // Suppress unused variable warning

    // Test cmsFormatterForPCSOfProfile
    cmsUInt32Number formatter = cmsFormatterForPCSOfProfile(hProfile, nBytes, lIsFloat);
    (void)formatter; // Suppress unused variable warning

    // Test cmsIsIntentSupported
    cmsBool isIntentSupported = cmsIsIntentSupported(hProfile, intent, direction);
    (void)isIntentSupported; // Suppress unused variable warning

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}