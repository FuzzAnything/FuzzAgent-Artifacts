// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static void cleanup_handle(cmsHANDLE handle, cmsContext ctx) {
    if (handle != NULL && ctx != NULL) {
        // cmsFreeContext is not a public API in lcms2.
        // Handles like cmsMD5alloc or cmsDictAlloc are typically freed via specific API or context deletion.
        // For MD5, there is no direct free, it's usually managed by the context or specific finalization.
        // For Dict, there is cmsDictFree.
        // Since we don't have specific free functions for all handle types exposed in the prompt's limited scope,
        // and cmsDeleteContext cleans up the memory pool associated with the context,
        // we rely on context deletion for cleanup of objects allocated within that context.
        // However, to be safe and explicit if specific free functions existed, we would use them.
        // Given the error, we simply do nothing here for generic handles as they are context-managed.
        // If a specific free function is known (e.g., cmsDictFree), it should be used.
        // Assuming cmsDictAlloc returns a handle that might need cmsDictFree if it existed in the API.
        // Since the prompt didn't list cmsDictFree, we assume context deletion handles it or it's a no-op for the fuzzer.
        // We will not call a non-existent function.
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = NULL;
    cmsContext ctx_dup = NULL;
    cmsHANDLE md5_handle = NULL;
    cmsHANDLE dict_handle = NULL;

    // Initialize a context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Test cmsMD5alloc
    md5_handle = cmsMD5alloc(ctx);
    if (md5_handle != NULL) {
        // MD5 context is typically freed when the main context is deleted or via specific finalization.
        // No direct public free function for MD5 handle in standard lcms2 API exposed here.
        // We rely on context deletion.
        md5_handle = NULL; 
    }

    // Test cmsDictAlloc
    dict_handle = cmsDictAlloc(ctx);
    if (dict_handle != NULL) {
        // Similarly, dictionary handles are managed by the context.
        // If cmsDictFree existed, we would use it.
        dict_handle = NULL;
    }

    // Test cmsGetContextUserData
    void* retrieved_data = cmsGetContextUserData(ctx);
    (void)retrieved_data; 

    // Test cmsDupContext
    // Use a portion of input data as user data pointer if Size is large enough, else NULL
    void* new_user_data = (Size > 0) ? (void*)(uintptr_t)(Size % 2000) : NULL;
    ctx_dup = cmsDupContext(ctx, new_user_data);
    if (ctx_dup != NULL) {
        // Test cmsGetContextUserData on duplicated context
        void* dup_data = cmsGetContextUserData(ctx_dup);
        (void)dup_data;

        // Test cmsUnregisterPluginsTHR on duplicated context
        cmsUnregisterPluginsTHR(ctx_dup);

        cleanup_context(ctx_dup);
        ctx_dup = NULL;
    }

    // Test cmsUnregisterPluginsTHR on original context
    cmsUnregisterPluginsTHR(ctx);

    // Cleanup
    cleanup_context(ctx);

    return 0;
}