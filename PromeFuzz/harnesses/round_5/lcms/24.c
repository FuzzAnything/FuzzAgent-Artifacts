// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsPipeline* create_test_pipeline(cmsContext context, cmsUInt32Number input_channels, cmsUInt32Number output_channels) {
    cmsPipeline* pipeline = cmsPipelineAlloc(context, input_channels, output_channels);
    if (!pipeline) return NULL;
    
    // Add a simple identity matrix stage to make the pipeline non-empty
    cmsFloat64Number identity_matrix[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    cmsStage* matrix_stage = cmsStageAllocMatrix(context, 3, 3, identity_matrix, NULL);
    if (matrix_stage) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, matrix_stage);
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Use first 4 bytes to determine pipeline configurations
    cmsUInt32Number input_channels1 = (data[0] % 16) + 1;
    cmsUInt32Number output_channels1 = (data[1] % 16) + 1;
    cmsUInt32Number input_channels2 = (data[2] % 16) + 1;
    cmsUInt32Number output_channels2 = (data[3] % 16) + 1;
    
    cmsPipeline* pipeline1 = create_test_pipeline(context, input_channels1, output_channels1);
    cmsPipeline* pipeline2 = create_test_pipeline(context, input_channels2, output_channels2);
    
    if (!pipeline1 || !pipeline2) {
        if (pipeline1) cmsPipelineFree(pipeline1);
        if (pipeline2) cmsPipelineFree(pipeline2);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Test cmsPipelineSetSaveAs8bitsFlag - only on valid pipelines
    cmsBool previous_flag = cmsPipelineSetSaveAs8bitsFlag(pipeline1, (data[0] & 0x01) ? TRUE : FALSE);
    (void)previous_flag;
    
    // Test cmsPipelineCat - only on valid pipelines
    cmsBool cat_result = cmsPipelineCat(pipeline1, pipeline2);
    (void)cat_result;
    
    // Test cmsPipelineGetPtrToFirstStage - only on valid pipelines
    cmsStage* first_stage = cmsPipelineGetPtrToFirstStage(pipeline1);
    (void)first_stage;
    
    // Test cmsPipelineDup - only on valid pipelines
    cmsPipeline* dup_pipeline = cmsPipelineDup(pipeline1);
    
    // Test cmsPipelineEvalFloat
    if (size >= sizeof(cmsFloat32Number) * 16) {
        cmsFloat32Number in[16];
        cmsFloat32Number out[16];
        
        // Use data for input values (limited to reasonable range)
        for (int i = 0; i < 16 && i < (int)(size / sizeof(cmsFloat32Number)); i++) {
            float val;
            memcpy(&val, data + i * sizeof(float), sizeof(float));
            // Clamp to reasonable range for color values
            if (val < 0.0f) val = 0.0f;
            if (val > 100.0f) val = 100.0f;
            in[i] = val;
        }
        
        // We'll assume the pipelines can handle up to 16 channels
        // The actual validation happens inside the library function
        cmsPipelineEvalFloat(in, out, pipeline1);
        
        if (dup_pipeline) {
            cmsPipelineEvalFloat(in, out, dup_pipeline);
        }
    }
    
    // Cleanup
    cmsPipelineFree(pipeline1);
    cmsPipelineFree(pipeline2);
    if (dup_pipeline) {
        cmsPipelineFree(dup_pipeline);
    }
    
    cmsDeleteContext(context);
    
    return 0;
}