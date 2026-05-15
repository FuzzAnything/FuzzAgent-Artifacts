// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsPipeline* create_dummy_pipeline(cmsContext context) {
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (!pipeline) return NULL;
    
    // Create and add some stages to the pipeline
    for (int i = 0; i < 3; i++) {
        // Create a matrix stage with identity matrix
        double matrix[12];
        for (int j = 0; j < 12; j++) {
            matrix[j] = (j % 5 == 0) ? 1.0 : 0.0;
        }
        
        cmsStage* stage = cmsStageAllocMatrix(context, 3, 3, matrix, NULL);
        if (!stage) {
            cmsPipelineFree(pipeline);
            return NULL;
        }
        
        cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
    }
    
    return pipeline;
}

static cmsStage* get_first_stage(const cmsPipeline* lut) {
    // Get the last stage first
    cmsStage* last = cmsPipelineGetPtrToLastStage(lut);
    if (!last) return NULL;
    
    // Traverse backwards to find the first stage
    cmsStage* current = last;
    cmsStage* prev = NULL;
    
    while (current) {
        prev = current;
        current = cmsStageNext(current);
        // If cmsStageNext returns NULL, we've reached the end
        // But we want to go backwards, so we need a different approach
        // Actually, we can't traverse backwards with the given API
        // So we'll just return the last stage for testing
    }
    
    return last; // Return last stage since we can't get first
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Create a dummy pipeline with stages
    cmsPipeline* pipeline = create_dummy_pipeline(context);
    if (!pipeline) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Use fuzzer data to determine which operations to perform
    uint8_t op = Data[0] % 6;
    size_t data_offset = 1;
    
    // Get a stage from the pipeline for testing
    cmsStage* stage = cmsPipelineGetPtrToLastStage(pipeline);
    
    switch (op) {
        case 0: {
            // Test cmsStageType
            if (stage) {
                cmsStageSignature sig = cmsStageType(stage);
                (void)sig; // Use result to avoid optimization
            }
            break;
        }
        case 1: {
            // Test cmsPipelineGetPtrToLastStage
            cmsStage* last = cmsPipelineGetPtrToLastStage(pipeline);
            if (last) {
                cmsStageSignature sig = cmsStageType(last);
                (void)sig;
            }
            break;
        }
        case 2: {
            // Test cmsStageNext
            if (stage) {
                cmsStage* next = cmsStageNext(stage);
                if (next) {
                    cmsStageSignature sig = cmsStageType(next);
                    (void)sig;
                }
            }
            break;
        }
        case 3: {
            // Test cmsStageFree on a duplicated stage
            if (stage) {
                cmsStage* dup = cmsStageDup(stage);
                if (dup) {
                    cmsStageFree(dup);
                }
            }
            break;
        }
        case 4: {
            // Test cmsStageData
            if (stage) {
                void* data = cmsStageData(stage);
                if (data && data_offset < Size) {
                    // Try to read a small amount from the data
                    uint8_t dummy = *((uint8_t*)data);
                    (void)dummy;
                }
            }
            break;
        }
        case 5: {
            // Test cmsStageDup
            if (stage) {
                cmsStage* dup = cmsStageDup(stage);
                if (dup) {
                    // Exercise the duplicated stage
                    cmsStageSignature sig = cmsStageType(dup);
                    void* data = cmsStageData(dup);
                    (void)sig;
                    (void)data;
                    
                    // Clean up
                    cmsStageFree(dup);
                }
            }
            break;
        }
    }
    
    // Additional fuzzing: traverse stages in pipeline
    if (data_offset < Size && (Data[data_offset] % 2 == 0)) {
        cmsStage* current = cmsPipelineGetPtrToLastStage(pipeline);
        while (current) {
            cmsStageSignature sig = cmsStageType(current);
            void* data = cmsStageData(current);
            (void)sig;
            (void)data;
            
            current = cmsStageNext(current);
        }
    }
    
    // Cleanup
    cmsPipelineFree(pipeline);
    cmsDeleteContext(context);
    
    return 0;
}