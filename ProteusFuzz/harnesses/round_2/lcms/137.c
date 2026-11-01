// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Dummy plugin callbacks
static void dummy_destroy_mutex(cmsContext ContextID, void* mtx) {
    (void)ContextID;
    if (mtx) {
        free(mtx);
    }
}

static cmsBool dummy_lock_mutex(cmsContext ContextID, void* mtx) {
    (void)ContextID;
    (void)mtx;
    return TRUE;
}

static void dummy_unlock_mutex(cmsContext ContextID, void* mtx) {
    (void)ContextID;
    (void)mtx;
}

static void* dummy_create_mutex(cmsContext ContextID) {
    (void)ContextID;
    void* mtx = malloc(sizeof(int));
    if (mtx) {
        memset(mtx, 0, sizeof(int));
    }
    return mtx;
}

// Initialize a dummy plugin structure.
// Since the exact fields of cmsPluginBase are internal and vary by version,
// we initialize it to zero and rely on the library's internal validation
// or simply pass a valid pointer to test the registration logic.
static cmsPluginBase dummy_plugin = {0};

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;

    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        // If context creation fails, try global plugin registration
        cmsPlugin(&dummy_plugin);
        return 0;
    }

    // Attempt to register the plugin for the thread/context
    // Note: This might fail if the plugin structure is not fully populated,
    // but that is expected behavior for fuzzing invalid inputs.
    cmsBool res1 = cmsPluginTHR(ctx, &dummy_plugin);
    (void)res1;

    // Test mutex creation
    void* mtx = _cmsCreateMutex(ctx);
    
    if (mtx) {
        // Test locking
        cmsBool lock_res = _cmsLockMutex(ctx, mtx);
        (void)lock_res;

        // Test unlocking
        _cmsUnlockMutex(ctx, mtx);

        // Test destruction
        _cmsDestroyMutex(ctx, mtx);
    }

    // Test global plugin registration
    cmsBool res2 = cmsPlugin(&dummy_plugin);
    (void)res2;

    // Cleanup
    cmsDeleteContext(ctx);

    return 0;
}