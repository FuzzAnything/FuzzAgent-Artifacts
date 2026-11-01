// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// cmsUnregisterPlugins at cmsplugin.c:627:16 in lcms2.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Use a valid mutex or NULL to avoid invalid memory access.
// Since _cmsLockMutex handles NULL or missing plugin callbacks gracefully,
// we pass NULL to ensure no invalid pointer dereference occurs in the plugin callback.
static void* dummy_mutex = NULL;

int LLVMFuzzerTestOneInput_180(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a context for testing
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Use input data to decide which API to fuzz
    uint8_t mode = Data[0];
    uint8_t seed = Data[1];

    switch (mode) {
        case 0:
            // Fuzz _cmsLockMutex with NULL mutex to avoid invalid memory access
            _cmsLockMutex(ctx, dummy_mutex);
            break;

        case 1:
            // Fuzz cmsUnregisterPluginsTHR
            cmsUnregisterPluginsTHR(ctx);
            break;

        case 2:
            // Fuzz cmsPluginTHR with NULL plugin to test error handling
            cmsPluginTHR(ctx, NULL);
            break;

        case 3:
            // Fuzz _cmsDestroyMutex with NULL mutex
            _cmsDestroyMutex(ctx, dummy_mutex);
            break;

        case 4:
            // Fuzz cmsPlugin (global) with NULL
            cmsPlugin(NULL);
            break;

        case 5:
            // Fuzz cmsUnregisterPlugins (global)
            cmsUnregisterPlugins();
            break;

        default:
            // Randomly invoke a mix of functions based on seed
            if (seed % 2 == 0) {
                _cmsLockMutex(ctx, dummy_mutex);
            } else {
                cmsUnregisterPluginsTHR(ctx);
            }
            break;
    }

    // Cleanup: Destroy the context
    cmsDeleteContext(ctx);

    return 0;
}