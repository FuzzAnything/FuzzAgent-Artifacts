// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_mutex(cmsContext ctx, void* mtx) {
    if (ctx != NULL && mtx != NULL) {
        _cmsUnlockMutex(ctx, mtx);
        _cmsDestroyMutex(ctx, mtx);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_195(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsContext ctx2 = cmsCreateContext(NULL, NULL);
    if (ctx2 == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Test _cmsCreateMutex
    void* mtx = _cmsCreateMutex(ctx);
    if (mtx != NULL) {
        _cmsLockMutex(ctx, mtx);
        _cmsUnlockMutex(ctx, mtx);
        _cmsDestroyMutex(ctx, mtx);
    }

    // Test _cmsCreateMutex with second context
    void* mtx2 = _cmsCreateMutex(ctx2);
    if (mtx2 != NULL) {
        _cmsLockMutex(ctx2, mtx2);
        _cmsUnlockMutex(ctx2, mtx2);
        _cmsDestroyMutex(ctx2, mtx2);
    }

    // Create a dummy profile for testing cmsGetProfileContextID
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile != NULL) {
        cmsContext profileCtx = cmsGetProfileContextID(hProfile);
        (void)profileCtx; // Use the context ID if needed
        cleanup_profile(hProfile);
    }

    // Test cmsUnregisterPluginsTHR
    cmsUnregisterPluginsTHR(ctx);
    cmsUnregisterPluginsTHR(ctx2);

    // Cleanup contexts
    cmsDeleteContext(ctx);
    cmsDeleteContext(ctx2);

    return 0;
}