// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsGetPCS at cmsio0.c:1069:34 in lcms2.h
// cmsChannelsOf at cmspcs.c:945:27 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number model;
    cmsColorSpaceSignature sig;
    cmsColorSpaceSignature pcs;
    cmsColorSpaceSignature cs;
    cmsUInt32Number channels;
    int notation;

    // Extract values from input data safely
    model = *(const cmsUInt32Number *)(Data);
    sig = *(const cmsColorSpaceSignature *)(Data + 4);
    notation = *(const int *)(Data + 8);

    // Create a dummy profile. cmsCreateGrayProfile requires WhitePoint and TransferFunction.
    // We use NULL for both to create a default gray profile.
    hProfile = cmsCreateGrayProfile(NULL, NULL);
    if (hProfile == NULL) {
        return 0;
    }

    // Test cmsSetHeaderModel
    cmsSetHeaderModel(hProfile, model);

    // Test cmsSetColorSpace
    cmsSetColorSpace(hProfile, sig);

    // Test cmsGetColorSpace
    cs = cmsGetColorSpace(hProfile);
    (void)cs;

    // Test cmsGetPCS
    pcs = cmsGetPCS(hProfile);
    (void)pcs;

    // Test cmsChannelsOf
    channels = cmsChannelsOf(sig);
    (void)channels;

    // Test _cmsICCcolorSpace with fuzzed notation
    cmsColorSpaceSignature mappedSig = _cmsICCcolorSpace(notation);
    (void)mappedSig;

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}