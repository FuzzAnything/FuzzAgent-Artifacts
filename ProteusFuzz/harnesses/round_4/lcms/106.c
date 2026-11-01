// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsUnregisterPlugins at cmsplugin.c:627:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

/* 
 * The cmsPluginBase structure fields are internal and not fully exposed in public headers.
 * To avoid compilation errors, we will not attempt to manually construct a cmsPluginBase.
 * Instead, we will focus on testing the API functions with valid contexts and NULL pointers
 * where appropriate, relying on the library's internal error handling.
 */

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    /* Create a context */
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    /* Test _cmsCreateMutex with valid context */
    void* mutex = _cmsCreateMutex(ctx);

    /* Test _cmsLockMutex if mutex was created */
    if (mutex) {
        cmsBool locked = _cmsLockMutex(ctx, mutex);
        (void)locked;

        /* Test _cmsDestroyMutex */
        _cmsDestroyMutex(ctx, mutex);
        mutex = NULL;
    }

    /* Test cmsPluginTHR with NULL plugin (should fail safely) */
    cmsBool ret = cmsPluginTHR(ctx, NULL);
    (void)ret;

    /* Test global plugin registration with NULL (should fail safely) */
    cmsBool global_ret = cmsPlugin(NULL);
    (void)global_ret;

    /* Cleanup context */
    cmsDeleteContext(ctx);

    /* Unregister plugins globally */
    cmsUnregisterPlugins();

    return 0;
}