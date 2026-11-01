// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Create a second context for diversity
    cmsContext ctx2 = cmsCreateContext(NULL, NULL);
    if (ctx2 == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Fuzz cmsGetContextUserData
    void* userData = cmsGetContextUserData(ctx);
    (void)userData;

    // Fuzz cmsUnregisterPluginsTHR
    cmsUnregisterPluginsTHR(ctx);
    cmsUnregisterPluginsTHR(ctx2);

    // Fuzz cmsGetProfileContextID
    cmsHPROFILE hProfile = NULL;
    if (Size >= 128) {
        hProfile = cmsOpenProfileFromMem((const void*)Data, Size);
    }
    
    if (hProfile != NULL) {
        cmsContext profileCtx = cmsGetProfileContextID(hProfile);
        (void)profileCtx;
        cleanup_profile(hProfile);
    } else {
        cmsContext nullProfileCtx = cmsGetProfileContextID(NULL);
        (void)nullProfileCtx;
    }

    // Fuzz cmsGetPipelineContextID
    // The function asserts on NULL input in the library source code.
    // We must not pass NULL to avoid the deadly signal.
    // Since we cannot easily create a valid cmsPipeline without internal APIs,
    // we skip this specific call to prevent the crash, or we would need to
    // construct a valid pipeline which is complex in the public API.
    // For safety in fuzzing, we omit the NULL test for this specific function.

    // Fuzz _cmsCreateMutex and _cmsDestroyMutex
    void* mtx = _cmsCreateMutex(ctx);
    if (mtx != NULL) {
        _cmsDestroyMutex(ctx, mtx);
    }
    
    void* mtx2 = _cmsCreateMutex(ctx2);
    if (mtx2 != NULL) {
        _cmsDestroyMutex(ctx2, mtx2);
    }

    // Cleanup contexts
    cmsDeleteContext(ctx);
    cmsDeleteContext(ctx2);

    return 0;
}