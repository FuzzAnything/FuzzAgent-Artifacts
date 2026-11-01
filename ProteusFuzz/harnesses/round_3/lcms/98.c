// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsGetPCS at cmsio0.c:1069:34 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void dummy_cleanup(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size < 9) return 0;

    // Fuzz _cmsICCcolorSpace with various inputs derived from data
    int notation = Data[0];
    cmsColorSpaceSignature sig1 = _cmsICCcolorSpace(notation);

    // Create a dummy profile for testing other functions
    // cmsCreateProfilePlaceholder requires a cmsContext, use NULL for default context
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(NULL);
    if (!hProfile) return 0;

    // Fuzz cmsSetColorSpace
    // Ensure we have enough data before accessing indices 1-4
    cmsColorSpaceSignature sig2 = (cmsColorSpaceSignature)(Data[1] | (Data[2] << 8) | (Data[3] << 16) | (Data[4] << 24));
    cmsSetColorSpace(hProfile, sig2);

    // Fuzz cmsGetColorSpace
    cmsColorSpaceSignature retrievedSig = cmsGetColorSpace(hProfile);

    // Fuzz cmsSetPCS
    // Ensure we have enough data before accessing indices 5-8
    cmsColorSpaceSignature pcsSig = (cmsColorSpaceSignature)(Data[5] | (Data[6] << 8) | (Data[7] << 16) | (Data[8] << 24));
    cmsSetPCS(hProfile, pcsSig);

    // Fuzz cmsGetPCS
    cmsColorSpaceSignature retrievedPcs = cmsGetPCS(hProfile);

    // Fuzz cmsChannelsOfColorSpace with various signatures
    cmsInt32Number channels = cmsChannelsOfColorSpace(sig1);
    channels = cmsChannelsOfColorSpace(sig2);
    channels = cmsChannelsOfColorSpace(retrievedSig);
    channels = cmsChannelsOfColorSpace(pcsSig);
    channels = cmsChannelsOfColorSpace(retrievedPcs);

    // Additional fuzzing: try more notations
    for (size_t i = 0; i < Size && i < 100; i++) {
        int testNotation = Data[i];
        cmsColorSpaceSignature testSig = _cmsICCcolorSpace(testNotation);
        cmsInt32Number testChannels = cmsChannelsOfColorSpace(testSig);
        (void)testSig;
        (void)testChannels;
    }

    // Cleanup
    dummy_cleanup(hProfile);

    return 0;
}