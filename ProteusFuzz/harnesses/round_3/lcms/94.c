// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsGetPCS at cmsio0.c:1069:34 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void test_color_space_mapping(const uint8_t *Data, size_t Size) {
    int notation = 0;
    if (Size > 0) {
        notation = Data[0];
    }
    
    cmsColorSpaceSignature sig = _cmsICCcolorSpace(notation);
    
    cmsInt32Number channels = cmsChannelsOfColorSpace(sig);
    (void)channels;
}

static void test_profile_operations(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return;
    }

    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(ctx);
    if (hProfile == NULL) {
        cmsDeleteContext(ctx);
        return;
    }

    cmsColorSpaceSignature spaceSig = 0;
    cmsColorSpaceSignature pcsSig = 0;

    if (Size > 0) {
        spaceSig = (cmsColorSpaceSignature)Data[0];
    }
    if (Size > 1) {
        pcsSig = (cmsColorSpaceSignature)Data[1];
    }

    cmsSetColorSpace(hProfile, spaceSig);
    cmsSetPCS(hProfile, pcsSig);

    cmsColorSpaceSignature retrievedSpace = cmsGetColorSpace(hProfile);
    cmsColorSpaceSignature retrievedPcs = cmsGetPCS(hProfile);

    (void)retrievedSpace;
    (void)retrievedPcs;

    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    test_color_space_mapping(Data, Size);
    test_profile_operations(Data, Size);

    return 0;
}