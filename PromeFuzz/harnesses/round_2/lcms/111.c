// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static cmsPipeline* create_test_pipeline(cmsContext ctx, cmsUInt32Number inputChannels, cmsUInt32Number outputChannels) {
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, inputChannels, outputChannels);
    if (!pipeline) return NULL;
    
    // Try to add a simple matrix stage if we have at least 3x3 matrix data
    if (inputChannels >= 3 && outputChannels >= 3) {
        cmsFloat64Number matrix[9] = {1.0, 0.0, 0.0,
                                       0.0, 1.0, 0.0,
                                       0.0, 0.0, 1.0};
        cmsStage* stage = cmsStageAllocMatrix(ctx, 3, 3, matrix, NULL);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        }
    }
    
    return pipeline;
}

static void dummy_eval16(const cmsUInt16Number* In, cmsUInt16Number* Out, const void* Data) {
    (void)Data;
    if (In && Out) {
        Out[0] = In[0];
    }
}

static void* dummy_dup(cmsContext ContextID, const void* Data) {
    (void)ContextID;
    (void)Data;
    return NULL;
}

static void dummy_free(cmsContext ContextID, void* Data) {
    (void)ContextID;
    (void)Data;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = create_test_context();
    if (!ctx) return 0;
    
    // Use first byte to determine pipeline configurations
    uint8_t config = Data[0];
    cmsUInt32Number in1 = (config & 0x0F) + 1;
    cmsUInt32Number out1 = ((config >> 4) & 0x0F) + 1;
    
    // Use second byte for second pipeline
    config = Data[1];
    cmsUInt32Number in2 = (config & 0x0F) + 1;
    cmsUInt32Number out2 = ((config >> 4) & 0x0F) + 1;
    
    // Create first pipeline
    cmsPipeline* pipeline1 = create_test_pipeline(ctx, in1, out1);
    if (!pipeline1) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Create second pipeline
    cmsPipeline* pipeline2 = create_test_pipeline(ctx, in2, out2);
    if (!pipeline2) {
        cmsPipelineFree(pipeline1);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Test cmsPipelineCat with various configurations
    if (Size > 2) {
        uint8_t cat_config = Data[2];
        
        // Try concatenation in different orders based on input
        if (cat_config & 0x01) {
            cmsPipeline* pipeline1_copy = cmsPipelineDup(pipeline1);
            if (pipeline1_copy) {
                cmsBool result = cmsPipelineCat(pipeline1_copy, pipeline2);
                (void)result; // Ignore result, just test the call
                cmsPipelineFree(pipeline1_copy);
            }
        }
        
        if (cat_config & 0x02) {
            cmsPipeline* pipeline2_copy = cmsPipelineDup(pipeline2);
            if (pipeline2_copy) {
                cmsBool result = cmsPipelineCat(pipeline2_copy, pipeline1);
                (void)result;
                cmsPipelineFree(pipeline2_copy);
            }
        }
    }
    
    // Test cmsPipelineOutputChannels
    cmsUInt32Number out_channels1 = cmsPipelineOutputChannels(pipeline1);
    cmsUInt32Number out_channels2 = cmsPipelineOutputChannels(pipeline2);
    (void)out_channels1;
    (void)out_channels2;
    
    // Test cmsPipelineDup
    cmsPipeline* dup_pipeline1 = cmsPipelineDup(pipeline1);
    cmsPipeline* dup_pipeline2 = cmsPipelineDup(pipeline2);
    
    if (dup_pipeline1) {
        // Test _cmsPipelineSetOptimizationParameters with dummy functions
        _cmsPipelineSetOptimizationParameters(dup_pipeline1, dummy_eval16, NULL, dummy_free, dummy_dup);
        
        // Test cmsPipelineCheckAndRetreiveStages
        if (Size > 3) {
            uint8_t check_config = Data[3];
            cmsStage* stage1 = NULL;
            cmsStage* stage2 = NULL;
            
            // Try different stage type checks based on input
            if (check_config & 0x01) {
                cmsPipelineCheckAndRetreiveStages(dup_pipeline1, 1, 
                                                  cmsSigMatrixElemType, &stage1);
            }
            if (check_config & 0x02) {
                cmsPipelineCheckAndRetreiveStages(dup_pipeline1, 2,
                                                  cmsSigMatrixElemType, &stage1,
                                                  cmsSigMatrixElemType, &stage2);
            }
        }
        
        cmsPipelineFree(dup_pipeline1);
    }
    
    if (dup_pipeline2) {
        cmsPipelineFree(dup_pipeline2);
    }
    
    // Clean up
    cmsPipelineFree(pipeline1);
    cmsPipelineFree(pipeline2);
    cmsDeleteContext(ctx);
    
    return 0;
}