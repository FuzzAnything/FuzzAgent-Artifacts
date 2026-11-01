// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetEncodedCMMversion at cmserr.c:30:15 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_profile(cmsHPROFILE *hProfile) {
    if (hProfile && *hProfile) {
        cmsCloseProfile(*hProfile);
        *hProfile = NULL;
    }
}

int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number intent;
    cmsUInt32Number direction;
    cmsUInt32Number nBytes;
    cmsBool lIsFloat;

    // Test cmsGetEncodedCMMversion (no arguments, always safe)
    int version = cmsGetEncodedCMMversion();
    (void)version; // Suppress unused variable warning

    // Attempt to load a profile from input data if it looks like ICC data
    // ICC profiles start with a size field (4 bytes) and "acsp" signature
    if (Size >= 12) {
        uint32_t profile_size = 0;
        memcpy(&profile_size, Data, 4);
        if (profile_size > 0 && profile_size <= Size && profile_size <= 100 * 1024 * 1024) {
            hProfile = cmsOpenProfileFromMem(Data, Size);
        }
    }

    if (hProfile) {
        // Extract parameters from input data
        intent = (Size > 0) ? Data[0] % 4 : 0; // Valid intents: 0-3
        direction = (Size > 1) ? Data[1] % 3 : 0; // Valid directions: 0-2 (INPUT, OUTPUT, PROOF)
        nBytes = (Size > 2) ? (Data[2] % 8) + 1 : 1; // Valid bytes: 1-8
        lIsFloat = (Size > 3) ? (Data[3] % 2) : 0;

        // Test cmsIsIntentSupported
        cmsBool is_intent_supported = cmsIsIntentSupported(hProfile, intent, direction);
        (void)is_intent_supported;

        // Test cmsGetEncodedICCversion
        cmsUInt32Number icc_version = cmsGetEncodedICCversion(hProfile);
        (void)icc_version;

        // Test cmsIsCLUT
        cmsBool is_clut = cmsIsCLUT(hProfile, intent, direction);
        (void)is_clut;

        // Test cmsFormatterForPCSOfProfile
        cmsUInt32Number pcs_formatter = cmsFormatterForPCSOfProfile(hProfile, nBytes, lIsFloat);
        (void)pcs_formatter;

        // Test cmsGetHeaderModel
        cmsUInt32Number header_model = cmsGetHeaderModel(hProfile);
        (void)header_model;

        cleanup_profile(&hProfile);
    }

    // Additional test: Try to open a dummy file if data is insufficient for profile
    if (!hProfile) {
        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);

            hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
            if (hProfile) {
                intent = (Size > 0) ? Data[0] % 4 : 0;
                direction = (Size > 1) ? Data[1] % 3 : 0;
                nBytes = (Size > 2) ? (Data[2] % 8) + 1 : 1;
                lIsFloat = (Size > 3) ? (Data[3] % 2) : 0;

                cmsIsIntentSupported(hProfile, intent, direction);
                cmsGetEncodedICCversion(hProfile);
                cmsIsCLUT(hProfile, intent, direction);
                cmsFormatterForPCSOfProfile(hProfile, nBytes, lIsFloat);
                cmsGetHeaderModel(hProfile);

                cleanup_profile(&hProfile);
            }
        }
    }

    return 0;
}