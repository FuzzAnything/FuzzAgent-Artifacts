// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsPipeline* create_test_pipeline(cmsContext ctx) {
    // Create a simple pipeline with 3 input and 3 output channels
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (!pipeline) return NULL;
    
    // Create a matrix stage and add it to pipeline
    cmsStage* matrix_stage = cmsStageAllocMatrix(ctx, 3, 3, 
        (const cmsFloat64Number[]){1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}, NULL);
    if (!matrix_stage) {
        cmsPipelineFree(pipeline);
        return NULL;
    }
    
    cmsPipelineInsertStage(pipeline, cmsAT_END, matrix_stage);
    return pipeline;
}

static cmsStage* create_test_stage(cmsContext ctx) {
    // Create a simple matrix stage
    return cmsStageAllocMatrix(ctx, 3, 3, 
        (const cmsFloat64Number[]){1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}, NULL);
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine which functions to test
    uint8_t selector = Data[0];
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create test pipeline and stage
    cmsPipeline* pipeline = create_test_pipeline(ctx);
    cmsStage* test_stage = create_test_stage(ctx);
    
    if (!pipeline || !test_stage) {
        if (pipeline) cmsPipelineFree(pipeline);
        if (test_stage) cmsStageFree(test_stage);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Test different functions based on selector
    switch (selector % 6) {
        case 0: {
            // Test cmsStageDup
            cmsStage* dup_stage = cmsStageDup(test_stage);
            if (dup_stage) {
                // Test cmsStageData on duplicated stage
                void* stage_data = cmsStageData(dup_stage);
                (void)stage_data; // Use to avoid unused variable warning
                cmsStageFree(dup_stage);
            }
            break;
        }
        case 1: {
            // Test cmsPipelineGetPtrToLastStage
            cmsStage* last_stage = cmsPipelineGetPtrToLastStage(pipeline);
            if (last_stage) {
                // Test cmsStageData on last stage
                void* stage_data = cmsStageData(last_stage);
                (void)stage_data;
            }
            break;
        }
        case 2: {
            // Test cmsStageData directly
            void* stage_data = cmsStageData(test_stage);
            (void)stage_data;
            break;
        }
        case 3: {
            // Test cmsStageNext
            cmsStage* first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (first_stage) {
                cmsStage* next_stage = cmsStageNext(first_stage);
                (void)next_stage; // May be NULL if only one stage
            }
            break;
        }
        case 4: {
            // Test cmsStageFree on a duplicated stage
            cmsStage* dup_stage = cmsStageDup(test_stage);
            if (dup_stage) {
                cmsStageFree(dup_stage);
            }
            break;
        }
        case 5: {
            // Test cmsPipelineGetPtrToFirstStage and traverse
            cmsStage* current = cmsPipelineGetPtrToFirstStage(pipeline);
            while (current) {
                // Test cmsStageData on each stage
                void* stage_data = cmsStageData(current);
                (void)stage_data;
                current = cmsStageNext(current);
            }
            break;
        }
    }
    
    // Cleanup
    cmsPipelineFree(pipeline);
    cmsStageFree(test_stage);
    cmsDeleteContext(ctx);
    
    return 0;
}