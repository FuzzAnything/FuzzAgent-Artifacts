// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
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

static void dummy_free(cmsContext ContextID, void* ptr) {
    // Do nothing - used for testing
    (void)ContextID;
    (void)ptr;
}

static void* dummy_dup(cmsContext ContextID, const void* ptr) {
    // Return the same pointer - used for testing
    (void)ContextID;
    return (void*)ptr;
}

static void dummy_eval16(const cmsUInt16Number In[], cmsUInt16Number Out[], const void* data) {
    // Simple identity transform for testing
    (void)data;
    for (int i = 0; i < 4; i++) {
        Out[i] = In[i];
    }
}

static cmsPipeline* create_test_pipeline(cmsUInt32Number input_channels, cmsUInt32Number output_channels) {
    cmsPipeline* pipeline = cmsPipelineAlloc(NULL, input_channels, output_channels);
    if (!pipeline) return NULL;
    
    // Add a simple identity CLUT stage
    cmsStage* stage = cmsStageAllocCLut16bit(NULL, 2, input_channels, output_channels, NULL);
    if (stage) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Use first byte to determine test scenario
    uint8_t scenario = Data[0] % 8;
    
    // Create test pipelines
    cmsPipeline* pipeline1 = create_test_pipeline(3, 3);
    cmsPipeline* pipeline2 = create_test_pipeline(3, 3);
    
    if (!pipeline1 || !pipeline2) {
        if (pipeline1) cmsPipelineFree(pipeline1);
        if (pipeline2) cmsPipelineFree(pipeline2);
        return 0;
    }
    
    switch (scenario) {
        case 0: {
            // Test _cmsPipelineSetOptimizationParameters
            _cmsPipelineSetOptimizationParameters(pipeline1, dummy_eval16, NULL, dummy_free, dummy_dup);
            break;
        }
        case 1: {
            // Test cmsPipelineCat
            cmsPipelineCat(pipeline1, pipeline2);
            break;
        }
        case 2: {
            // Test cmsPipelineDup
            cmsPipeline* dup = cmsPipelineDup(pipeline1);
            if (dup) {
                cmsPipelineFree(dup);
            }
            break;
        }
        case 3: {
            // Test cmsPipelineCheckAndRetreiveStages
            cmsStage* stage1 = NULL;
            cmsPipelineCheckAndRetreiveStages(pipeline1, 1, 
                cmsSigCLutElemType, &stage1);
            break;
        }
        case 4: {
            // Test cmsPipelineInputChannels
            cmsUInt32Number channels = cmsPipelineInputChannels(pipeline1);
            (void)channels; // Use variable to avoid unused warning
            break;
        }
        case 5: {
            // Test combination: dup then cat
            cmsPipeline* dup = cmsPipelineDup(pipeline1);
            if (dup) {
                cmsPipelineCat(dup, pipeline2);
                cmsPipelineFree(dup);
            }
            break;
        }
        case 6: {
            // Test combination: set params then dup
            _cmsPipelineSetOptimizationParameters(pipeline1, dummy_eval16, NULL, dummy_free, dummy_dup);
            cmsPipeline* dup = cmsPipelineDup(pipeline1);
            if (dup) {
                cmsPipelineFree(dup);
            }
            break;
        }
        case 7: {
            // Test multiple operations
            cmsPipeline* dup = cmsPipelineDup(pipeline1);
            if (dup) {
                _cmsPipelineSetOptimizationParameters(dup, dummy_eval16, NULL, dummy_free, dummy_dup);
                cmsUInt32Number channels = cmsPipelineInputChannels(dup);
                (void)channels;
                cmsPipelineCat(dup, pipeline2);
                cmsPipelineFree(dup);
            }
            break;
        }
    }
    
    // Cleanup
    cmsPipelineFree(pipeline1);
    cmsPipelineFree(pipeline2);
    
    return 0;
}