// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void* dummy_plugin = NULL;

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    // Create a context first since most functions require it
    cmsContext context = cmsCreateContext(dummy_plugin, NULL);
    if (context == NULL) {
        return 0;
    }

    // Test _cmsCreateMutex
    void* mutex = _cmsCreateMutex(context);
    
    // Test _cmsLockMutex if mutex was created
    if (mutex != NULL) {
        cmsBool lock_result = _cmsLockMutex(context, mutex);
        
        // Test _cmsUnlockMutex if lock was successful
        if (lock_result) {
            _cmsUnlockMutex(context, mutex);
        }
    }
    
    // Test _cmsGetTransformMaxWorkers with NULL transform
    // This tests the function's handling of invalid input
    _cmsGetTransformMaxWorkers(NULL);
    
    // Test _cmsGetTransformMaxWorkers with a dummy pointer from fuzzer data
    // Since struct _cmstransform_struct is incomplete, we can't allocate it
    // But we can pass a pointer to fuzzer data to test the function
    if (Size > 0) {
        // Cast the fuzzer data to the expected pointer type
        struct _cmstransform_struct* dummy_transform = (struct _cmstransform_struct*)Data;
        _cmsGetTransformMaxWorkers(dummy_transform);
    }
    
    // Test _cmsDestroyMutex if mutex was created
    if (mutex != NULL) {
        _cmsDestroyMutex(context, mutex);
    }
    
    // Clean up context
    // Note: cmsDeleteContext is not in the target list but is needed for cleanup
    // We'll use it since it's available in lcms2.h
    cmsDeleteContext(context);
    
    return 0;
}