// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static cmsHPROFILE create_test_profile(cmsContext *ctx) {
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        return NULL;
    }
    return hProfile;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext *ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsHPROFILE hProfile = create_test_profile(ctx);
    if (hProfile == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Extract values from input data to fuzz parameters
    cmsUInt32Number flags = 0;
    cmsUInt32Number manufacturer = 0;
    cmsUInt32Number version = 0;
    cmsUInt32Number endian_value = 0;

    if (Size >= 4) {
        memcpy(&flags, Data, 4);
    }
    if (Size >= 8) {
        memcpy(&manufacturer, Data + 4, 4);
    }
    if (Size >= 12) {
        memcpy(&version, Data + 8, 4);
    }
    if (Size >= 16) {
        memcpy(&endian_value, Data + 12, 4);
    }

    // Fuzz cmsSetHeaderFlags
    cmsSetHeaderFlags(hProfile, flags);

    // Fuzz cmsSetHeaderManufacturer
    cmsSetHeaderManufacturer(hProfile, manufacturer);

    // Fuzz cmsGetEncodedICCversion
    cmsUInt32Number retrieved_version = cmsGetEncodedICCversion(hProfile);
    (void)retrieved_version; // Suppress unused variable warning

    // Fuzz cmsSetEncodedICCversion
    cmsSetEncodedICCversion(hProfile, version);

    // Fuzz cmsGetHeaderCreator
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    (void)creator; // Suppress unused variable warning

    // Fuzz _cmsAdjustEndianess32
    cmsUInt32Number adjusted_endian = _cmsAdjustEndianess32(endian_value);
    (void)adjusted_endian; // Suppress unused variable warning

    // Cleanup
    cleanup_profile(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}