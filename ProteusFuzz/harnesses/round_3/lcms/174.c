// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Dummy evaluator for 16-bit pipeline
static void dummy_eval16(const cmsUInt16Number* src, cmsUInt16Number* dst, const void* UserData) {
    (void)src;
    (void)dst;
    (void)UserData;
}

// Dummy free function for user data
static void dummy_free(cmsContext ID, void* Data) {
    (void)ID;
    if (Data == NULL) return;
    // Avoid freeing magic pointers or invalid addresses
    if ((uintptr_t)Data < 0x1000) return; 
    free(Data);
}

// Dummy dup function for user data
static void* dummy_dup(cmsContext ID, const void* Data) {
    (void)ID;
    if (Data == NULL) return NULL;
    // Return a magic pointer for magic inputs
    if (Data == (void*)0x1234 || Data == (void*)0x5678) {
        return (void*)0x9999;
    }
    // Allocate 1 byte for safety
    void* new_data = malloc(1);
    if (new_data) {
        memcpy(new_data, Data, 1);
    }
    return new_data;
}

int LLVMFuzzerTestOneInput_174(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsPipeline* p1 = cmsPipelineAlloc(ctx, 3, 3);
    cmsPipeline* p2 = cmsPipelineAlloc(ctx, 3, 3);
    cmsPipeline* p3 = NULL;

    if (p1 == NULL || p2 == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Fuzz cmsPipelineSetSaveAs8bitsFlag
    cmsBool flag = (Data[0] % 2);
    cmsPipelineSetSaveAs8bitsFlag(p1, flag);
    cmsPipelineSetSaveAs8bitsFlag(p2, !flag);

    // Allocate real data for the pipeline to avoid freeing magic pointers
    // Use uint8_t* to allow array access
    uint8_t* real_data_1 = malloc(1);
    uint8_t* real_data_2 = malloc(1);
    if (real_data_1) real_data_1[0] = 0;
    if (real_data_2) real_data_2[0] = 0;

    // Fuzz _cmsPipelineSetOptimizationParameters with real data
    _cmsPipelineSetOptimizationParameters(p1, dummy_eval16, real_data_1, dummy_free, dummy_dup);
    _cmsPipelineSetOptimizationParameters(p2, dummy_eval16, real_data_2, dummy_free, dummy_dup);

    // Fuzz cmsPipelineDup
    if (Size > 1 && Data[1] % 2) {
        p3 = cmsPipelineDup(p1);
    }
    if (Size > 2 && Data[2] % 2) {
        cmsPipeline* p4 = cmsPipelineDup(p2);
        if (p4) {
            cmsPipelineFree(p4);
        }
    }

    // Fuzz cmsPipelineCat
    if (Size > 3 && Data[3] % 2) {
        cmsPipelineCat(p1, p2);
    }
    if (Size > 4 && Data[4] % 2 && p3 != NULL) {
        cmsPipelineCat(p3, p1);
    }

    // Fuzz cmsPipelineInsertStage
    // Ensure we have enough data before accessing Data[5] and Data[6]
    if (Size > 6) {
        cmsStageLoc loc = (Data[6] % 2) ? cmsAT_BEGIN : cmsAT_END;
        cmsPipelineInsertStage(p1, loc, NULL);
    }

    // Cleanup
    cmsPipelineFree(p1);
    cmsPipelineFree(p2);
    cmsPipelineFree(p3);
    cmsDeleteContext(ctx);

    return 0;
}