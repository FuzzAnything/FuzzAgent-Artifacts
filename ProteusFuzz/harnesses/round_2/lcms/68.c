// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_api_functions(cmsHPROFILE hProfile, const uint8_t *Data, size_t Size) {
    if (hProfile == NULL) return;

    // Extract values from fuzz input for parameters
    cmsUInt32Number nBytes = (Size > 0) ? (Data[0] % 8) + 1 : 1; // 1-8
    cmsBool lIsFloat = (Size > 1) ? (Data[1] % 2) : 0;
    cmsUInt32Number Intent = (Size > 2) ? (Data[2] % 4) : 0; // 0-3
    cmsUInt32Number UsedDirection = (Size > 3) ? (Data[3] % 3) : 0; // 0-2 (INPUT/OUTPUT/PROOF)

    // 1. cmsFormatterForPCSOfProfile
    cmsUInt32Number formatterResult = cmsFormatterForPCSOfProfile(hProfile, nBytes, lIsFloat);
    (void)formatterResult;

    // 2. cmsIsCLUT
    cmsBool isClut = cmsIsCLUT(hProfile, Intent, UsedDirection);
    (void)isClut;

    // 3. cmsGetEncodedICCversion
    cmsUInt32Number version = cmsGetEncodedICCversion(hProfile);
    (void)version;

    // 4. cmsIsIntentSupported
    cmsBool intentSupported = cmsIsIntentSupported(hProfile, Intent, UsedDirection);
    (void)intentSupported;

    // 5. cmsGetHeaderModel
    cmsUInt32Number model = cmsGetHeaderModel(hProfile);
    (void)model;

    // 6. cmsGetHeaderFlags
    cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
    (void)flags;
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a dummy file if needed (not strictly needed for cmsOpenProfileFromMem, but per instructions)
    // We will use cmsOpenProfileFromMem directly with the fuzz data as potential profile content.
    // If the data is not a valid ICC profile, cmsOpenProfileFromMem will return NULL, which is safe.

    cmsHPROFILE hProfile = cmsOpenProfileFromMem((const uint8_t *)Data, Size);

    if (hProfile != NULL) {
        test_api_functions(hProfile, Data, Size);
        cmsCloseProfile(hProfile);
    }

    // Additional state exploration: Try to open a dummy file if the input suggests it
    // Write a small dummy file to test file-based opening if Size is large enough to contain a filename hint or just always try
    // Per instruction: "If the code requires a file, use "./dummy_file" and write the necessary data to it."
    // We will write the fuzz data to dummy_file and try to open it as a profile.
    FILE *f = fopen("./dummy_file", "wb");
    if (f != NULL) {
        fwrite(Data, 1, Size, f);
        fclose(f);

        cmsHPROFILE hProfileFile = cmsOpenProfileFromFile("./dummy_file", "r");
        if (hProfileFile != NULL) {
            test_api_functions(hProfileFile, Data, Size);
            cmsCloseProfile(hProfileFile);
        }
    }

    return 0;
}