// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsGetPipelineContextID at cmslut.c:1407:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
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

static void cleanup_handle(cmsContext ctx, cmsHANDLE handle) {
    if (handle != NULL) {
        // Use _cmsFree for internal handles allocated via context-specific plugins
        // Note: _cmsFree is the internal counterpart to _cmsMalloc/_cmsCalloc
        _cmsFree(ctx, handle);
    }
}

static void cleanup_pipeline(cmsContext ctx, cmsPipeline *lut) {
    if (lut != NULL) {
        cmsPipelineFree(lut);
    }
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Test 1: cmsIT8LoadFromMem
    // Loads IT8 color data from a memory buffer.
    cmsHANDLE it8_handle = cmsIT8LoadFromMem(ctx, Data, Size);
    if (it8_handle != NULL) {
        // IT8 handles are allocated internally; use _cmsFree if allocated via plugin context
        // However, standard practice for IT8 handles is often cmsFree or specific cleanup.
        // Given the error, we use _cmsFree which is available in lcms2_plugin.h
        _cmsFree(ctx, it8_handle);
    }

    // Test 3: cmsGetPipelineContextID
    // Create a simple pipeline to test context retrieval
    cmsPipeline *lut = cmsPipelineAlloc(ctx, 3, 3);
    if (lut != NULL) {
        cmsContext pipeline_ctx = cmsGetPipelineContextID(lut);
        if (pipeline_ctx != NULL) {
            // Verify context matches (no-op for fuzzing, just ensures no crash)
            (void)pipeline_ctx;
        }
        cmsPipelineFree(lut);
    }

    // Test 4: cmsDictAlloc
    // Allocates a dictionary structure within the context
    cmsHANDLE dict_handle = cmsDictAlloc(ctx);
    if (dict_handle != NULL) {
        _cmsFree(ctx, dict_handle);
    }

    // Additional exploration: Try with different data sizes and patterns
    // Reuse the same context for multiple operations to test state changes
    cmsHANDLE it8_handle2 = cmsIT8LoadFromMem(ctx, Data, Size > 100 ? Size - 100 : Size);
    if (it8_handle2 != NULL) {
        _cmsFree(ctx, it8_handle2);
    }

    cmsHANDLE dict_handle2 = cmsDictAlloc(ctx);
    if (dict_handle2 != NULL) {
        _cmsFree(ctx, dict_handle2);
    }

    // Cleanup
    cleanup_context(ctx);

    return 0;
}