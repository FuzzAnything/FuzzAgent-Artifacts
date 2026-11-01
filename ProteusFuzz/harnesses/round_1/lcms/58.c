// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateXYZProfile at cmsvirt.c:617:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsGetPCS at cmsio0.c:1069:34 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number profileSize = 0;
    const uint8_t *profileData = NULL;

    if (size < 128) {
        return NULL;
    }

    profileData = data;
    profileSize = size;

    hProfile = cmsOpenProfileFromMem(profileData, profileSize);
    
    if (hProfile == NULL) {
        cmsHPROFILE hXYZ = cmsCreateXYZProfile();
        if (hXYZ != NULL) {
            cmsCloseProfile(hXYZ);
        }
    }

    return hProfile;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = create_dummy_profile(Data, Size);
    cmsColorSpaceSignature pcsSig, csSig;
    cmsInt32Number channels;
    int notation;

    if (hProfile == NULL) {
        return 0;
    }

    if (Size > 0) {
        pcsSig = (cmsColorSpaceSignature)Data[0];
        csSig = (cmsColorSpaceSignature)Data[1 % Size];
        notation = (int)Data[2 % Size];
    } else {
        pcsSig = cmsSigXYZData;
        csSig = cmsSigRgbData;
        notation = 0;
    }

    cmsSetPCS(hProfile, pcsSig);
    cmsSetColorSpace(hProfile, csSig);

    cmsColorSpaceSignature retrievedPcs = cmsGetPCS(hProfile);
    cmsColorSpaceSignature retrievedCs = cmsGetColorSpace(hProfile);

    cmsColorSpaceSignature mappedSig = _cmsICCcolorSpace(notation);
    channels = cmsChannelsOfColorSpace(mappedSig);

    if (retrievedPcs != 0) {
        channels = cmsChannelsOfColorSpace(retrievedPcs);
    }
    if (retrievedCs != 0) {
        channels = cmsChannelsOfColorSpace(retrievedCs);
    }

    cmsCloseProfile(hProfile);

    return 0;
}