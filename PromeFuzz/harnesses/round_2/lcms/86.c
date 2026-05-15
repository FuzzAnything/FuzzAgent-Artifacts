// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void* dummy_plugin = NULL;
static void* dummy_user_data = NULL;

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    // Create a context first since most functions require it
    cmsContext ctx = cmsCreateContext(dummy_plugin, dummy_user_data);
    if (!ctx) {
        return 0;
    }

    // Test _cmsCreateMutex
    void* mtx = _cmsCreateMutex(ctx);
    
    if (mtx) {
        // Test _cmsLockMutex
        cmsBool lock_result = _cmsLockMutex(ctx, mtx);
        
        // Test _cmsUnlockMutex
        _cmsUnlockMutex(ctx, mtx);
        
        // Test _cmsDestroyMutex
        _cmsDestroyMutex(ctx, mtx);
    }

    // Test _cmsGetTransformMaxWorkers with NULL pointer
    cmsInt32Number max_workers = _cmsGetTransformMaxWorkers(NULL);
    
    // Test with a small allocated buffer as dummy transform structure
    // We can't use sizeof on incomplete type, so allocate a small buffer
    if (Size > 0) {
        // Allocate a small buffer to use as dummy transform
        void* dummy_buffer = malloc(128);
        if (dummy_buffer) {
            memset(dummy_buffer, 0, 128);
            max_workers = _cmsGetTransformMaxWorkers((struct _cmstransform_struct*)dummy_buffer);
            free(dummy_buffer);
        }
    }

    return 0;
}