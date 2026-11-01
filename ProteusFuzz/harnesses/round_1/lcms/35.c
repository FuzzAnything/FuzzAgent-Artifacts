// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateXYZProfile at cmsvirt.c:617:23 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
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

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    cmsHPROFILE hProfile = cmsCreateXYZProfile();
    if (hProfile == NULL) {
        return 0;
    }

    cmsUInt8Number profileID[16];
    cmsUInt32Number manufacturer;
    cmsUInt32Number version;

    // Initialize data from input
    memcpy(profileID, Data, 16);
    if (Size >= 32) {
        manufacturer = *(const cmsUInt32Number *)(Data + 16);
        version = *(const cmsUInt32Number *)(Data + 20);
    } else {
        manufacturer = 0;
        version = 0;
    }

    // Fuzz cmsSetHeaderProfileID
    cmsSetHeaderProfileID(hProfile, profileID);

    // Fuzz cmsGetHeaderProfileID
    cmsUInt8Number retrievedID[16];
    cmsGetHeaderProfileID(hProfile, retrievedID);

    // Fuzz cmsSetHeaderManufacturer
    cmsSetHeaderManufacturer(hProfile, manufacturer);

    // Fuzz cmsGetHeaderManufacturer
    cmsUInt32Number retrievedManufacturer = cmsGetHeaderManufacturer(hProfile);

    // Fuzz cmsSetEncodedICCversion
    cmsSetEncodedICCversion(hProfile, version);

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}