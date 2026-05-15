// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
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

static cmsPipeline* create_test_pipeline(cmsContext context, const uint8_t* data, size_t size) {
    // Create a simple pipeline with matrix stages
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (!pipeline) return NULL;
    
    // Create matrix data based on fuzzer input
    double matrix[9];
    for (int i = 0; i < 9 && i * sizeof(double) < size; i++) {
        // Use fuzzer data to initialize matrix values
        uint8_t bytes[sizeof(double)];
        size_t bytes_to_copy = sizeof(double);
        if (i * sizeof(double) + bytes_to_copy > size) {
            bytes_to_copy = size - i * sizeof(double);
        }
        memset(bytes, 0, sizeof(double));
        memcpy(bytes, data + i * sizeof(double), bytes_to_copy);
        memcpy(&matrix[i], bytes, sizeof(double));
    }
    
    // Create and append a matrix stage
    cmsStage* stage = cmsStageAllocMatrix(context, 3, 3, matrix, NULL);
    if (stage) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return 0;
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Create test pipeline
    cmsPipeline* pipeline = create_test_pipeline(context, Data, Size);
    if (!pipeline) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Test cmsPipelineGetPtrToFirstStage
    cmsStage* first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
    
    // Test cmsPipelineGetPtrToLastStage
    cmsStage* last_stage = cmsPipelineGetPtrToLastStage(pipeline);
    
    // If we have at least one stage, test stage functions
    if (first_stage) {
        // Test cmsStageDup
        cmsStage* dup_stage = cmsStageDup(first_stage);
        if (dup_stage) {
            // Test cmsStageType on duplicated stage
            cmsStageSignature type = cmsStageType(dup_stage);
            (void)type; // Use type to avoid unused variable warning
            
            // Test cmsStageData on duplicated stage
            void* stage_data = cmsStageData(dup_stage);
            (void)stage_data; // Use stage_data to avoid unused variable warning
            
            // Free duplicated stage
            cmsStageFree(dup_stage);
        }
        
        // Test cmsStageType on original stage
        cmsStageSignature orig_type = cmsStageType(first_stage);
        (void)orig_type;
        
        // Test cmsStageData on original stage
        void* orig_data = cmsStageData(first_stage);
        (void)orig_data;
        
        // Test cmsStageNext traversal
        cmsStage* current = first_stage;
        while (current) {
            // Test functions on each stage
            cmsStageSignature current_type = cmsStageType(current);
            void* current_data = cmsStageData(current);
            (void)current_type;
            (void)current_data;
            
            // Move to next stage
            current = cmsStageNext(current);
        }
    }
    
    // Cleanup
    cmsPipelineFree(pipeline);
    cmsDeleteContext(context);
    
    return 0;
}