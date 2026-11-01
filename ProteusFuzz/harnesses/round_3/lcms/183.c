// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static cmsHPROFILE create_dummy_profile(const uint8_t *Data, size_t Size) {
    cmsHPROFILE hProfile = NULL;

    // Try to load from data if it looks like a profile
    if (Size > 0) {
        hProfile = cmsOpenProfileFromMem(Data, Size);
    }

    // Fallback to creating a new one if loading fails or data is invalid
    if (hProfile == NULL) {
        hProfile = cmsCreate_sRGBProfile();
    }

    return hProfile;
}

int LLVMFuzzerTestOneInput_183(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = create_dummy_profile(Data, Size);
    if (hProfile == NULL) {
        return 0;
    }

    // Extract values from input data for fuzzing parameters
    cmsUInt32Number flags = 0;
    cmsUInt32Number manufacturer = 0;
    cmsUInt32Number version = 0;
    cmsUInt32Number endian_value = 0;

    if (Size >= 16) {
        memcpy(&flags, Data, 4);
        memcpy(&manufacturer, Data + 4, 4);
        memcpy(&version, Data + 8, 4);
        memcpy(&endian_value, Data + 12, 4);
    } else if (Size >= 4) {
        memcpy(&flags, Data, 4);
        manufacturer = flags;
        version = flags;
        endian_value = flags;
    } else if (Size >= 1) {
        flags = Data[0];
        manufacturer = Data[0];
        version = Data[0];
        endian_value = Data[0];
    }

    // Fuzz cmsSetHeaderFlags
    cmsSetHeaderFlags(hProfile, flags);

    // Fuzz cmsSetHeaderManufacturer
    cmsSetHeaderManufacturer(hProfile, manufacturer);

    // Fuzz cmsGetHeaderCreator
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    (void)creator; // Suppress unused variable warning

    // Fuzz cmsGetEncodedICCversion
    cmsUInt32Number current_version = cmsGetEncodedICCversion(hProfile);
    (void)current_version;

    // Fuzz cmsSetEncodedICCversion
    cmsSetEncodedICCversion(hProfile, version);

    // Fuzz _cmsAdjustEndianess32
    cmsUInt32Number adjusted_endian = _cmsAdjustEndianess32(endian_value);
    (void)adjusted_endian;

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}