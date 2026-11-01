// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDictFree at cmsnamed.c:1101:16 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_resources(cmsContext ctx, cmsContext ctx_dup, cmsHANDLE gdb, cmsHANDLE dict, cmsHANDLE md5) {
    if (dict != NULL) {
        cmsDictFree(dict);
    }
    // Note: cmsMD5free is not always exposed in public headers, relying on context deletion.
    // Note: cmsGBDFree is internal, relying on context deletion.

    if (ctx_dup != NULL) {
        cmsDeleteContext(ctx_dup);
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = NULL;
    cmsContext ctx_dup = NULL;
    cmsHANDLE gdb = NULL;
    cmsHANDLE dict = NULL;
    cmsHANDLE md5 = NULL;
    void *user_data = NULL;

    // Step 1: Prepare environment
    // Create a context with a user data pointer derived from fuzz input
    // Since cmsSetContextUserData is not available in this version, we pass user_data directly to cmsCreateContext
    if (Size > 0) {
        size_t alloc_size = (Size > 1024) ? 1024 : Size;
        user_data = malloc(alloc_size);
        if (user_data != NULL) {
            memcpy(user_data, Data, alloc_size);
        }
    }

    ctx = cmsCreateContext(NULL, user_data);
    if (ctx == NULL) {
        if (user_data != NULL) free(user_data);
        return 0;
    }

    // Step 3: Explore program states by invoking target functions diversely

    // 1. cmsGBDAlloc
    gdb = cmsGBDAlloc(ctx);

    // 2. cmsDupContext
    // Try duplicating with the fuzz data as new user data
    cmsContext temp_ctx = cmsDupContext(ctx, (void*)Data);
    if (temp_ctx != NULL) {
        ctx_dup = temp_ctx;
    } else {
        // Fallback to duplicating with NULL user data
        ctx_dup = cmsDupContext(ctx, NULL);
    }

    // 3. cmsGetContextUserData
    void *retrieved_data = cmsGetContextUserData(ctx);
    if (retrieved_data != NULL) {
        volatile char *check = (volatile char *)retrieved_data;
        (void)check; 
    }

    // 4. cmsDictAlloc
    dict = cmsDictAlloc(ctx);

    // 5. cmsMD5alloc
    md5 = cmsMD5alloc(ctx);

    // Additional exploration on duplicated context
    if (ctx_dup != NULL) {
        cmsHANDLE gdb_dup = cmsGBDAlloc(ctx_dup);
        (void)gdb_dup;
        
        cmsHANDLE dict_dup = cmsDictAlloc(ctx_dup);
        if (dict_dup != NULL) {
            cmsDictFree(dict_dup);
        }

        cmsHANDLE md5_dup = cmsMD5alloc(ctx_dup);
        (void)md5_dup;
    }

    // Step 2: Cleanup
    cleanup_resources(ctx, ctx_dup, gdb, dict, md5);
    if (user_data != NULL) {
        free(user_data);
    }

    return 0;
}