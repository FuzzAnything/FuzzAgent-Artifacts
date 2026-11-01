// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_resources(cmsContext ctx, void* ptr1, void* ptr2, void* ptr3, cmsStage* stage, cmsPipeline* pipe) {
    if (ptr1) {
        _cmsFree(ctx, ptr1);
    }
    if (ptr2) {
        _cmsFree(ctx, ptr2);
    }
    if (ptr3) {
        _cmsFree(ctx, ptr3);
    }
    if (stage) {
        cmsStageFree(stage);
    }
    if (pipe) {
        cmsPipelineFree(pipe);
    }
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely access indices up to 10
    if (Size < 12) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Extract parameters from fuzz data with bounds checking
    cmsUInt32Number size1 = Data[0] * 100 + 1;
    cmsUInt32Number size2 = Data[1] * 100 + 1;
    cmsUInt32Number num = Data[2];
    cmsUInt32Number elemSize = Data[3];
    cmsUInt32Number inputChan = (Data[4] % 5) + 1;
    cmsUInt32Number outputChan = (Data[5] % 5) + 1;
    cmsUInt32Number newReallocSize = Data[6] * 50 + 1;

    // Limit sizes to prevent OOM or excessive allocation
    if (size1 > 10240) size1 = 10240;
    if (size2 > 10240) size2 = 10240;
    if (elemSize > 1024) elemSize = 1024;
    if (num > 100) num = 100;
    if (newReallocSize > 10240) newReallocSize = 10240;

    // 1. Test _cmsDupMem
    void* src = _cmsMalloc(ctx, size1);
    if (src) {
        memset(src, 0xAA, size1);
        void* dup = _cmsDupMem(ctx, src, size1);
        if (dup) {
            // Just ensure it doesn't crash, free it
            _cmsFree(ctx, dup);
        }
        _cmsFree(ctx, src);
    }

    // 2. Test _cmsCalloc
    void* calloced = _cmsCalloc(ctx, num, elemSize);
    if (calloced) {
        // Verify zeroed
        // _cmsFree(ctx, calloced); // Handled in cleanup
    }

    // 3. Test _cmsMallocZero
    void* mallocZeroed = _cmsMallocZero(ctx, size2);
    if (mallocZeroed) {
        // Handled in cleanup
    }

    // 4. Test _cmsRealloc
    void* reallocPtr = _cmsMalloc(ctx, 10);
    if (reallocPtr) {
        void* realloced = _cmsRealloc(ctx, reallocPtr, newReallocSize);
        if (realloced) {
            _cmsFree(ctx, realloced);
        } else {
            _cmsFree(ctx, reallocPtr);
        }
    }

    // 5. Test cmsPipelineAlloc
    cmsPipeline* pipe = cmsPipelineAlloc(ctx, inputChan, outputChan);
    if (pipe) {
        // Handled in cleanup
    }

    // 6. Test cmsStageAllocCLut16bitGranular
    // Ensure inputChan and outputChan are valid (1-4) for the CLUT function
    // The library expects clutPoints to have at least inputChan elements.
    // We limit inputChan to 4 to match the array size and library constraints.
    if (inputChan > 4) inputChan = 4;
    if (outputChan > 4) outputChan = 4;

    cmsUInt32Number clutPoints[4] = {1, 1, 1, 1};
    
    // Use Data[7] to Data[10] for clutPoints, ensuring we don't read out of bounds
    // and that values are reasonable (>=1)
    for (int i = 0; i < inputChan; i++) {
        if (7 + i < Size) {
            // Ensure at least 1 point, cap at a small number to avoid huge allocations
            cmsUInt32Number val = Data[7 + i] % 5; 
            if (val == 0) val = 1;
            clutPoints[i] = val;
        } else {
            clutPoints[i] = 1;
        }
    }
    
    // Calculate total points safely
    cmsUInt32Number totalPoints = 1;
    for (int i = 0; i < inputChan; i++) {
        totalPoints *= clutPoints[i];
        // Prevent overflow or huge allocations
        if (totalPoints > 10000) {
            totalPoints = 10000;
            break;
        }
    }
    
    cmsUInt32Number tableSize = totalPoints * outputChan * sizeof(cmsUInt16Number);
    
    // Cap table size
    if (tableSize > 4096) tableSize = 4096;
    
    cmsUInt16Number* table = NULL;
    if (tableSize > 0) {
        table = (cmsUInt16Number*)_cmsMalloc(ctx, tableSize);
        if (table) {
            memset(table, 0, tableSize);
        }
    }

    cmsStage* stage = NULL;
    if (table) {
        stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table);
        if (stage) {
            // Handled in cleanup
        }
        _cmsFree(ctx, table);
    }

    // Cleanup
    cleanup_resources(ctx, calloced, mallocZeroed, NULL, stage, pipe);

    return 0;
}