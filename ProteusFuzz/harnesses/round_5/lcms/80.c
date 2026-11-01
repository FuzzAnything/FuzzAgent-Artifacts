// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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

static void cleanup_transform(cmsHTRANSFORM hTransform) {
    if (hTransform != NULL) {
        cmsDeleteTransform(hTransform);
    }
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    // Attempt to open a profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ContextID, Data, Size);
    if (hProfile == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Test cmsGetHeaderRenderingIntent
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(hProfile);
    (void)intent; // Suppress unused variable warning

    // Test cmsGetSupportedIntentsTHR
    cmsUInt32Number nMax = 10;
    cmsUInt32Number *Codes = (cmsUInt32Number *)malloc(nMax * sizeof(cmsUInt32Number));
    char **Descriptions = (char **)malloc(nMax * sizeof(char *));
    if (Codes && Descriptions) {
        cmsGetSupportedIntentsTHR(ContextID, nMax, Codes, Descriptions);
        free(Codes);
        free(Descriptions);
    }

    // Create a second profile for transform testing (reusing the same data)
    cmsHPROFILE hProfile2 = cmsOpenProfileFromMemTHR(ContextID, Data, Size);
    if (hProfile2 == NULL) {
        cleanup_profile(hProfile);
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Test cmsCreateTransformTHR
    cmsHTRANSFORM hTransform = cmsCreateTransformTHR(ContextID, hProfile, 0, hProfile2, 0, 0, 0);
    cleanup_transform(hTransform);

    // Test cmsCreateMultiprofileTransformTHR
    cmsHPROFILE profiles[2] = {hProfile, hProfile2};
    hTransform = cmsCreateMultiprofileTransformTHR(ContextID, profiles, 2, 0, 0, 0, 0);
    cleanup_transform(hTransform);

    // Test cmsCreateProofingTransformTHR
    hTransform = cmsCreateProofingTransformTHR(ContextID, hProfile, 0, hProfile2, 0, hProfile2, 0, 0, 0);
    cleanup_transform(hTransform);

    // Cleanup
    cleanup_profile(hProfile);
    cleanup_profile(hProfile2);
    cmsDeleteContext(ContextID);

    return 0;
}