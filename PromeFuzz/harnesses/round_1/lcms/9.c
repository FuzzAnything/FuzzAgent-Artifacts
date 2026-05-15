// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineUnlinkStage at cmslut.c:1555:16 in lcms2.h
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

static cmsStage* create_dummy_stage(cmsContext context) {
    // Create a simple identity matrix stage
    cmsStage* stage = cmsStageAllocIdentity(context, 3);
    return stage;
}

static cmsPipeline* create_dummy_pipeline(cmsContext context) {
    // Create a pipeline with 3 input and 3 output channels
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (!pipeline) return NULL;
    
    // Add a dummy stage
    cmsStage* stage = create_dummy_stage(context);
    if (stage) {
        cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage);
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use default context
    cmsContext context = NULL;
    
    // Create dummy pipelines for testing
    cmsPipeline* pipeline1 = create_dummy_pipeline(context);
    cmsPipeline* pipeline2 = create_dummy_pipeline(context);
    
    if (!pipeline1 || !pipeline2) {
        if (pipeline1) cmsPipelineFree(pipeline1);
        if (pipeline2) cmsPipelineFree(pipeline2);
        return 0;
    }
    
    // Use first byte to determine which operations to perform
    uint8_t op_selector = Data[0];
    size_t data_offset = 1;
    
    // Test cmsPipelineCat
    if ((op_selector & 0x01) && data_offset < Size) {
        cmsPipelineCat(pipeline1, pipeline2);
    }
    
    // Test cmsPipelineCheckAndRetreiveStages
    if ((op_selector & 0x02) && data_offset < Size) {
        cmsStage* stage1 = NULL;
        cmsStage* stage2 = NULL;
        
        // Try to retrieve identity stages
        cmsPipelineCheckAndRetreiveStages(pipeline1, 1, 
                                         cmsSigIdentityElemType, &stage1);
        
        // Try with two expected stages if we have enough data
        if (data_offset + 1 < Size) {
            uint8_t stage_types = Data[data_offset] % 3;
            cmsStageSignature types[2];
            
            // Set some random stage types based on input
            types[0] = (stage_types == 0) ? cmsSigIdentityElemType : 
                      (stage_types == 1) ? cmsSigMatrixElemType : 
                      cmsSigCLutElemType;
            types[1] = cmsSigIdentityElemType;
            
            cmsPipelineCheckAndRetreiveStages(pipeline1, 2, 
                                             types[0], &stage1,
                                             types[1], &stage2);
        }
        data_offset++;
    }
    
    // Test cmsPipelineUnlinkStage
    if ((op_selector & 0x04) && data_offset < Size) {
        cmsStage* unlinked_stage = NULL;
        
        // Choose location based on input data
        cmsStageLoc loc = (Data[data_offset] & 0x01) ? cmsAT_BEGIN : cmsAT_END;
        cmsPipelineUnlinkStage(pipeline1, loc, &unlinked_stage);
        
        // If we got a stage back, free it
        if (unlinked_stage) {
            cmsStageFree(unlinked_stage);
        }
        data_offset++;
    }
    
    // Test cmsPipelineInsertStage
    if ((op_selector & 0x08) && data_offset < Size) {
        cmsStage* new_stage = create_dummy_stage(context);
        if (new_stage) {
            // Choose location based on input data
            cmsStageLoc loc = (Data[data_offset] & 0x01) ? cmsAT_BEGIN : cmsAT_END;
            cmsPipelineInsertStage(pipeline1, loc, new_stage);
        }
        data_offset++;
    }
    
    // Test cmsPipelineDup
    if (op_selector & 0x10) {
        cmsPipeline* dup_pipeline = cmsPipelineDup(pipeline1);
        if (dup_pipeline) {
            // Do some operations on the duplicate
            cmsStage* dup_stage = create_dummy_stage(context);
            if (dup_stage) {
                cmsPipelineInsertStage(dup_pipeline, cmsAT_END, dup_stage);
            }
            
            // Free the duplicate
            cmsPipelineFree(dup_pipeline);
        }
    }
    
    // Clean up
    cmsPipelineFree(pipeline1);
    cmsPipelineFree(pipeline2);
    
    return 0;
}