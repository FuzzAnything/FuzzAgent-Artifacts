// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageType at cmslut.c:1228:29 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

static cmsPipeline* create_dummy_pipeline(cmsContext context) {
    // Create a simple pipeline with identity stages
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (!pipeline) return NULL;
    
    // Add a couple of identity stages
    cmsStage* stage1 = cmsStageAllocIdentity(context, 3);
    if (stage1) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, stage1);
    }
    
    cmsStage* stage2 = cmsStageAllocIdentity(context, 3);
    if (stage2) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, stage2);
    }
    
    return pipeline;
}

static cmsStage* create_dummy_stage(cmsContext context) {
    // Create a simple identity stage
    return cmsStageAllocIdentity(context, 3);
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use the first byte to determine which function to test
    uint8_t selector = Data[0];
    
    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Create dummy pipeline and stage for testing
    cmsPipeline* pipeline = create_dummy_pipeline(context);
    cmsStage* stage = create_dummy_stage(context);
    
    if (!pipeline || !stage) {
        if (pipeline) cmsPipelineFree(pipeline);
        if (stage) cmsStageFree(stage);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Test different functions based on selector
    switch (selector % 6) {
        case 0:
            // Test cmsStageFree
            if (stage) {
                cmsStageFree(stage);
                stage = NULL;
            }
            break;
            
        case 1:
            // Test cmsStageType
            if (stage) {
                cmsStageSignature sig = cmsStageType(stage);
                (void)sig; // Use result to avoid unused variable warning
            }
            break;
            
        case 2:
            // Test cmsStageNext
            // Get the first stage from the pipeline using public API
            cmsStage* first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (first_stage) {
                cmsStage* next = cmsStageNext(first_stage);
                (void)next; // Use result
            }
            break;
            
        case 3:
            // Test cmsStageData
            if (stage) {
                void* data = cmsStageData(stage);
                (void)data; // Use result
            }
            break;
            
        case 4:
            // Test cmsStageDup
            if (stage) {
                cmsStage* dup = cmsStageDup(stage);
                if (dup) {
                    cmsStageFree(dup);
                }
            }
            break;
            
        case 5:
            // Test cmsPipelineGetPtrToLastStage
            if (pipeline) {
                cmsStage* last = cmsPipelineGetPtrToLastStage(pipeline);
                (void)last; // Use result
            }
            break;
    }
    
    // Cleanup
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    if (stage) {
        cmsStageFree(stage);
    }
    cmsDeleteContext(context);
    
    return 0;
}