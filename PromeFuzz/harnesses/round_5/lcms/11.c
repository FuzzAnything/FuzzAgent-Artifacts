// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineEvalReverseFloat at cmslut.c:1753:19 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsPipeline* create_test_pipeline(cmsContext context, const uint8_t* data, size_t size) {
    // Create a simple pipeline with some basic stages for testing
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (!pipeline) return NULL;
    
    // Add an identity matrix stage if we have enough data
    if (size >= 9 * sizeof(cmsFloat64Number)) {
        cmsFloat64Number* matrix = (cmsFloat64Number*)data;
        cmsFloat64Number offset[3] = {0, 0, 0};
        
        cmsStage* matrix_stage = cmsStageAllocMatrix(context, 3, 3, matrix, offset);
        if (matrix_stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, matrix_stage);
        }
    }
    
    // Add a CLUT stage if we have enough data
    if (size >= 24 * sizeof(cmsUInt16Number)) {
        cmsUInt16Number* clut_data = (cmsUInt16Number*)data;
        cmsUInt32Number grid_points = 2; // 2x2x2 grid
        
        cmsStage* clut_stage = cmsStageAllocCLut16bit(context, grid_points, 3, 3, clut_data);
        if (clut_stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, clut_stage);
        }
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_11(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Create a test pipeline
    cmsPipeline* pipeline = create_test_pipeline(context, Data, Size);
    if (!pipeline) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Test cmsPipelineStageCount (only with valid pipeline)
    cmsUInt32Number stage_count = cmsPipelineStageCount(pipeline);
    
    // Test cmsPipelineInputChannels (only with valid pipeline)
    cmsUInt32Number input_channels = cmsPipelineInputChannels(pipeline);
    
    // Test cmsPipelineCheckAndRetreiveStages
    cmsStage* stage1 = NULL;
    cmsStage* stage2 = NULL;
    cmsBool check_result = FALSE;
    
    // Try to retrieve stages based on available data
    if (stage_count >= 1) {
        check_result = cmsPipelineCheckAndRetreiveStages(pipeline, 1, 
                                                        cmsSigMatrixElemType, &stage1);
    }
    if (stage_count >= 2) {
        check_result = cmsPipelineCheckAndRetreiveStages(pipeline, 2,
                                                        cmsSigMatrixElemType, &stage1,
                                                        cmsSigCLutElemType, &stage2);
    }
    
    // Test cmsPipelineEvalFloat
    if (Size >= 3 * sizeof(cmsFloat32Number)) {
        cmsFloat32Number in[3];
        cmsFloat32Number out[3];
        
        // Use first 3 floats from data as input
        memcpy(in, Data, 3 * sizeof(cmsFloat32Number));
        cmsPipelineEvalFloat(in, out, pipeline);
    }
    
    // Test cmsPipelineEvalReverseFloat
    if (Size >= 6 * sizeof(cmsFloat32Number)) {
        cmsFloat32Number target[3];
        cmsFloat32Number result[3];
        cmsFloat32Number hint[3];
        
        // Use next 6 floats from data as target and hint
        memcpy(target, Data + 3 * sizeof(cmsFloat32Number), 3 * sizeof(cmsFloat32Number));
        memcpy(hint, Data + 6 * sizeof(cmsFloat32Number), 3 * sizeof(cmsFloat32Number));
        
        cmsPipelineEvalReverseFloat(target, result, hint, pipeline);
    }
    
    // Test with different pipeline configurations
    // Create another pipeline with different channel counts
    cmsPipeline* pipeline2 = cmsPipelineAlloc(context, 4, 3);
    if (pipeline2) {
        // Test stage count on different pipeline
        cmsPipelineStageCount(pipeline2);
        
        // Test input channels
        cmsPipelineInputChannels(pipeline2);
        
        // Free the second pipeline
        cmsPipelineFree(pipeline2);
    }
    
    // Create a pipeline with 1 input and 1 output channel
    cmsPipeline* pipeline3 = cmsPipelineAlloc(context, 1, 1);
    if (pipeline3) {
        // Add an identity stage
        cmsStage* identity_stage = cmsStageAllocIdentity(context, 1);
        if (identity_stage) {
            cmsPipelineInsertStage(pipeline3, cmsAT_END, identity_stage);
        }
        
        // Test evaluation with single channel
        if (Size >= sizeof(cmsFloat32Number)) {
            cmsFloat32Number in[1];
            cmsFloat32Number out[1];
            in[0] = *(cmsFloat32Number*)Data;
            cmsPipelineEvalFloat(in, out, pipeline3);
        }
        
        cmsPipelineFree(pipeline3);
    }
    
    // Test cmsPipelineFree with NULL (documented as safe)
    cmsPipelineFree(NULL);
    
    // Clean up main pipeline
    cmsPipelineFree(pipeline);
    
    // Delete context
    cmsDeleteContext(context);
    
    return 0;
}