// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsPipeline* create_test_pipeline(cmsContext context, cmsUInt32Number inputChannels, cmsUInt32Number outputChannels) {
    cmsPipeline* pipeline = cmsPipelineAlloc(context, inputChannels, outputChannels);
    if (!pipeline) return NULL;
    
    // Add some identity stages to make the pipeline non-empty
    cmsStage* identity_stage = cmsStageAllocIdentity(context, outputChannels);
    if (identity_stage) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, identity_stage);
    }
    
    // Add a matrix stage
    cmsStage* matrix_stage = cmsStageAllocMatrix(context, outputChannels, outputChannels, NULL, NULL);
    if (matrix_stage) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, matrix_stage);
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Use input data to determine pipeline parameters
    cmsUInt32Number inputChannels = (Data[0] % 16) + 1;
    cmsUInt32Number outputChannels = ((Data[0] >> 4) % 16) + 1;
    
    // Create test pipeline
    cmsPipeline* pipeline = create_test_pipeline(context, inputChannels, outputChannels);
    if (!pipeline) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Test cmsPipelineGetPtrToFirstStage
    cmsStage* first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
    
    // Test cmsPipelineGetPtrToLastStage
    cmsStage* last_stage = cmsPipelineGetPtrToLastStage(pipeline);
    
    // If we have stages, test the stage functions
    if (first_stage) {
        // Test cmsStageDup
        cmsStage* dup_stage = cmsStageDup(first_stage);
        if (dup_stage) {
            // Test cmsStageType on duplicated stage
            cmsStageSignature type = cmsStageType(dup_stage);
            
            // Test cmsStageData on duplicated stage
            void* stage_data = cmsStageData(dup_stage);
            
            // Free duplicated stage
            cmsStageFree(dup_stage);
        }
        
        // Test cmsStageType on first stage
        cmsStageSignature first_type = cmsStageType(first_stage);
        
        // Test cmsStageData on first stage
        void* first_data = cmsStageData(first_stage);
        
        // Test cmsStageNext traversal
        cmsStage* current = first_stage;
        while (current) {
            cmsStageSignature current_type = cmsStageType(current);
            void* current_data = cmsStageData(current);
            current = cmsStageNext(current);
        }
    }
    
    if (last_stage) {
        // Test cmsStageType on last stage
        cmsStageSignature last_type = cmsStageType(last_stage);
        
        // Test cmsStageData on last stage
        void* last_data = cmsStageData(last_stage);
    }
    
    // Test with empty pipeline
    cmsPipeline* empty_pipeline = cmsPipelineAlloc(context, 3, 3);
    if (empty_pipeline) {
        cmsStage* empty_first = cmsPipelineGetPtrToFirstStage(empty_pipeline);
        cmsStage* empty_last = cmsPipelineGetPtrToLastStage(empty_pipeline);
        cmsPipelineFree(empty_pipeline);
    }
    
    // Cleanup
    cmsPipelineFree(pipeline);
    cmsDeleteContext(context);
    
    return 0;
}