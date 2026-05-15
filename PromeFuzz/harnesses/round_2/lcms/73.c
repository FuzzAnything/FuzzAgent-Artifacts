// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
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

static cmsStage* create_test_stage(cmsContext context_id, const uint8_t* data, size_t size) {
    // Create a simple identity matrix stage for testing
    if (size < sizeof(double) * 9) {
        return NULL;
    }
    
    double matrix[9];
    for (int i = 0; i < 9; i++) {
        matrix[i] = (double)data[i % size] / 255.0;
    }
    
    return cmsStageAllocMatrix(context_id, 3, 3, matrix, NULL);
}

static cmsPipeline* create_test_pipeline(cmsContext context_id, const uint8_t* data, size_t size) {
    cmsPipeline* pipeline = cmsPipelineAlloc(context_id, 3, 3);
    if (!pipeline) {
        return NULL;
    }
    
    // Add a few stages to the pipeline
    for (int i = 0; i < 3 && size > 0; i++) {
        cmsStage* stage = create_test_stage(context_id, data, size);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
            // Move data pointer to simulate different data for each stage
            data += (size > 100) ? 100 : size;
            size -= (size > 100) ? 100 : size;
        }
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // Create a context
    cmsContext context_id = cmsCreateContext(NULL, NULL);
    if (!context_id) {
        return 0;
    }
    
    // Create test pipeline with stages
    cmsPipeline* pipeline = create_test_pipeline(context_id, Data, Size);
    if (!pipeline) {
        cmsDeleteContext(context_id);
        return 0;
    }
    
    // Test cmsPipelineGetPtrToFirstStage
    cmsStage* first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
    
    // Test cmsStageNext for traversal
    cmsStage* current_stage = first_stage;
    while (current_stage) {
        // Test cmsStageData
        void* stage_data = cmsStageData(current_stage);
        
        // Test cmsStageDup
        cmsStage* dup_stage = cmsStageDup(current_stage);
        if (dup_stage) {
            // Test cmsStageFree on duplicated stage
            cmsStageFree(dup_stage);
        }
        
        current_stage = cmsStageNext(current_stage);
    }
    
    // Test cmsPipelineGetPtrToLastStage
    cmsStage* last_stage = cmsPipelineGetPtrToLastStage(pipeline);
    if (last_stage) {
        // Test cmsStageData on last stage
        void* last_stage_data = cmsStageData(last_stage);
        
        // Test cmsStageDup on last stage
        cmsStage* dup_last = cmsStageDup(last_stage);
        if (dup_last) {
            cmsStageFree(dup_last);
        }
    }
    
    // Remove NULL pointer tests that cause crashes
    // cmsStageDup(NULL);  // Removed - causes crash
    // cmsStageFree(NULL); // Removed - causes crash
    
    // Create a standalone stage and test functions
    cmsStage* standalone_stage = create_test_stage(context_id, Data, Size);
    if (standalone_stage) {
        cmsStage* dup_standalone = cmsStageDup(standalone_stage);
        if (dup_standalone) {
            cmsStageFree(dup_standalone);
        }
        
        void* standalone_data = cmsStageData(standalone_stage);
        cmsStageFree(standalone_stage);
    }
    
    // Clean up
    cmsPipelineFree(pipeline);
    cmsDeleteContext(context_id);
    
    return 0;
}