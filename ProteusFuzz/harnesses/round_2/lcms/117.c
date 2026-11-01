// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateNULLProfile at cmsvirt.c:1018:23 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsGetPCS at cmsio0.c:1069:34 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
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

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsColorSpaceSignature pcs;
    cmsColorSpaceSignature sig;
    int ourNotation;

    // Create a NULL profile for testing
    hProfile = cmsCreateNULLProfile();
    if (hProfile == NULL) {
        return 0;
    }

    // Fuzz cmsGetColorSpace
    if (Size >= 1) {
        cmsGetColorSpace(hProfile);
    }

    // Fuzz cmsGetPCS
    if (Size >= 2) {
        pcs = cmsGetPCS(hProfile);
        (void)pcs;
    }

    // Fuzz cmsSetPCS with data from input
    if (Size >= 4) {
        pcs = *(const cmsColorSpaceSignature*)(Data);
        cmsSetPCS(hProfile, pcs);
    }

    // Fuzz _cmsICCcolorSpace with data from input
    if (Size >= 8) {
        ourNotation = *(const int*)(Data + 4);
        sig = _cmsICCcolorSpace(ourNotation);
        (void)sig;
    }

    // Fuzz cmsSetColorSpace with data from input
    if (Size >= 12) {
        sig = *(const cmsColorSpaceSignature*)(Data + 8);
        cmsSetColorSpace(hProfile, sig);
    }

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}