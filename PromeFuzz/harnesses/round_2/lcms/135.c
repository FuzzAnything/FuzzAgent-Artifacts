// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/lcms/latest/code/include/lcms2.h"

static cmsStage* create_test_stage(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 4) return NULL;
    
    cmsUInt32Number inputChannels = data[0] % 8 + 1;
    cmsUInt32Number outputChannels = data[1] % 8 + 1;
    
    // Create a tone curve stage as a simple test stage
    cmsToneCurve* toneCurve = cmsBuildGamma(ctx, 2.2);
    if (!toneCurve) return NULL;
    
    cmsStage* stage = cmsStageAllocToneCurves(ctx, outputChannels, &toneCurve);
    cmsFreeToneCurve(toneCurve);
    
    return stage;
}

static cmsPipeline* create_test_pipeline(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 8) return NULL;
    
    cmsUInt32Number inputChannels = data[0] % 8 + 1;
    cmsUInt32Number outputChannels = data[1] % 8 + 1;
    
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, inputChannels, outputChannels);
    if (!pipeline) return NULL;
    
    // Add a few stages based on input data
    for (int i = 0; i < 3 && (size_t)(i * 2 + 4) < size; i++) {
        cmsStage* stage = create_test_stage(ctx, data + i * 2, size - i * 2);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        }
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_135(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create a test pipeline with stages
    cmsPipeline* pipeline = create_test_pipeline(ctx, Data, Size);
    if (!pipeline) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Test cmsPipelineGetPtrToLastStage
    cmsStage* lastStage = cmsPipelineGetPtrToLastStage(pipeline);
    
    // Traverse the pipeline using cmsStageNext
    // Get the first stage by inserting a dummy stage and then getting next
    cmsStage* dummyStage = create_test_stage(ctx, Data, Size);
    if (dummyStage) {
        // Insert dummy at beginning to get first real stage
        cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, dummyStage);
        cmsStage* current = cmsStageNext(dummyStage);
        
        while (current) {
            // Test cmsStageType
            cmsStageSignature type = cmsStageType(current);
            
            // Test cmsStageData
            void* stageData = cmsStageData(current);
            
            // Test cmsStageDup
            cmsStage* dupStage = cmsStageDup(current);
            if (dupStage) {
                // Test cmsStageFree on duplicated stage
                cmsStageFree(dupStage);
            }
            
            // Move to next stage
            current = cmsStageNext(current);
        }
    }
    
    // Test with some edge cases
    if (Size > 10) {
        // Try to duplicate NULL stage
        cmsStage* nullDup = cmsStageDup(NULL);
        
        // Try to get type of NULL stage
        if (Data[0] % 2 == 0) {
            cmsStageType(NULL);
        }
        
        // Try to get data of NULL stage
        if (Data[1] % 2 == 0) {
            cmsStageData(NULL);
        }
        
        // Try to get next of NULL stage
        if (Data[2] % 2 == 0) {
            cmsStageNext(NULL);
        }
        
        // Try to free NULL stage
        cmsStageFree(NULL);
    }
    
    // Clean up
    cmsPipelineFree(pipeline);
    cmsDeleteContext(ctx);
    
    return 0;
}