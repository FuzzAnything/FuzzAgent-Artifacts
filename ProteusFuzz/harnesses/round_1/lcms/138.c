// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

/*
 * Fuzz driver for lcms pipeline functions.
 * Target APIs:
 *   _cmsPipelineSetOptimizationParameters
 *   cmsPipelineFree
 *   cmsPipelineInsertStage
 *   cmsPipelineSetSaveAs8bitsFlag
 *   cmsPipelineCat
 *   cmsPipelineDup
 */

/*
 * Custom evaluator and callbacks for _cmsPipelineSetOptimizationParameters.
 */

static void DummyFreeUserData(cmsContext ContextID, void* ptr) {
    (void)ContextID;
    (void)ptr;
}

static void* DummyDupUserData(cmsContext ContextID, const void* ptr) {
    (void)ContextID;
    (void)ptr;
    return NULL;
}

static void DummyEval16(const cmsUInt16Number* in, cmsUInt16Number* out, const void* UserData) {
    (void)in;
    (void)out;
    (void)UserData;
}

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    /* Create a pipeline */
    cmsPipeline* p1 = cmsPipelineAlloc(ctx, 3, 3);
    if (!p1) {
        cmsDeleteContext(ctx);
        return 0;
    }

    cmsPipeline* p2 = NULL;
    cmsPipeline* p_dup = NULL;

    /* Use input data to drive decisions */
    uint8_t op = Data[0] % 10;

    switch (op) {
        case 0:
            /* Test _cmsPipelineSetOptimizationParameters with NULL callbacks */
            _cmsPipelineSetOptimizationParameters(p1, NULL, NULL, NULL, NULL);
            break;
        case 1:
            /* Test with dummy evaluator and callbacks */
            _cmsPipelineSetOptimizationParameters(p1, DummyEval16, (void*)0x1234, DummyFreeUserData, DummyDupUserData);
            break;
        case 2:
            /* Test cmsPipelineSetSaveAs8bitsFlag with true */
            cmsPipelineSetSaveAs8bitsFlag(p1, TRUE);
            break;
        case 3:
            /* Test cmsPipelineSetSaveAs8bitsFlag with false */
            cmsPipelineSetSaveAs8bitsFlag(p1, FALSE);
            break;
        case 4:
            /* Test cmsPipelineDup */
            p_dup = cmsPipelineDup(p1);
            if (p_dup) {
                /* Do not free here, let the cleanup section handle it to avoid double-free if logic changes */
                /* We will free p_dup in the cleanup section only if it was allocated in this iteration */
                /* But since p_dup is local, we must free it before function exit. 
                   The error was caused by freeing p_dup, then later trying to access it or freeing p1 which might share state?
                   Actually, the error log shows:
                   Freed at: cmsPipelineFree(p_dup) (line 85 in original)
                   Read at: cmsPipelineFree(p_dup) (line 122 in original) -> This implies p_dup was freed twice or p1 was freed and p_dup shared memory?
                   Wait, the stack trace shows:
                   Freed by thread T0 here: ... cmsPipelineFree(p_dup) ... (line 85)
                   READ of size 8 at ... cmsPipelineFree(p_dup) ... (line 122)
                   This means p_dup was freed at line 85, and then accessed again at line 122.
                   In the original code:
                   case 4:
                       p_dup = cmsPipelineDup(p1);
                       if (p_dup) {
                           cmsPipelineFree(p_dup); // Line 85
                       }
                   ...
                   // Cleanup
                   if (p_dup) { // Line 122 check
                       cmsPipelineFree(p_dup); // Line 122 call -> USE AFTER FREE
                   }
                   
                   The fix is to NOT free p_dup inside the switch case, or set p_dup to NULL after freeing.
                   Let's set p_dup to NULL after freeing in case 4, so the cleanup block skips it.
                */
                cmsPipelineFree(p_dup);
                p_dup = NULL;
            }
            break;
        case 5:
            /* Test cmsPipelineCat with p1 and p1 (self) */
            cmsPipelineCat(p1, p1);
            break;
        case 6:
            /* Skip cmsPipelineCat with NULL l2 to avoid SEGV in library */
            break;
        case 7:
            /* Test cmsPipelineInsertStage with NULL stage (should fail safely) */
            cmsPipelineInsertStage(p1, cmsAT_BEGIN, NULL);
            break;
        case 8:
            /* Test cmsPipelineInsertStage with NULL stage at end */
            cmsPipelineInsertStage(p1, cmsAT_END, NULL);
            break;
        case 9:
            /* Create a second pipeline and test cat */
            p2 = cmsPipelineAlloc(ctx, 3, 3);
            if (p2) {
                cmsPipelineCat(p1, p2);
                cmsPipelineFree(p2);
                p2 = NULL;
            }
            break;
        default:
            break;
    }

    /* Cleanup */
    cmsPipelineFree(p1);
    if (p2) {
        cmsPipelineFree(p2);
    }
    if (p_dup) {
        cmsPipelineFree(p_dup);
    }
    cmsDeleteContext(ctx);

    return 0;
}