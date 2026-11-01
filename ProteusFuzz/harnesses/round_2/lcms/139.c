// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
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

static cmsHPROFILE create_profile_from_data(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return NULL;
    }
    
    // Attempt to create a profile from the fuzzed data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    return hProfile;
}

int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size) {
    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number nBytes, intent, direction;
    cmsBool isFloat;
    cmsUInt32Number result;
    cmsBool boolResult;
    int cmmVersion;

    // Initialize variables with fuzzed data
    if (Size >= 4) {
        nBytes = Data[0] % 8 + 1; // Ensure nBytes is between 1 and 8
        intent = Data[1] % 4;     // Ensure intent is between 0 and 3
        direction = Data[2] % 3;  // Ensure direction is between 0 and 2
        isFloat = (Data[3] % 2) == 1;
    } else {
        nBytes = 1;
        intent = 0;
        direction = 0;
        isFloat = 0;
    }

    // Create a profile from the fuzzed data
    hProfile = create_profile_from_data(Data, Size);

    if (hProfile != NULL) {
        // Test cmsFormatterForPCSOfProfile
        result = cmsFormatterForPCSOfProfile(hProfile, nBytes, isFloat);
        (void)result; // Suppress unused variable warning

        // Test cmsIsCLUT
        boolResult = cmsIsCLUT(hProfile, intent, direction);
        (void)boolResult; // Suppress unused variable warning

        // Test cmsGetEncodedICCversion
        result = cmsGetEncodedICCversion(hProfile);
        (void)result; // Suppress unused variable warning

        // Test cmsIsIntentSupported
        boolResult = cmsIsIntentSupported(hProfile, intent, direction);
        (void)boolResult; // Suppress unused variable warning

        // Test cmsGetHeaderFlags
        result = cmsGetHeaderFlags(hProfile);
        (void)result; // Suppress unused variable warning

        // Cleanup the profile
        cleanup_profile(hProfile);
    }

    // Test cmsGetEncodedCMMversion (does not require a profile)
    cmmVersion = cmsGetEncodedCMMversion();
    (void)cmmVersion; // Suppress unused variable warning

    return 0;
}