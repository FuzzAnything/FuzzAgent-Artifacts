// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsStage* create_test_stage(cmsContext ctx, const uint8_t* data, size_t size) {
    // Create a simple matrix stage for testing
    double matrix[9];
    
    // Fill matrix with data from fuzzer input
    size_t bytes_to_copy = size < sizeof(matrix) ? size : sizeof(matrix);
    memcpy(matrix, data, bytes_to_copy);
    
    // If we didn't get enough data, fill remaining with default values
    for (size_t i = bytes_to_copy / sizeof(double); i < 9; i++) {
        matrix[i] = (i % 4 == 0) ? 1.0 : 0.0;
    }
    
    return cmsStageAllocMatrix(ctx, 3, 3, matrix, NULL);
}

static cmsPipeline* create_test_pipeline(cmsContext ctx, const uint8_t* data, size_t size) {
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (!pipeline) return NULL;
    
    // Create and add a stage
    cmsStage* stage = create_test_stage(ctx, data, size);
    if (stage) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create a pipeline with test data
    cmsPipeline* pipeline = create_test_pipeline(ctx, Data, Size);
    if (!pipeline) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Create a separate stage for testing (not inserted into pipeline)
    cmsStage* test_stage = create_test_stage(ctx, Data, Size);
    
    if (test_stage) {
        // Test cmsStageType
        cmsStageSignature sig = cmsStageType(test_stage);
        
        // Test cmsStageData
        void* stage_data = cmsStageData(test_stage);
        
        // Test cmsStageNext (should be NULL for standalone stage)
        cmsStage* next_stage = cmsStageNext(test_stage);
        
        // Test cmsPipelineGetPtrToLastStage
        cmsStage* last_stage = cmsPipelineGetPtrToLastStage(pipeline);
        
        // Test cmsStageDup with varying inputs
        uint8_t selector = Data[0] % 4;
        
        switch (selector) {
            case 0:
                // Test duplication of test stage
                {
                    cmsStage* dup_stage = cmsStageDup(test_stage);
                    if (dup_stage) {
                        cmsStageFree(dup_stage);
                    }
                }
                break;
                
            case 1:
                // Test duplication of last stage from pipeline
                if (last_stage) {
                    cmsStage* dup_stage = cmsStageDup(last_stage);
                    if (dup_stage) {
                        cmsStageFree(dup_stage);
                    }
                }
                break;
                
            case 2:
                // Add another stage to pipeline and test traversal
                if (Size > 10) {
                    cmsStage* new_stage = create_test_stage(ctx, Data + 1, Size - 1);
                    if (new_stage) {
                        cmsPipelineInsertStage(pipeline, cmsAT_END, new_stage);
                        
                        // Get the last stage and test functions on it
                        cmsStage* current = cmsPipelineGetPtrToLastStage(pipeline);
                        if (current) {
                            cmsStageType(current);
                            cmsStageData(current);
                            cmsStage* next = cmsStageNext(current);
                            // Do nothing with next - just test the call
                        }
                    }
                }
                break;
                
            case 3:
                // Test with NULL inputs (edge cases)
                cmsStageType(NULL);
                cmsStageData(NULL);
                cmsStageNext(NULL);
                cmsStageDup(NULL);
                cmsPipelineGetPtrToLastStage(NULL);
                break;
        }
        
        cmsStageFree(test_stage);
    }
    
    // Cleanup - free pipeline first, then context
    cmsPipelineFree(pipeline);
    cmsDeleteContext(ctx);
    
    return 0;
}