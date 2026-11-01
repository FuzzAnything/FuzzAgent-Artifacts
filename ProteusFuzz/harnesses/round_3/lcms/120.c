// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a new context for fuzzing
    cmsContext localCtx = cmsCreateContext(NULL, NULL);
    cmsContext ctxToUse = localCtx;

    if (localCtx == NULL) {
        return 0;
    }

    // Fuzz cmsGetProfileContextID with NULL profile
    cmsContext profileCtx = cmsGetProfileContextID(NULL);
    (void)profileCtx;

    // Fuzz cmsGetContextUserData
    void *userData = cmsGetContextUserData(ctxToUse);
    (void)userData;

    // Fuzz _cmsCreateMutex
    void *mtx = _cmsCreateMutex(ctxToUse);

    // Fuzz _cmsUnlockMutex
    if (mtx != NULL) {
        _cmsUnlockMutex(ctxToUse, mtx);
    }
    // Removed: _cmsUnlockMutex(ctxToUse, NULL); 
    // Reason: The library implementation does not handle NULL mutexes in defMtxUnlock,
    // causing a segfault when pthread_mutex_unlock is called on NULL.

    // Fuzz _cmsDestroyMutex
    if (mtx != NULL) {
        _cmsDestroyMutex(ctxToUse, mtx);
    }
    // Removed: _cmsDestroyMutex(ctxToUse, NULL);
    // Reason: Similar potential issues with NULL handling in internal implementations.

    // Fuzz cmsUnregisterPluginsTHR
    cmsUnregisterPluginsTHR(ctxToUse);

    // Cleanup
    if (localCtx != NULL) {
        cmsDeleteContext(localCtx);
    }

    return 0;
}