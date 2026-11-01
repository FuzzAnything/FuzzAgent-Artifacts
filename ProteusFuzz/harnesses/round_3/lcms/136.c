// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Fuzz cmsPluginTHR and cmsPlugin with dummy plugin data
    // We treat the input data as a potential plugin structure
    // Note: This is a simplified approach; real plugins have specific structures.
    // We cast Data to void* to pass to the API.
    if (Size >= sizeof(cmsPluginBase)) {
        cmsBool result = cmsPluginTHR(ctx, (void*)Data);
        (void)result; // Suppress unused variable warning
    }

    if (Size >= sizeof(cmsPluginBase)) {
        cmsBool result = cmsPlugin((void*)Data);
        (void)result;
    }

    // Fuzz mutex operations
    void* mtx = _cmsCreateMutex(ctx);
    if (mtx != NULL) {
        cmsBool locked = _cmsLockMutex(ctx, mtx);
        (void)locked;

        _cmsUnlockMutex(ctx, mtx);

        _cmsDestroyMutex(ctx, mtx);
        mtx = NULL; // Mark as destroyed
    }

    // Additional fuzzing: try locking/unlocking without creation (should be safe)
    // But per instructions, do not pass NULL to _cmsUnlockMutex or _cmsLockMutex
    // So we skip those if mtx is NULL.

    // Try creating mutex again and destroying immediately
    void* mtx2 = _cmsCreateMutex(ctx);
    if (mtx2 != NULL) {
        _cmsDestroyMutex(ctx, mtx2);
    }

    cleanup_context(ctx);
    return 0;
}