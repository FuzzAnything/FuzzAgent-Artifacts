// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsPipeline* create_test_pipeline(cmsContext ctx, cmsUInt32Number inputChannels, cmsUInt32Number outputChannels) {
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, inputChannels, outputChannels);
    if (!pipeline) return NULL;
    
    // Add an identity stage to make the pipeline non-empty
    cmsStage* identity = cmsStageAllocIdentity(ctx, inputChannels);
    if (!identity) {
        cmsPipelineFree(pipeline);
        return NULL;
    }
    cmsPipelineInsertStage(pipeline, cmsAT_END, identity);
    
    return pipeline;
}

static void dummy_eval16(const cmsUInt16Number In[], cmsUInt16Number Out[], const void* Data) {
    // Simple copy for testing
    for (int i = 0; i < 16; i++) {
        Out[i] = In[i];
    }
}

static void* dummy_dup(cmsContext ContextID, const void* Data) {
    (void)ContextID;
    (void)Data;
    return malloc(1); // Minimal allocation
}

static void dummy_free(cmsContext ContextID, void* Data) {
    (void)ContextID;
    free(Data);
}

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Use first bytes to determine pipeline configurations
    cmsUInt32Number input1 = (Data[0] % 8) + 1;
    cmsUInt32Number output1 = (Data[1] % 8) + 1;
    cmsUInt32Number input2 = (Data[2] % 8) + 1;
    cmsUInt32Number output2 = (Data[3] % 8) + 1;
    
    // Create two test pipelines
    cmsPipeline* l1 = create_test_pipeline(ctx, input1, output1);
    cmsPipeline* l2 = create_test_pipeline(ctx, input2, output2);
    
    if (!l1 || !l2) {
        if (l1) cmsPipelineFree(l1);
        if (l2) cmsPipelineFree(l2);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Test cmsPipelineCat
    cmsBool cat_result = cmsPipelineCat(l1, l2);
    (void)cat_result; // Result may vary based on channel compatibility
    
    // Test cmsPipelineOutputChannels
    cmsUInt32Number out_channels = cmsPipelineOutputChannels(l1);
    (void)out_channels;
    
    // Test _cmsPipelineSetOptimizationParameters with dummy functions
    _cmsPipelineSetOptimizationParameters(l1, dummy_eval16, NULL, dummy_free, dummy_dup);
    
    // Test cmsPipelineDup
    cmsPipeline* dup_pipeline = cmsPipelineDup(l1);
    if (dup_pipeline) {
        // Test cmsPipelineCheckAndRetreiveStages
        cmsStage* stage1 = NULL;
        cmsStage* stage2 = NULL;
        cmsBool check_result = cmsPipelineCheckAndRetreiveStages(
            dup_pipeline, 2, 
            cmsSigIdentityElemType, &stage1,
            cmsSigIdentityElemType, &stage2
        );
        (void)check_result;
        
        cmsPipelineFree(dup_pipeline);
    }
    
    // Cleanup
    cmsPipelineFree(l1);
    cmsPipelineFree(l2);
    cmsDeleteContext(ctx);
    
    return 0;
}