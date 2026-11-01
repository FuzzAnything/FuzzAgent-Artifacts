// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

/*
 * Fuzz driver for lcms pipeline functions.
 * Target APIs:
 *  - cmsPipelineDup
 *  - _cmsPipelineSetOptimizationParameters
 *  - cmsPipelineSetSaveAs8bitsFlag
 *  - cmsPipelineFree
 *  - cmsPipelineCat
 *  - cmsPipelineInsertStage
 */

static cmsContext g_ctx = NULL;

/* Dummy evaluator functions to satisfy callback requirements */
/* Signature: void (*)(const cmsUInt16Number *In, cmsUInt16Number *Out, const void *Data) */
static void _dummy_eval16(const cmsUInt16Number *In, cmsUInt16Number *Out, const void *Data) {
    (void)In;
    (void)Out;
    (void)Data;
}

/* Signature: void (*)(cmsContext ContextID, void *Data) */
static void _dummy_free(cmsContext ContextID, void *Data) {
    (void)ContextID;
    (void)Data;
}

/* Signature: void* (*)(cmsContext ContextID, const void *Data) */
static void* _dummy_dup(cmsContext ContextID, const void *Data) {
    (void)ContextID;
    (void)Data;
    return NULL;
}

int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    if (!g_ctx) {
        g_ctx = cmsCreateContext(NULL, NULL);
        if (!g_ctx) return 0;
    }

    cmsPipeline* p1 = NULL;
    cmsPipeline* p2 = NULL;
    cmsPipeline* p_dup = NULL;
    cmsBool save8_old = 0;
    cmsBool cat_res = 0;
    cmsBool insert_res = 0;

    /* Create initial pipelines if data indicates */
    if (Data[0] & 0x01) {
        p1 = cmsPipelineAlloc(g_ctx, 3, 3);
    }
    if (Data[0] & 0x02) {
        p2 = cmsPipelineAlloc(g_ctx, 3, 3);
    }

    /* Fuzz cmsPipelineDup */
    if (Data[0] & 0x04) {
        if (p1) {
            p_dup = cmsPipelineDup(p1);
            if (p_dup) {
                cmsPipelineFree(p_dup);
            }
        } else {
            p_dup = cmsPipelineDup(NULL);
            if (p_dup) {
                cmsPipelineFree(p_dup);
            }
        }
    }

    /* Fuzz _cmsPipelineSetOptimizationParameters */
    if (Data[0] & 0x08) {
        if (p1) {
            _cmsPipelineSetOptimizationParameters(p1, _dummy_eval16, (void*)0x1234, _dummy_free, _dummy_dup);
        }
        if (p2) {
            _cmsPipelineSetOptimizationParameters(p2, _dummy_eval16, (void*)0x5678, _dummy_free, _dummy_dup);
        }
    }

    /* Fuzz cmsPipelineSetSaveAs8bitsFlag */
    if (Data[0] & 0x10) {
        if (p1) {
            save8_old = cmsPipelineSetSaveAs8bitsFlag(p1, (Data[1] & 0x01) ? TRUE : FALSE);
            (void)save8_old;
        }
        if (p2) {
            save8_old = cmsPipelineSetSaveAs8bitsFlag(p2, (Data[1] & 0x02) ? TRUE : FALSE);
            (void)save8_old;
        }
    }

    /* Fuzz cmsPipelineCat */
    if (Data[0] & 0x20) {
        if (p1 && p2) {
            cat_res = cmsPipelineCat(p1, p2);
            (void)cat_res;
        } else if (p1) {
            cat_res = cmsPipelineCat(p1, NULL);
            (void)cat_res;
        } else if (p2) {
            cat_res = cmsPipelineCat(NULL, p2);
            (void)cat_res;
        } else {
            cat_res = cmsPipelineCat(NULL, NULL);
            (void)cat_res;
        }
    }

    /* Fuzz cmsPipelineInsertStage */
    /* Test with NULL stage to trigger error handling paths */
    if (Data[0] & 0x40) {
        if (p1) {
            insert_res = cmsPipelineInsertStage(p1, (Data[1] & 0x01) ? cmsAT_BEGIN : cmsAT_END, NULL);
            (void)insert_res;
        }
        if (p2) {
            insert_res = cmsPipelineInsertStage(p2, (Data[1] & 0x02) ? cmsAT_BEGIN : cmsAT_END, NULL);
            (void)insert_res;
        }
    }

    /* Cleanup */
    if (p1) cmsPipelineFree(p1);
    if (p2) cmsPipelineFree(p2);
    if (p_dup) cmsPipelineFree(p_dup);

    return 0;
}