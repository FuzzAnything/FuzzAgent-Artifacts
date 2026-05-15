// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fill_random_pipeline_data(cmsPipeline* pipeline, const uint8_t* data, size_t size) {
    if (!pipeline || size == 0) return;
    
    // Create some dummy stages if needed - simplified approach
    // In real fuzzing, we'd want to create more complex pipeline structures
    if (size > sizeof(cmsUInt32Number)) {
        cmsUInt32Number stage_type = *(cmsUInt32Number*)data;
        // We could create different stage types based on data
        // but for basic fuzzing, we'll keep it simple
    }
}

static cmsPipeline* create_test_pipeline(cmsContext ctx, cmsUInt32Number in, cmsUInt32Number out, 
                                         const uint8_t* data, size_t size) {
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, in, out);
    if (pipeline) {
        fill_random_pipeline_data(pipeline, data, size);
    }
    return pipeline;
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = NULL; // Using default context
    
    // Extract parameters from fuzzing data
    cmsUInt32Number in_channels = (Data[0] % 16) + 1;  // 1-16 channels
    cmsUInt32Number out_channels = (Data[1] % 16) + 1; // 1-16 channels
    cmsUInt32Number flags = Data[2];
    cmsUInt32Number eval_type = Data[3] % 2; // 0 for float, 1 for 16-bit
    
    // Create first pipeline
    cmsPipeline* pipeline1 = create_test_pipeline(ctx, in_channels, out_channels, 
                                                  Data + 4, Size > 4 ? Size - 4 : 0);
    if (!pipeline1) return 0;
    
    // Create second pipeline for concatenation
    cmsPipeline* pipeline2 = NULL;
    if (Size > 8) {
        cmsUInt32Number in2 = (Data[4] % 16) + 1;
        cmsUInt32Number out2 = (Data[5] % 16) + 1;
        pipeline2 = create_test_pipeline(ctx, in2, out2, Data + 8, Size > 8 ? Size - 8 : 0);
    }
    
    // Test cmsPipelineCat if we have two pipelines
    if (pipeline2) {
        cmsPipelineCat(pipeline1, pipeline2);
    }
    
    // Test cmsPipelineSetSaveAs8bitsFlag
    cmsBool prev_flag = cmsPipelineSetSaveAs8bitsFlag(pipeline1, flags & 1);
    (void)prev_flag; // Mark as used
    
    // Test cmsPipelineEvalFloat
    if (eval_type == 0 && Size >= (4 + in_channels * sizeof(cmsFloat32Number))) {
        cmsFloat32Number* in_float = (cmsFloat32Number*)malloc(in_channels * sizeof(cmsFloat32Number));
        cmsFloat32Number* out_float = (cmsFloat32Number*)malloc(out_channels * sizeof(cmsFloat32Number));
        
        if (in_float && out_float) {
            // Copy data for input
            size_t copy_size = in_channels * sizeof(cmsFloat32Number);
            if (copy_size > Size - 4) copy_size = Size - 4;
            memcpy(in_float, Data + 4, copy_size);
            
            cmsPipelineEvalFloat(in_float, out_float, pipeline1);
        }
        
        free(in_float);
        free(out_float);
    }
    
    // Test cmsPipelineEval16
    if (eval_type == 1 && Size >= (4 + in_channels * sizeof(cmsUInt16Number))) {
        cmsUInt16Number* in_16 = (cmsUInt16Number*)malloc(in_channels * sizeof(cmsUInt16Number));
        cmsUInt16Number* out_16 = (cmsUInt16Number*)malloc(out_channels * sizeof(cmsUInt16Number));
        
        if (in_16 && out_16) {
            // Copy data for input
            size_t copy_size = in_channels * sizeof(cmsUInt16Number);
            if (copy_size > Size - 4) copy_size = Size - 4;
            memcpy(in_16, Data + 4, copy_size);
            
            cmsPipelineEval16(in_16, out_16, pipeline1);
        }
        
        free(in_16);
        free(out_16);
    }
    
    // Test cmsPipelineDup
    cmsPipeline* dup_pipeline = cmsPipelineDup(pipeline1);
    if (dup_pipeline) {
        // Test evaluation on duplicated pipeline
        if (Size >= (4 + in_channels * sizeof(cmsFloat32Number))) {
            cmsFloat32Number* in_float = (cmsFloat32Number*)malloc(in_channels * sizeof(cmsFloat32Number));
            cmsFloat32Number* out_float = (cmsFloat32Number*)malloc(out_channels * sizeof(cmsFloat32Number));
            
            if (in_float && out_float) {
                size_t copy_size = in_channels * sizeof(cmsFloat32Number);
                if (copy_size > Size - 4) copy_size = Size - 4;
                memcpy(in_float, Data + 4, copy_size);
                
                cmsPipelineEvalFloat(in_float, out_float, dup_pipeline);
            }
            
            free(in_float);
            free(out_float);
        }
        cmsPipelineFree(dup_pipeline);
    }
    
    // Cleanup
    cmsPipelineFree(pipeline1);
    if (pipeline2) {
        cmsPipelineFree(pipeline2);
    }
    
    return 0;
}