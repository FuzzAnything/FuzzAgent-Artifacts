// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_logger(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text) {
    // Do nothing - just a placeholder for logger
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    // Create a context first to use with other functions
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Use fuzzer input to decide which operations to perform
    if (Size > 0) {
        uint8_t operation = Data[0] % 6;
        
        switch (operation) {
            case 0: {
                // Test cmsDeleteContext with NULL (cleans global context)
                cmsDeleteContext(NULL);
                // Recreate our context since we still need it
                ctx = cmsCreateContext(NULL, NULL);
                break;
            }
            case 1: {
                // Test _cmsUnlockMutex
                void* mutex = _cmsCreateMutex(ctx);
                if (mutex) {
                    // Note: We're unlocking without locking first, which is what fuzzing should test
                    _cmsUnlockMutex(ctx, mutex);
                    _cmsDestroyMutex(ctx, mutex);
                }
                break;
            }
            case 2: {
                // Test _cmsDestroyMutex
                void* mutex = _cmsCreateMutex(ctx);
                if (mutex) {
                    _cmsDestroyMutex(ctx, mutex);
                }
                break;
            }
            case 3: {
                // Test cmsUnregisterPluginsTHR
                cmsUnregisterPluginsTHR(ctx);
                break;
            }
            case 4: {
                // Test cmsCreateContext with various parameters
                // Use fuzzer data for plugin and userdata pointers (cast carefully)
                void* plugin = (Size > 8) ? (void*)(Data + 1) : NULL;
                void* userdata = (Size > 16) ? (void*)(Data + 9) : NULL;
                
                cmsContext new_ctx = cmsCreateContext(plugin, userdata);
                if (new_ctx) {
                    cmsDeleteContext(new_ctx);
                }
                break;
            }
            case 5: {
                // Test _cmsCreateMutex and exercise it
                void* mutex = _cmsCreateMutex(ctx);
                if (mutex) {
                    // Try to unlock (without proper locking sequence)
                    _cmsUnlockMutex(ctx, mutex);
                    _cmsDestroyMutex(ctx, mutex);
                }
                break;
            }
        }
    }

    // Additional fuzzing: Try combinations of operations
    if (Size > 1) {
        uint8_t secondary_op = Data[1] % 3;
        
        switch (secondary_op) {
            case 0: {
                // Create and immediately destroy mutex
                void* mutex = _cmsCreateMutex(ctx);
                if (mutex) {
                    _cmsDestroyMutex(ctx, mutex);
                }
                break;
            }
            case 1: {
                // Unregister plugins and recreate context
                cmsUnregisterPluginsTHR(ctx);
                cmsDeleteContext(ctx);
                ctx = cmsCreateContext(NULL, NULL);
                break;
            }
            case 2: {
                // Test with invalid/null mutex
                _cmsUnlockMutex(ctx, NULL);
                _cmsDestroyMutex(ctx, NULL);
                break;
            }
        }
    }

    // Clean up our main context
    cmsDeleteContext(ctx);
    
    // Test one more deletion (should be safe)
    cmsDeleteContext(NULL);

    return 0;
}