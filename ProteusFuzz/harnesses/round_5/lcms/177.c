// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsGetPipelineContextID at cmslut.c:1407:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_177(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = NULL;
    cmsContext dup_ctx = NULL;
    cmsHANDLE md5_handle = NULL;
    cmsHANDLE dict_handle = NULL;
    cmsHANDLE gdb_handle = NULL;
    cmsContext pipeline_ctx = NULL;
    void* user_data = NULL;
    cmsPipeline* p = NULL;

    // Initialize a default context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Set user data for testing cmsGetContextUserData
    // Using a pointer to the input data or a static value to ensure validity
    // Note: cmsSetContextUserData is not in the target list, so we skip setting it
    // and just test retrieval which returns NULL if not set, which is valid behavior.
    user_data = (void*)(uintptr_t)(Size % 1000);
    
    // Test cmsGetContextUserData (will return NULL as we didn't set it)
    void* retrieved_data = cmsGetContextUserData(ctx);
    (void)retrieved_data;

    // Test cmsDupContext
    dup_ctx = cmsDupContext(ctx, NULL);
    if (dup_ctx != NULL) {
        // Test cmsGetContextUserData on duplicated context
        void* dup_data = cmsGetContextUserData(dup_ctx);
        (void)dup_data;
    }

    // Test cmsMD5alloc
    md5_handle = cmsMD5alloc(ctx);
    if (md5_handle != NULL) {
        // Test with duplicated context as well
        cmsHANDLE md5_handle_dup = cmsMD5alloc(dup_ctx);
        (void)md5_handle_dup;
    }

    // Test cmsDictAlloc
    dict_handle = cmsDictAlloc(ctx);
    if (dict_handle != NULL) {
        cmsHANDLE dict_handle_dup = cmsDictAlloc(dup_ctx);
        (void)dict_handle_dup;
    }

    // Test cmsGBDAlloc
    gdb_handle = cmsGBDAlloc(ctx);
    if (gdb_handle != NULL) {
        cmsHANDLE gdb_handle_dup = cmsGBDAlloc(dup_ctx);
        (void)gdb_handle_dup;
    }

    // Test cmsGetPipelineContextID
    // Create a simple pipeline to test the function
    p = cmsPipelineAlloc(ctx, 3, 3);
    if (p != NULL) {
        pipeline_ctx = cmsGetPipelineContextID(p);
        (void)pipeline_ctx;
        cmsPipelineFree(p);
    }

    // Cleanup
    if (dup_ctx != NULL) {
        cmsDeleteContext(dup_ctx);
    }
    cleanup_context(ctx);

    return 0;
}