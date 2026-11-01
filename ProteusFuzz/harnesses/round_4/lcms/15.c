// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
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

static void cleanup_stage(cmsStage *stage) {
    if (stage != NULL) {
        cmsStageFree(stage);
    }
}

static void cleanup_pipeline(cmsPipeline *pipe) {
    if (pipe != NULL) {
        cmsPipelineFree(pipe);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Extract parameters from fuzz input
    cmsUInt32Number num = Data[0];
    cmsUInt32Number size = Data[1];
    cmsUInt32Number inputChan = Data[2];
    cmsUInt32Number outputChan = Data[3];

    // Limit values to reasonable ranges to avoid immediate crashes or OOM
    if (num > 10000) num = 10000;
    if (size > 10000) size = 10000;
    if (inputChan > 4) inputChan = 4;
    if (outputChan > 4) outputChan = 4;

    // 1. Test _cmsCalloc
    void *ptr_calloc = _cmsCalloc(ctx, num, size);
    if (ptr_calloc != NULL) {
        // Optional: write some data to ensure allocation is usable
        memset(ptr_calloc, 0x42, num * size);
        // Note: _cmsCalloc uses context-specific plugins. 
        // We rely on context cleanup for freeing, as there is no direct public free for this internal call.
    }

    // 2. Test cmsStageAllocCLut16bitGranular
    cmsUInt32Number clutPoints[4] = {1, 1, 1, 1};
    if (inputChan > 0 && inputChan <= 4) clutPoints[0] = (Size > 4) ? (Data[4] % 16) + 1 : 2;
    if (inputChan > 1 && inputChan <= 4) clutPoints[1] = (Size > 5) ? (Data[5] % 16) + 1 : 2;
    if (inputChan > 2 && inputChan <= 4) clutPoints[2] = (Size > 6) ? (Data[6] % 16) + 1 : 2;
    if (inputChan > 3 && inputChan <= 4) clutPoints[3] = (Size > 7) ? (Data[7] % 16) + 1 : 2;

    // Allocate a small table for the CLUT
    cmsUInt32Number tableSize = 1;
    for (int i = 0; i < inputChan; i++) tableSize *= clutPoints[i];
    tableSize *= outputChan;
    if (tableSize > 100000) tableSize = 100000;

    cmsUInt16Number *table = (cmsUInt16Number *)malloc(tableSize * sizeof(cmsUInt16Number));
    if (table != NULL) {
        memset(table, 0, tableSize * sizeof(cmsUInt16Number));
        cmsStage *stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table);
        cleanup_stage(stage);
        free(table);
    }

    // 3. Test _cmsMallocZero
    void *ptr_malloc_zero = _cmsMallocZero(ctx, size);
    if (ptr_malloc_zero != NULL) {
        // No direct free for _cmsMallocZero in public API, rely on context cleanup
    }

    // 4. Test _cmsRealloc
    void *ptr_realloc = _cmsRealloc(ctx, ptr_malloc_zero, size * 2);
    if (ptr_realloc != NULL) {
        // Realloc might have moved the pointer, so we update our reference
        // But we still don't have a direct free, so we skip explicit free
    } else {
        // If realloc failed, ptr_malloc_zero might still be valid, but we can't free it directly
    }

    // 5. Test cmsPipelineAlloc
    cmsPipeline *pipe = cmsPipelineAlloc(ctx, inputChan, outputChan);
    cleanup_pipeline(pipe);

    // 6. Test _cmsDupMem
    if (ptr_calloc != NULL) {
        void *ptr_dup = _cmsDupMem(ctx, ptr_calloc, num * size);
        // No direct free for _cmsDupMem, rely on context cleanup
    }

    // Cleanup context
    cleanup_context(ctx);

    return 0;
}