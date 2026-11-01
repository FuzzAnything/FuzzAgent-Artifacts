// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 128) {
        // Create a minimal valid profile if input is too small
        cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
        return hProfile;
    }

    // Try to create a profile from the fuzzed data if it looks like an ICC profile
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (hProfile == NULL) {
        // Fallback to sRGB if parsing fails
        hProfile = cmsCreate_sRGBProfile();
    }
    return hProfile;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = create_dummy_profile(Data, Size);
    if (hProfile == NULL) {
        return 0;
    }

    // Test cmsGetHeaderManufacturer
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hProfile);
    (void)manufacturer; // Suppress unused variable warning

    // Test cmsSetHeaderFlags
    cmsUInt32Number flags = 0;
    if (Size > 0) flags = Data[0];
    if (Size > 1) flags |= (Data[1] << 8);
    if (Size > 2) flags |= (Data[2] << 16);
    if (Size > 3) flags |= (Data[3] << 24);
    cmsSetHeaderFlags(hProfile, flags);

    // Test cmsGetHeaderProfileID
    cmsUInt8Number profileID[16] = {0};
    cmsGetHeaderProfileID(hProfile, profileID);

    // Test cmsSetHeaderProfileID
    if (Size >= 16) {
        cmsSetHeaderProfileID(hProfile, (cmsUInt8Number*)Data);
    } else {
        // Use a dummy ID if input is too small
        memset(profileID, 0x42, 16);
        cmsSetHeaderProfileID(hProfile, profileID);
    }

    // Test cmsSetEncodedICCversion
    cmsUInt32Number version = 0;
    if (Size >= 8) {
        version = Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24);
    }
    cmsSetEncodedICCversion(hProfile, version);

    // Test cmsFormatterForColorspaceOfProfile
    cmsUInt32Number nBytes = 1;
    cmsBool lIsFloat = 0; // Use 0 instead of cmsFALSE
    if (Size >= 10) {
        nBytes = (Data[8] % 3) + 1; // 1, 2, or 3 (though 3 might be invalid)
        lIsFloat = (Data[9] % 2) != 0;
    }
    cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(hProfile, nBytes, lIsFloat);
    (void)formatter;

    // Cleanup
    cmsCloseProfile(hProfile);

    return 0;
}