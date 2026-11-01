// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
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

static void fuzzer_test_color_space_mapping(const uint8_t *Data, size_t Size) {
    if (Size >= 4) {
        cmsColorSpaceSignature sig = *(const cmsColorSpaceSignature*)Data;
        int result = _cmsLCMScolorSpace(sig);
        (void)result;
    }

    if (Size >= 4) {
        int notation = *(const int*)Data;
        cmsColorSpaceSignature icsig = _cmsICCcolorSpace(notation);
        (void)icsig;
    }
}

static void fuzzer_test_profile_manipulation(const uint8_t *Data, size_t Size) {
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return;

    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(ContextID);
    if (hProfile == NULL) {
        cmsDeleteContext(ContextID);
        return;
    }

    if (Size >= 4) {
        cmsColorSpaceSignature pcs = *(const cmsColorSpaceSignature*)Data;
        cmsSetPCS(hProfile, pcs);
    }

    if (Size >= 4) {
        cmsColorSpaceSignature space = *(const cmsColorSpaceSignature*)Data;
        cmsSetColorSpace(hProfile, space);
    }

    cmsColorSpaceSignature retrievedSpace = cmsGetColorSpace(hProfile);
    cmsColorSpaceSignature retrievedPCS = cmsGetPCS(hProfile);
    
    (void)retrievedSpace;
    (void)retrievedPCS;

    cmsCloseProfile(hProfile);
    cmsDeleteContext(ContextID);
}

int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzzer_test_color_space_mapping(Data, Size);
    fuzzer_test_profile_manipulation(Data, Size);

    return 0;
}