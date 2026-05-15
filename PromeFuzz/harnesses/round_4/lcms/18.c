// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
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

static cmsContext create_test_context(void) {
    // Create a context with default memory manager
    return cmsCreateContext(NULL, NULL);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Create a context for all operations
    cmsContext ctx = create_test_context();
    if (!ctx) {
        return 0;
    }

    // Use fuzzer input to generate allocation sizes
    cmsUInt32Number sizes[6] = {0};
    size_t offset = 0;
    
    // Extract up to 6 size values from the input
    for (int i = 0; i < 6 && offset + sizeof(cmsUInt32Number) <= Size; i++) {
        memcpy(&sizes[i], Data + offset, sizeof(cmsUInt32Number));
        offset += sizeof(cmsUInt32Number);
    }

    // Test cmsMD5alloc - allocate and free using _cmsFree (since cmsMD5free is not available)
    cmsHANDLE md5_handle = cmsMD5alloc(ctx);
    if (md5_handle) {
        _cmsFree(ctx, md5_handle);
    }

    // Test _cmsMalloc with various sizes
    void *malloc_ptr = NULL;
    if (Size > 0) {
        cmsUInt32Number malloc_size = sizes[0] % 1024; // Limit size for fuzzing
        malloc_ptr = _cmsMalloc(ctx, malloc_size);
    }

    // Test _cmsCalloc
    void *calloc_ptr = NULL;
    if (Size > sizeof(cmsUInt32Number)) {
        cmsUInt32Number num = (sizes[1] % 256) + 1; // Avoid zero
        cmsUInt32Number elem_size = (sizes[2] % 256) + 1; // Avoid zero
        calloc_ptr = _cmsCalloc(ctx, num, elem_size);
    }

    // Test _cmsMallocZero
    void *malloc_zero_ptr = NULL;
    if (Size > 3 * sizeof(cmsUInt32Number)) {
        cmsUInt32Number zero_size = sizes[3] % 1024;
        malloc_zero_ptr = _cmsMallocZero(ctx, zero_size);
    }

    // Test _cmsRealloc with the malloc_ptr if it was allocated
    if (malloc_ptr && Size > 4 * sizeof(cmsUInt32Number)) {
        cmsUInt32Number new_size = sizes[4] % 2048;
        void *realloc_ptr = _cmsRealloc(ctx, malloc_ptr, new_size);
        if (realloc_ptr) {
            malloc_ptr = realloc_ptr; // Update pointer if realloc succeeded
        }
    } else if (Size > 4 * sizeof(cmsUInt32Number)) {
        // Test realloc with NULL pointer (should act like malloc)
        cmsUInt32Number new_size = sizes[4] % 1024;
        void *realloc_ptr = _cmsRealloc(ctx, NULL, new_size);
        if (realloc_ptr) {
            // Clean up if allocated
            _cmsFree(ctx, realloc_ptr);
        }
    }

    // Clean up allocated memory
    if (malloc_ptr) {
        _cmsFree(ctx, malloc_ptr);
    }
    if (calloc_ptr) {
        _cmsFree(ctx, calloc_ptr);
    }
    if (malloc_zero_ptr) {
        _cmsFree(ctx, malloc_zero_ptr);
    }

    // Test with zero sizes (edge cases)
    void *zero_malloc = _cmsMalloc(ctx, 0);
    if (zero_malloc) {
        _cmsFree(ctx, zero_malloc);
    }
    
    void *zero_calloc = _cmsCalloc(ctx, 0, 10);
    if (zero_calloc) {
        _cmsFree(ctx, zero_calloc);
    }
    
    void *zero_malloc_zero = _cmsMallocZero(ctx, 0);
    if (zero_malloc_zero) {
        _cmsFree(ctx, zero_malloc_zero);
    }

    // Test realloc with zero size (should free and return NULL)
    void *dummy_ptr = _cmsMalloc(ctx, 10);
    if (dummy_ptr) {
        void *realloc_zero = _cmsRealloc(ctx, dummy_ptr, 0);
        // realloc_zero should be NULL, original pointer is freed
        // No need to free dummy_ptr as realloc with size 0 frees it
    }

    // Create another context to test isolation
    cmsContext ctx2 = create_test_context();
    if (ctx2) {
        // Test allocations in second context
        void *ctx2_alloc = _cmsMalloc(ctx2, 64);
        if (ctx2_alloc) {
            _cmsFree(ctx2, ctx2_alloc);
        }
        
        // Test MD5 in second context - allocate and free using _cmsFree
        cmsHANDLE md5_handle2 = cmsMD5alloc(ctx2);
        if (md5_handle2) {
            _cmsFree(ctx2, md5_handle2);
        }
        
        // Clean up second context
        cmsDeleteContext(ctx2);
    }

    // Clean up main context
    cmsDeleteContext(ctx);

    return 0;
}