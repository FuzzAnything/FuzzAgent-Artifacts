// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsChannelsOf at cmspcs.c:945:27 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsGetPCS at cmsio0.c:1069:34 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    // Attempt to create a profile from the provided data if it's large enough
    // Real profiles need specific structures, but for fuzzing we try to parse whatever is given
    if (size < 128) {
        return NULL;
    }
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (hProfile == NULL) {
        return NULL;
    }
    return hProfile;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Test _cmsICCcolorSpace with various inputs
    int notation = Data[0];
    cmsColorSpaceSignature sig = _cmsICCcolorSpace(notation);
    (void)sig; // Use the result to avoid unused variable warning

    // Test cmsChannelsOf with various color space signatures
    // Ensure we have enough data before accessing indices
    cmsColorSpaceSignature colorSpace = 0;
    if (Size >= 5) {
        colorSpace = (cmsColorSpaceSignature)(Data[1] << 24 | Data[2] << 16 | Data[3] << 8 | Data[4]);
    }
    cmsUInt32Number channels = cmsChannelsOf(colorSpace);
    (void)channels;

    // Attempt to create a profile for testing other functions
    cmsHPROFILE hProfile = create_dummy_profile(Data, Size);
    if (hProfile != NULL) {
        // Test cmsGetColorSpace
        cmsColorSpaceSignature retrievedColorSpace = cmsGetColorSpace(hProfile);
        (void)retrievedColorSpace;

        // Test cmsGetPCS
        cmsColorSpaceSignature pcs = cmsGetPCS(hProfile);
        (void)pcs;

        // Test cmsSetHeaderModel
        cmsUInt32Number model = 0;
        if (Size >= 9) {
            model = (cmsUInt32Number)(Data[5] << 24 | Data[6] << 16 | Data[7] << 8 | Data[8]);
        }
        cmsSetHeaderModel(hProfile, model);

        // Test cmsSetColorSpace
        cmsColorSpaceSignature newColorSpace = 0;
        if (Size >= 13) {
            newColorSpace = (cmsColorSpaceSignature)(Data[9] << 24 | Data[10] << 16 | Data[11] << 8 | Data[12]);
        }
        cmsSetColorSpace(hProfile, newColorSpace);

        // Cleanup
        cmsCloseProfile(hProfile);
    }

    return 0;
}