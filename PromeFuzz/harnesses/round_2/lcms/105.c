// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
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

static cmsPipeline* create_test_pipeline(cmsContext context, cmsUInt32Number input_channels, cmsUInt32Number output_channels) {
    cmsPipeline* pipeline = cmsPipelineAlloc(context, input_channels, output_channels);
    if (!pipeline) return NULL;
    
    // Add an identity stage to make the pipeline non-empty
    cmsStage* identity = cmsStageAllocIdentity(context, output_channels);
    if (!identity) {
        cmsPipelineFree(pipeline);
        return NULL;
    }
    
    cmsPipelineInsertStage(pipeline, cmsAT_END, identity);
    return pipeline;
}

static void dummy_free(cmsContext ContextID, void* data) {
    // Do nothing - just for testing
    (void)ContextID;
    (void)data;
}

static void* dummy_dup(cmsContext ContextID, const void* data) {
    // Return the same pointer - just for testing
    (void)ContextID;
    return (void*)data;
}

static void dummy_eval16(const cmsUInt16Number* in, cmsUInt16Number* out, const void* data) {
    // Identity transformation
    for (int i = 0; i < 4; i++) {
        out[i] = in[i];
    }
    (void)data;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Use first byte to determine input channels (1-16)
    cmsUInt32Number input_channels = (Data[0] % 16) + 1;
    
    // Use second byte to determine output channels (1-16)
    cmsUInt32Number output_channels = (Data[1] % 16) + 1;
    
    // Create first pipeline
    cmsPipeline* pipeline1 = create_test_pipeline(context, input_channels, output_channels);
    if (!pipeline1) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Use third byte to determine second pipeline's input channels
    cmsUInt32Number input_channels2 = (Data[2] % 16) + 1;
    
    // Create second pipeline
    cmsPipeline* pipeline2 = create_test_pipeline(context, input_channels2, output_channels);
    if (!pipeline2) {
        cmsPipelineFree(pipeline1);
        cmsDeleteContext(context);
        return 0;
    }
    
    // Test cmsPipelineCat
    cmsBool cat_result = cmsPipelineCat(pipeline1, pipeline2);
    
    // Test cmsPipelineOutputChannels
    cmsUInt32Number output_chans = cmsPipelineOutputChannels(pipeline1);
    
    // Test cmsPipelineDup
    cmsPipeline* dup_pipeline = cmsPipelineDup(pipeline1);
    if (dup_pipeline) {
        // Test cmsPipelineCheckAndRetreiveStages on duplicated pipeline
        cmsStage* stage1 = NULL;
        cmsStage* stage2 = NULL;
        
        // Check for identity stages (cmsSigIdentityElemType = 0x69643432)
        cmsBool check_result = cmsPipelineCheckAndRetreiveStages(
            dup_pipeline, 
            2, 
            cmsSigIdentityElemType, &stage1,
            cmsSigIdentityElemType, &stage2
        );
        
        // Test _cmsPipelineSetOptimizationParameters with dummy functions
        // Only do this if we have valid data
        if (Size >= 8) {
            void* private_data = (void*)(Data + 4);
            _cmsPipelineSetOptimizationParameters(
                dup_pipeline,
                dummy_eval16,
                private_data,
                dummy_free,
                dummy_dup
            );
        }
        
        // Clean up duplicated pipeline
        cmsPipelineFree(dup_pipeline);
    }
    
    // Clean up
    cmsPipelineFree(pipeline1);
    cmsPipelineFree(pipeline2);
    cmsDeleteContext(context);
    
    return 0;
}