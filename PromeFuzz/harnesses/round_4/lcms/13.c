// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsStage* create_dummy_stage(cmsContext ctx) {
    // Create a simple identity matrix stage
    cmsStage* stage = cmsStageAllocIdentity(ctx, 3);
    return stage;
}

static cmsPipeline* create_dummy_pipeline(cmsContext ctx) {
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (!pipeline) return NULL;
    
    cmsStage* stage = create_dummy_stage(ctx);
    if (!stage) {
        cmsPipelineFree(pipeline);
        return NULL;
    }
    
    cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
    return pipeline;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create dummy pipeline for testing
    cmsPipeline* pipeline = create_dummy_pipeline(ctx);
    if (!pipeline) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Use fuzz data to determine which functions to call
    if (Size > 0) {
        uint8_t selector = Data[0] % 6;
        
        switch (selector) {
            case 0: {
                // Create a standalone stage and free it
                cmsStage* standalone_stage = create_dummy_stage(ctx);
                if (standalone_stage) {
                    cmsStageFree(standalone_stage);
                }
                break;
            }
                
            case 1: {
                cmsStage* first = cmsPipelineGetPtrToFirstStage(pipeline);
                if (first) {
                    cmsStage* next = cmsStageNext(first);
                    (void)next; // Use result to avoid unused variable warning
                }
                break;
            }
            
            case 2: {
                cmsStage* first = cmsPipelineGetPtrToFirstStage(pipeline);
                if (first) {
                    void* data = cmsStageData(first);
                    (void)data;
                }
                break;
            }
            
            case 3: {
                cmsStage* first = cmsPipelineGetPtrToFirstStage(pipeline);
                if (first) {
                    cmsStage* dup = cmsStageDup(first);
                    if (dup) {
                        cmsStageFree(dup);
                    }
                }
                break;
            }
            
            case 4: {
                cmsStage* last = cmsPipelineGetPtrToLastStage(pipeline);
                (void)last;
                break;
            }
            
            case 5: {
                cmsStage* first = cmsPipelineGetPtrToFirstStage(pipeline);
                (void)first;
                break;
            }
        }
    }
    
    // Cleanup
    cmsPipelineFree(pipeline);
    cmsDeleteContext(ctx);
    
    return 0;
}