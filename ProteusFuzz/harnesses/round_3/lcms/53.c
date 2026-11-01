// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

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

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Use data to seed context creation or user data
    void *userData = NULL;
    if (Size > 0) {
        userData = malloc(Size);
        if (userData == NULL) return 0;
        memcpy(userData, Data, Size);
    }

    // Test cmsDupContext with NULL (default context)
    cmsContext ctx1 = cmsDupContext(NULL, userData);
    if (ctx1 != NULL) {
        // Test cmsCreateNULLProfileTHR
        cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx1);
        if (nullProfile != NULL) {
            // Test cmsGetProfileContextID
            cmsContext ctxFromProfile = cmsGetProfileContextID(nullProfile);
            (void)ctxFromProfile; // Use to avoid unused warning
            cleanup_profile(nullProfile);
        }

        // Test cmsCreateXYZProfileTHR
        cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(ctx1);
        if (xyzProfile != NULL) {
            cmsContext ctxFromXyzProfile = cmsGetProfileContextID(xyzProfile);
            (void)ctxFromXyzProfile;
            
            // Create a simple transform to test cmsGetTransformContextID
            // Use CMS_XYZ and CMSUInt32Number constants instead of strings
            cmsHTRANSFORM trans = cmsCreateTransform(xyzProfile, cmsSigXYZData, xyzProfile, cmsSigXYZData, 8, 0);
            if (trans != NULL) {
                cmsContext ctxFromTrans = cmsGetTransformContextID(trans);
                (void)ctxFromTrans;
                cleanup_transform(trans);
            }
            
            cleanup_profile(xyzProfile);
        }

        // Test _cmsCreateMutex
        void *mutex = _cmsCreateMutex(ctx1);
        (void)mutex; // Mutex cleanup is handled by context deletion

        cleanup_context(ctx1);
    }

    // Test cmsDupContext with a newly created context
    cmsContext ctx2 = cmsDupContext(NULL, NULL);
    if (ctx2 != NULL) {
        cmsContext ctx3 = cmsDupContext(ctx2, userData);
        if (ctx3 != NULL) {
            cleanup_context(ctx3);
        }
        cleanup_context(ctx2);
    }

    // Test with NULL profile for cmsGetProfileContextID
    cmsContext ctxFromNullProfile = cmsGetProfileContextID(NULL);
    (void)ctxFromNullProfile;

    // Test with NULL transform for cmsGetTransformContextID
    cmsContext ctxFromNullTrans = cmsGetTransformContextID(NULL);
    (void)ctxFromNullTrans;

    if (userData != NULL) {
        free(userData);
    }
    return 0;
}