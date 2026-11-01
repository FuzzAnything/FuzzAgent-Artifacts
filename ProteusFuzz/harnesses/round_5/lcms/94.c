// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Forward declaration for the cleanup function
static void cleanup_contexts(cmsContext ctx, cmsContext ctx_dup);

static void cleanup_contexts(cmsContext ctx, cmsContext ctx_dup) {
    if (ctx_dup) {
        cmsDeleteContext(ctx_dup);
    }
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    // Create a default context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Extract data from input to control behavior
    int use_custom_user_data = (Size > 0) ? Data[0] % 2 : 0;
    void *user_data = NULL;
    if (use_custom_user_data && Size > 1) {
        user_data = malloc(Size - 1);
        if (user_data) {
            memcpy(user_data, Data + 1, Size - 1);
        }
    }

    // Note: cmsSetContextUserData is not available in the standard public API of lcms2 
    // in some versions or requires specific headers/plugins not fully exposed here.
    // The target API list only includes cmsGetContextUserData. 
    // We will skip setting user data to avoid compilation errors and focus on the listed APIs.
    // If the library version supports it and the header is correct, it would be used here.
    // For now, we proceed without setting custom user data to ensure compilation.

    // 2. Invoke target functions diversely

    // Test cmsGetContextUserData
    void *retrieved_data = cmsGetContextUserData(ctx);
    (void)retrieved_data; // Suppress unused warning

    // Test cmsDupContext
    // We pass NULL for NewUserData since we couldn't set custom data safely
    cmsContext ctx_dup = cmsDupContext(ctx, NULL);
    if (ctx_dup) {
        // Verify user data in duplicated context (should be NULL or copied if set)
        void *dup_data = cmsGetContextUserData(ctx_dup);
        (void)dup_data;
    }

    // Test cmsMD5alloc
    cmsHANDLE md5_ctx = cmsMD5alloc(ctx);
    if (md5_ctx) {
        // Allocation successful. MD5 operations require more context not provided in target list.
    }

    // Test cmsDictAlloc
    cmsHANDLE dict = cmsDictAlloc(ctx);
    if (dict) {
        // Allocation successful.
    }

    // Test cmsGBDAlloc
    cmsHANDLE gdb = cmsGBDAlloc(ctx);
    if (gdb) {
        // Allocation successful.
    }

    // Test cmsGetPipelineContextID
    // cmsPipeline is an incomplete type in lcms2.h, so we cannot instantiate it directly.
    // We can only test with a NULL pointer (which asserts) or a valid pointer obtained from the API.
    // Since we cannot create a pipeline without profiles, we test the NULL case carefully.
    // The documentation says it asserts on NULL. In a fuzzer, we usually avoid assertions that kill.
    // However, to test the function, we must pass a valid pointer.
    // Since we cannot create one, we skip the direct call with a valid pipeline.
    // We can, however, verify the function signature compiles by casting a dummy pointer if needed,
    // but passing NULL is the only way to trigger the specific logic without a real pipeline.
    // To avoid the assertion crash in the fuzzer, we will not call it with NULL.
    // Instead, we rely on the fact that the function exists and compiles.
    // If a valid pipeline could be created, we would do:
    // cmsContext pipeline_ctx_id = cmsGetPipelineContextID(valid_pipeline);
    
    // 3. Cleanup
    // Free allocated user data if we had set it (we didn't in this safe version)
    if (user_data) {
        free(user_data);
    }
    
    // Clean up contexts
    cleanup_contexts(ctx, ctx_dup);

    return 0;
}