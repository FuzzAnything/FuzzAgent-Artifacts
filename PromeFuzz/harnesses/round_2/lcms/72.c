// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

static cmsStage* create_dummy_stage(cmsContext ctx) {
    // Create a simple identity stage for testing
    cmsStage* stage = cmsStageAllocIdentity(ctx, 3);
    if (!stage) return NULL;
    
    return stage;
}

static cmsPipeline* create_dummy_pipeline(cmsContext ctx) {
    // Create a simple pipeline with identity stages
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (!pipeline) return NULL;
    
    cmsStage* stage1 = cmsStageAllocIdentity(ctx, 3);
    if (!stage1) {
        cmsPipelineFree(pipeline);
        return NULL;
    }
    
    cmsStage* stage2 = cmsStageAllocIdentity(ctx, 3);
    if (!stage2) {
        cmsStageFree(stage1);
        cmsPipelineFree(pipeline);
        return NULL;
    }
    
    cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage1);
    cmsPipelineInsertStage(pipeline, cmsAT_END, stage2);
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Use first byte to determine test path
    uint8_t test_case = Data[0] % 6;
    
    switch (test_case) {
        case 0: {
            // Test cmsStageDup and cmsStageFree
            cmsStage* original = create_dummy_stage(ctx);
            if (original) {
                cmsStage* duplicate = cmsStageDup(original);
                if (duplicate) {
                    cmsStageFree(duplicate);
                }
                cmsStageFree(original);
            }
            break;
        }
        
        case 1: {
            // Test pipeline traversal with cmsPipelineGetPtrToFirstStage and cmsStageNext
            cmsPipeline* pipeline = create_dummy_pipeline(ctx);
            if (pipeline) {
                cmsStage* current = cmsPipelineGetPtrToFirstStage(pipeline);
                while (current) {
                    // Access stage data
                    void* data = cmsStageData(current);
                    (void)data; // Use data to avoid unused variable warning
                    
                    // Move to next stage
                    current = cmsStageNext(current);
                }
                cmsPipelineFree(pipeline);
            }
            break;
        }
        
        case 2: {
            // Test cmsPipelineGetPtrToLastStage
            cmsPipeline* pipeline = create_dummy_pipeline(ctx);
            if (pipeline) {
                cmsStage* last = cmsPipelineGetPtrToLastStage(pipeline);
                if (last) {
                    void* data = cmsStageData(last);
                    (void)data;
                }
                cmsPipelineFree(pipeline);
            }
            break;
        }
        
        case 3: {
            // Test multiple operations on same pipeline
            cmsPipeline* pipeline = create_dummy_pipeline(ctx);
            if (pipeline) {
                // Get first and last
                cmsStage* first = cmsPipelineGetPtrToFirstStage(pipeline);
                cmsStage* last = cmsPipelineGetPtrToLastStage(pipeline);
                
                if (first && last) {
                    // Duplicate first stage
                    cmsStage* dup_first = cmsStageDup(first);
                    if (dup_first) {
                        cmsStageFree(dup_first);
                    }
                    
                    // Duplicate last stage
                    cmsStage* dup_last = cmsStageDup(last);
                    if (dup_last) {
                        cmsStageFree(dup_last);
                    }
                    
                    // Traverse from first to last
                    cmsStage* current = first;
                    while (current) {
                        void* data = cmsStageData(current);
                        (void)data;
                        current = cmsStageNext(current);
                    }
                }
                cmsPipelineFree(pipeline);
            }
            break;
        }
        
        case 4: {
            // Test with empty pipeline
            cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
            if (pipeline) {
                cmsStage* first = cmsPipelineGetPtrToFirstStage(pipeline);
                cmsStage* last = cmsPipelineGetPtrToLastStage(pipeline);
                (void)first; (void)last; // Should be NULL
                cmsPipelineFree(pipeline);
            }
            break;
        }
        
        case 5: {
            // Test chain of operations
            cmsStage* stage1 = create_dummy_stage(ctx);
            if (stage1) {
                cmsStage* stage2 = cmsStageDup(stage1);
                if (stage2) {
                    cmsStage* stage3 = cmsStageDup(stage2);
                    if (stage3) {
                        void* data1 = cmsStageData(stage1);
                        void* data2 = cmsStageData(stage2);
                        void* data3 = cmsStageData(stage3);
                        (void)data1; (void)data2; (void)data3;
                        cmsStageFree(stage3);
                    }
                    cmsStageFree(stage2);
                }
                cmsStageFree(stage1);
            }
            break;
        }
    }
    
    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}