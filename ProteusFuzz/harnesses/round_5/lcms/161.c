// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Forward declarations for internal types to allow function pointer casting
struct _cmsContext_struct;
struct _cmsPipeline_struct;

// Helper to create a dummy pipeline
static cmsPipeline* create_dummy_pipeline(cmsContext ctx, cmsUInt32Number in, cmsUInt32Number out) {
    cmsPipeline* p = cmsPipelineAlloc(ctx, in, out);
    if (!p) return NULL;
    
    cmsStage* s = cmsStageAllocIdentity(ctx, in);
    if (!s) {
        cmsPipelineFree(p);
        return NULL;
    }
    
    if (!cmsPipelineInsertStage(p, cmsAT_BEGIN, s)) {
        cmsStageFree(s);
        cmsPipelineFree(p);
        return NULL;
    }
    
    return p;
}

// Custom callback: DupUserDataFn
// Signature: void *(*)(struct _cmsContext_struct *, const void *)
static void* custom_dup_data_impl(struct _cmsContext_struct *ctx, const void *data) {
    if (!data) return NULL;
    // We cannot know the size here, so we just return a dummy pointer or NULL
    // In a real scenario, size would be tracked. For fuzzing, we return NULL to avoid allocation issues
    // or a small static buffer if we wanted to be more aggressive, but NULL is safer.
    return NULL; 
}

// Custom callback: FreeUserDataFn
// Signature: void (*)(struct _cmsContext_struct *, void *)
static void custom_free_data_impl(struct _cmsContext_struct *ctx, void *data) {
    // Do nothing, or free if we allocated something real
    (void)ctx;
    (void)data;
}

// Custom callback: _cmsPipelineEval16Fn
// Signature: void (*)(const unsigned short *, unsigned short *, const void *)
// Note: The actual signature in lcms2 is complex, often involving the pipeline pointer implicitly or explicitly.
// Based on the error, it expects: void (*)(const unsigned short *, unsigned short *, const void *)
// We provide a stub that does nothing.
static void custom_eval16_impl(const cmsUInt16Number *src, cmsUInt16Number *dst, const void *data) {
    (void)src;
    (void)dst;
    (void)data;
}

int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Extract parameters from input
    cmsUInt32Number in_ch = (Data[0] % 10) + 1;
    cmsUInt32Number out_ch = (Data[1] % 10) + 1;
    cmsStageLoc loc = (Data[2] % 2) ? cmsAT_BEGIN : cmsAT_END;
    cmsBool flag = (Data[3] % 2);
    
    cmsPipeline* p1 = NULL;
    cmsPipeline* p2 = NULL;
    cmsPipeline* p_dup = NULL;
    cmsStage* stage = NULL;
    
    // Create initial pipelines
    p1 = create_dummy_pipeline(ctx, in_ch, out_ch);
    p2 = create_dummy_pipeline(ctx, out_ch, in_ch);
    
    if (!p1 || !p2) {
        cmsPipelineFree(p1);
        cmsPipelineFree(p2);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Test 1: cmsPipelineInsertStage
    if (Size > 4) {
        stage = cmsStageAllocIdentity(ctx, in_ch);
        if (stage) {
            cmsPipelineInsertStage(p1, loc, stage);
            // stage is now owned by p1
        }
    }
    
    // Test 2: cmsPipelineDup
    p_dup = cmsPipelineDup(p1);
    
    // Test 3: cmsPipelineCat
    cmsPipelineCat(p1, p2);
    
    // Test 4: cmsPipelineSetSaveAs8bitsFlag
    cmsBool prev_flag = cmsPipelineSetSaveAs8bitsFlag(p1, flag);
    (void)prev_flag;
    
    // Test 5: _cmsPipelineSetOptimizationParameters
    // Casting to the exact expected types to satisfy the compiler
    _cmsPipelineEval16Fn eval_fn = (_cmsPipelineEval16Fn)custom_eval16_impl;
    _cmsDupUserDataFn dup_fn = (_cmsDupUserDataFn)custom_dup_data_impl;
    _cmsFreeUserDataFn free_fn = (_cmsFreeUserDataFn)custom_free_data_impl;
    
    _cmsPipelineSetOptimizationParameters(p1, eval_fn, (void*)0x1234, free_fn, dup_fn);
    
    // Test 6: cmsPipelineFree
    cmsPipelineFree(p1);
    cmsPipelineFree(p2);
    cmsPipelineFree(p_dup);
    
    cmsDeleteContext(ctx);
    
    return 0;
}