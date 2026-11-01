// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext create_context(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return NULL;
    
    // Attempt to use input data to influence context if possible, 
    // though cmsCreateContext doesn't directly take data.
    // We rely on the global state or subsequent operations.
    return ctx;
}

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a context for thread-specific operations
    cmsContext ctx = create_context(Data, Size);
    if (ctx == NULL) {
        // If context creation fails, we can still test global functions
        // but most mutex functions require a valid context.
        // Try global plugin registration if possible, though we lack a real plugin.
        // Passing NULL or invalid data to cmsPlugin might trigger error handling.
        cmsPlugin(NULL); 
        return 0;
    }

    // Test 1: Create Mutex
    void *mtx = _cmsCreateMutex(ctx);
    
    // Test 2: Lock Mutex
    if (mtx != NULL) {
        cmsBool lockResult = _cmsLockMutex(ctx, mtx);
        // We don't check lockResult specifically for failure as it might be valid to fail
        // if no plugin is registered, but we proceed to unlock if we got a handle.
        
        // Test 3: Unlock Mutex
        _cmsUnlockMutex(ctx, mtx);
        
        // Test 4: Destroy Mutex
        _cmsDestroyMutex(ctx, mtx);
    }

    // Test 5: Register Plugin (Global)
    // We pass a pointer from the input data as a fake plugin.
    // This will likely fail validation inside lcms, which is what we want to test.
    // Ensure Size is large enough to avoid out-of-bounds if we were to dereference,
    // but cmsPlugin expects a specific struct. Passing raw data is safe for fuzzing
    // as long as the library handles invalid pointers gracefully.
    if (Size >= sizeof(void*)) {
        void *fake_plugin = (void*)Data;
        cmsPlugin(fake_plugin);
    }

    // Test 6: Register Plugin (Thread-specific)
    if (Size >= sizeof(void*)) {
        void *fake_plugin = (void*)Data;
        cmsPluginTHR(ctx, fake_plugin);
    }

    // Cleanup
    cleanup_context(ctx);

    return 0;
}