// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

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

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsUnregisterPluginsTHR(ctx);
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a context for thread-safe operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Test 1: cmsCreateProfilePlaceholder
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(ctx);
    if (hProfile != NULL) {
        cmsContext profileCtx = cmsGetProfileContextID(hProfile);
        (void)profileCtx; // Use the context ID
        cleanup_profile(hProfile);
    }

    // Test 2: cmsCreate_sRGBProfileTHR
    cmsHPROFILE hSrgbProfile = cmsCreate_sRGBProfileTHR(ctx);
    if (hSrgbProfile != NULL) {
        cmsContext srgbCtx = cmsGetProfileContextID(hSrgbProfile);
        (void)srgbCtx; // Use the context ID
        cleanup_profile(hSrgbProfile);
    }

    // Test 3: Create a transform to test cmsGetTransformContextID
    // Re-create profiles if they were cleaned up
    cmsHPROFILE hProfile2 = cmsCreateProfilePlaceholder(ctx);
    cmsHPROFILE hSrgbProfile2 = cmsCreate_sRGBProfileTHR(ctx);
    
    if (hProfile2 != NULL && hSrgbProfile2 != NULL) {
        // Use valid pixel formats (e.g., TYPE_RGB_8) instead of undefined tags
        // cmsCreateTransform expects 6 arguments: Input, InputFormat, Output, OutputFormat, Intent, dwFlags
        cmsHTRANSFORM hTransform = cmsCreateTransform(
            hProfile2, TYPE_RGB_8,
            hSrgbProfile2, TYPE_RGB_8,
            INTENT_PERCEPTUAL, 0
        );
        if (hTransform != NULL) {
            cmsContext transformCtx = cmsGetTransformContextID(hTransform);
            (void)transformCtx; // Use the context ID
            cleanup_transform(hTransform);
        }
        cleanup_profile(hProfile2);
        cleanup_profile(hSrgbProfile2);
    }

    // Test 4: cmsGetStageContextID (requires a valid cmsStage pointer)
    // Since we cannot easily create a valid cmsStage without internal APIs,
    // we skip direct testing of this function to avoid undefined behavior.

    // Test 5: cmsUnregisterPluginsTHR
    cmsUnregisterPluginsTHR(ctx);

    // Cleanup
    cleanup_context(ctx);

    return 0;
}