// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

/*
 * Fuzz driver for lcms pipeline functions.
 * Target APIs:
 *  - _cmsPipelineSetOptimizationParameters
 *  - cmsPipelineFree
 *  - cmsPipelineInsertStage
 *  - cmsPipelineSetSaveAs8bitsFlag
 *  - cmsPipelineCat
 *  - cmsPipelineDup
 */

/* Dummy evaluator and lifecycle callbacks for _cmsPipelineSetOptimizationParameters */
/* Signature: void (*)(const cmsUInt16Number *, cmsUInt16Number *, const void *) */
static void DummyEval16(const cmsUInt16Number *In, cmsUInt16Number *Out, const void *Data) {
    (void)In;
    (void)Out;
    (void)Data;
    /* Do nothing or simple copy if needed, but for fuzzing empty is fine */
}

/* Signature: void (*)(cmsContext, void *) */
static void DummyFreeUserData(cmsContext Context, void *Data) {
    (void)Context;
    if (Data) {
        free(Data);
    }
}

/* Signature: void *(*)(cmsContext, const void *) */
static void* DummyDupUserData(cmsContext Context, const void *Data) {
    (void)Context;
    if (!Data) return NULL;
    /* Simple allocation for fuzzing, size is unknown so we just return a new block or NULL */
    /* In a real scenario, we'd know the size. Here we return NULL to be safe or allocate a dummy */
    return malloc(1); 
}

/* Helper to create a simple empty pipeline */
static cmsPipeline* CreateEmptyPipeline(cmsContext ctx) {
    return cmsPipelineAlloc(ctx, 3, 3);
}

/* Helper to create a simple stage (identity-like) for insertion */
static cmsStage* CreateSimpleStage(cmsContext ctx) {
    return cmsStageAllocIdentity(ctx, 3);
}

int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    if (!Data || Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        ctx = NULL;
    }

    cmsPipeline* p1 = CreateEmptyPipeline(ctx);
    cmsPipeline* p2 = CreateEmptyPipeline(ctx);
    cmsStage* stage = CreateSimpleStage(ctx);

    /* Use input data to drive diverse behavior with bounds checking */
    uint8_t mode = 0;
    cmsBool onFlag = FALSE;
    cmsStageLoc loc = cmsAT_BEGIN;

    if (Size > 0) mode = Data[0] % 10;
    if (Size > 1) onFlag = (Data[1] % 2) ? TRUE : FALSE;
    if (Size > 2) loc = (Data[2] % 2) ? cmsAT_BEGIN : cmsAT_END;

    /* 1. Test _cmsPipelineSetOptimizationParameters */
    if (p1) {
        _cmsPipelineSetOptimizationParameters(p1, DummyEval16, NULL, NULL, NULL);
        if (mode == 1) {
            void* priv = malloc(64);
            if (priv) {
                memset(priv, 0, 64);
                _cmsPipelineSetOptimizationParameters(p1, DummyEval16, priv, DummyFreeUserData, DummyDupUserData);
            }
        }
        if (mode == 2) {
            _cmsPipelineSetOptimizationParameters(p1, NULL, NULL, NULL, NULL);
        }
    }

    /* 2. Test cmsPipelineInsertStage */
    if (p1 && stage) {
        cmsPipelineInsertStage(p1, loc, stage);
        /* Try inserting again at different location */
        cmsPipelineInsertStage(p1, loc == cmsAT_BEGIN ? cmsAT_END : cmsAT_BEGIN, stage);
    }

    /* 3. Test cmsPipelineSetSaveAs8bitsFlag */
    if (p1) {
        cmsBool old = cmsPipelineSetSaveAs8bitsFlag(p1, onFlag);
        (void)old;
        cmsPipelineSetSaveAs8bitsFlag(p1, !onFlag);
    }

    /* 4. Test cmsPipelineDup */
    cmsPipeline* p1_dup = NULL;
    if (p1) {
        p1_dup = cmsPipelineDup(p1);
    }

    /* 5. Test cmsPipelineCat */
    if (p1 && p2) {
        cmsPipelineCat(p1, p2);
    }
    if (p1_dup && p2) {
        cmsPipelineCat(p1_dup, p2);
    }
    if (p1 && p1_dup) {
        cmsPipelineCat(p1, p1_dup);
    }

    /* 6. Test cmsPipelineFree on all created pipelines */
    cmsPipelineFree(p1);
    cmsPipelineFree(p2);
    cmsPipelineFree(p1_dup);

    /* Free stage if not already consumed/freed by pipeline */
    /* If p1 was NULL, stage was never inserted, so we must free it manually. */
    /* If p1 was not NULL, stage was inserted and freed with p1. */
    if (!p1 && stage) {
        cmsStageFree(stage);
    }

    /* Cleanup context */
    cmsDeleteContext(ctx);

    return 0;
}