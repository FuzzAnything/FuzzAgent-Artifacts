// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    // Test 1: Create context with NULL plugin
    cmsContext ctx1 = cmsCreateContext(NULL, NULL);
    if (!ctx1) {
        return 0;
    }
    
    // Test 2: Create mutex
    void* mutex1 = _cmsCreateMutex(ctx1);
    
    if (mutex1) {
        // Test 3: Lock mutex
        cmsBool locked1 = _cmsLockMutex(ctx1, mutex1);
        
        // Test 4: Unlock mutex if locked
        if (locked1) {
            _cmsUnlockMutex(ctx1, mutex1);
        }
        
        // Test 5: Try to lock/unlock multiple times based on input data
        if (Size > 0) {
            size_t iterations = Size % 10;
            for (size_t i = 0; i < iterations; i++) {
                if (_cmsLockMutex(ctx1, mutex1)) {
                    _cmsUnlockMutex(ctx1, mutex1);
                }
            }
        }
        
        // Test 6: Destroy mutex
        _cmsDestroyMutex(ctx1, mutex1);
    }
    
    // Test 7: Create another context and test mutex operations
    cmsContext ctx2 = cmsCreateContext(NULL, NULL);
    if (ctx2) {
        void* mutex2 = _cmsCreateMutex(ctx2);
        if (mutex2) {
            // Test lock/unlock in different order
            if (_cmsLockMutex(ctx2, mutex2)) {
                _cmsUnlockMutex(ctx2, mutex2);
            }
            _cmsDestroyMutex(ctx2, mutex2);
        }
    }
    
    // Test 8: Test with invalid context pointer (using fuzzer data)
    if (Size >= sizeof(void*)) {
        // Use first sizeof(void*) bytes as a fake context pointer
        cmsContext fake_ctx = (cmsContext)(uintptr_t)(*(const void**)Data);
        void* fake_mutex = _cmsCreateMutex(fake_ctx);
        if (fake_mutex) {
            _cmsLockMutex(fake_ctx, fake_mutex);
            _cmsUnlockMutex(fake_ctx, fake_mutex);
            _cmsDestroyMutex(fake_ctx, fake_mutex);
        }
    }
    
    // Test 9: Test with invalid mutex pointer (using fuzzer data)
    if (Size >= 2 * sizeof(void*)) {
        // Use second sizeof(void*) bytes as a fake mutex pointer
        void* fake_mutex2 = (void*)(uintptr_t)(*((const void**)(Data + sizeof(void*))));
        _cmsLockMutex(ctx1, fake_mutex2);
        _cmsUnlockMutex(ctx1, fake_mutex2);
        _cmsDestroyMutex(ctx1, fake_mutex2);
    }
    
    // Note: We cannot create a valid _cmstransform_struct because it's an opaque type
    // and we don't have access to its full definition or constructor functions
    // The _cmsGetTransformMaxWorkers function requires a valid transform structure
    // and passing NULL triggers an assertion, so we cannot call it safely
    
    // Cleanup: Note that cmsDeleteContext is not in our API list, so we cannot call it
    // The contexts will be leaked, but that's acceptable for fuzzing
    
    return 0;
}