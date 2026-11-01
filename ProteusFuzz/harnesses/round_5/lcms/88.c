// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsContext ctx, cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_transform(cmsContext ctx, cmsHTRANSFORM transform) {
    if (transform != NULL) {
        cmsDeleteTransform(transform);
    }
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Fuzz cmsOpenProfileFromMemTHR
    cmsHPROFILE profile = cmsOpenProfileFromMemTHR(ctx, Data, Size);
    
    // Fuzz cmsGetHeaderRenderingIntent if profile is valid
    if (profile != NULL) {
        cmsUInt32Number intent = cmsGetHeaderRenderingIntent(profile);
        (void)intent; // Use the value to avoid unused warning
    }

    // Fuzz cmsGetSupportedIntentsTHR
    cmsUInt32Number codes[10];
    char *descriptions[10];
    cmsUInt32Number nMax = 10;
    cmsGetSupportedIntentsTHR(ctx, nMax, codes, descriptions);

    // Fuzz cmsCreateTransformTHR
    cmsHTRANSFORM transform1 = NULL;
    if (profile != NULL) {
        // Try with same profile for input and output
        transform1 = cmsCreateTransformTHR(ctx, profile, 0, profile, 0, 0, 0);
        if (transform1 == NULL) {
            // Try with NULL output (single profile transform)
            transform1 = cmsCreateTransformTHR(ctx, profile, 0, NULL, 0, 0, 0);
        }
    }

    // Fuzz cmsCreateMultiprofileTransformTHR
    cmsHPROFILE profiles[2];
    cmsUInt32Number nProfiles = 0;
    if (profile != NULL) {
        profiles[0] = profile;
        nProfiles = 1;
    }
    cmsHTRANSFORM transform2 = cmsCreateMultiprofileTransformTHR(ctx, profiles, nProfiles, 0, 0, 0, 0);

    // Fuzz cmsCreateProofingTransformTHR
    cmsHTRANSFORM transform3 = NULL;
    if (profile != NULL) {
        transform3 = cmsCreateProofingTransformTHR(ctx, profile, 0, profile, 0, profile, 0, 0, 0);
        if (transform3 == NULL) {
            // Try with NULL proofing profile
            transform3 = cmsCreateProofingTransformTHR(ctx, profile, 0, profile, 0, NULL, 0, 0, 0);
        }
    }

    // Cleanup
    cleanup_transform(ctx, transform1);
    cleanup_transform(ctx, transform2);
    cleanup_transform(ctx, transform3);
    cleanup_profile(ctx, profile);
    cmsDeleteContext(ctx);

    return 0;
}