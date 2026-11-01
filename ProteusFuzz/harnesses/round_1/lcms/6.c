// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_context(cmsContext ctx) {
    if (ctx == NULL) return;
    cmsUnregisterPluginsTHR(ctx);
    cmsDeleteContext(ctx);
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile == NULL) return;
    cmsCloseProfile(profile);
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform == NULL) return;
    cmsDeleteTransform(transform);
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = NULL;
    cmsHPROFILE profile = NULL;
    cmsHTRANSFORM transform = NULL;
    void *mutex = NULL;

    // Create a context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Create a NULL profile using the context
    profile = cmsCreateNULLProfileTHR(ctx);

    // Test cmsGetProfileContextID
    cmsContext profile_ctx = cmsGetProfileContextID(profile);
    (void)profile_ctx; // Suppress unused variable warning

    // Create a transform if profile is valid
    if (profile != NULL) {
        cmsUInt32Number intent = 0; 
        transform = cmsCreateTransform(profile, TYPE_GRAY_8, profile, TYPE_GRAY_8, intent, 0);
    }

    // Test cmsGetTransformContextID
    cmsContext transform_ctx = cmsGetTransformContextID(transform);
    (void)transform_ctx; // Suppress unused variable warning

    // Test _cmsCreateMutex
    mutex = _cmsCreateMutex(ctx);
    if (mutex != NULL) {
        // The mutex is allocated within the context's memory pool.
        // It is automatically freed when the context is deleted.
        // We do not need to manually free it.
    }

    // Cleanup in reverse order of creation
    cleanup_transform(transform);
    cleanup_profile(profile);
    cleanup_context(ctx);

    return 0;
}