// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// cmsUnregisterPlugins at cmsplugin.c:627:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        ctx = NULL;
    }

    cmsUInt32Number size_val = 0;
    cmsUInt32Number num_val = 0;
    cmsUInt32Number realloc_size = 0;

    if (Size >= 4) {
        size_val = *(const cmsUInt32Number*)(Data);
        if (Size >= 8) {
            num_val = *(const cmsUInt32Number*)(Data + 4);
            if (Size >= 12) {
                realloc_size = *(const cmsUInt32Number*)(Data + 8);
            }
        }
    }

    // Clamp values to reasonable limits to avoid immediate crashes or OOM
    if (size_val > 1024 * 1024) size_val = 1024 * 1024;
    if (num_val > 1000) num_val = 1000;
    if (realloc_size > 1024 * 1024) realloc_size = 1024 * 1024;

    // Test _cmsMallocZero
    void *ptr_zero = _cmsMallocZero(ctx, size_val);
    if (ptr_zero != NULL) {
        // Test _cmsRealloc on the allocated pointer
        void *ptr_realloc = _cmsRealloc(ctx, ptr_zero, realloc_size);
        if (ptr_realloc != NULL) {
            // Test _cmsDupMem on the reallocated pointer
            void *ptr_dup = _cmsDupMem(ctx, ptr_realloc, realloc_size);
            if (ptr_dup != NULL) {
                // No explicit free needed, context cleanup handles it
            }
        }
    }

    // Test _cmsCalloc
    void *ptr_calloc = _cmsCalloc(ctx, num_val, size_val);
    if (ptr_calloc != NULL) {
        // Try to reallocate the calloc'd memory
        void *ptr_calloc_realloc = _cmsRealloc(ctx, ptr_calloc, realloc_size);
        if (ptr_calloc_realloc != NULL) {
            // Try to dup it
            void *ptr_calloc_dup = _cmsDupMem(ctx, ptr_calloc_realloc, realloc_size);
            // No explicit free needed
        }
    }

    // Test _cmsDestroyMutex with a dummy mutex (void*)
    // We pass a pointer that was allocated via the context's allocator if possible,
    // or a standard malloc. However, _cmsDestroyMutex expects a mutex handle.
    // If the plugin's DestroyMutexPtr is not NULL, it will try to free the pointer.
    // To avoid double-free, we should NOT free the pointer ourselves if we pass it to _cmsDestroyMutex
    // and the plugin might free it.
    // Instead, we allocate via the context if possible, or just pass a dummy pointer and let the plugin handle it.
    // Since we don't know the plugin's behavior, we allocate a small buffer and pass it.
    // If the plugin frees it, we must not free it again.
    // In a real scenario, we would need to know if the plugin manages the memory.
    // For fuzzing, we accept the potential leak to avoid double-free.
    
    // Allocate a dummy mutex pointer
    void *dummy_mutex = malloc(100);
    if (dummy_mutex != NULL) {
        // Only call _cmsDestroyMutex if we have a valid context to avoid NULL pointer dereference in plugin
        if (ctx != NULL) {
            _cmsDestroyMutex(ctx, dummy_mutex);
        }
        // Do NOT free dummy_mutex here because _cmsDestroyMutex might have freed it.
        // If the plugin didn't free it, it's a leak, but double-free is worse.
    }

    // Test with NULL context for memory functions to ensure they handle it gracefully
    // We skip _cmsDestroyMutex with NULL context as it caused a crash in the plugin
    // We must free the returned pointers from NULL context calls to avoid leaks
    void *null_malloc_zero = _cmsMallocZero(NULL, size_val);
    if (null_malloc_zero != NULL) {
        free(null_malloc_zero);
    }

    void *null_calloc = _cmsCalloc(NULL, num_val, size_val);
    if (null_calloc != NULL) {
        free(null_calloc);
    }

    void *null_realloc = _cmsRealloc(NULL, NULL, realloc_size);
    if (null_realloc != NULL) {
        free(null_realloc);
    }

    void *null_dup = _cmsDupMem(NULL, NULL, size_val);
    if (null_dup != NULL) {
        free(null_dup);
    }

    // Cleanup context
    cleanup_context(ctx);

    // Call cmsUnregisterPlugins once to ensure it's safe
    cmsUnregisterPlugins();

    return 0;
}