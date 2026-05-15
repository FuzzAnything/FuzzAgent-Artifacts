// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
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

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static cmsPipeline* create_test_pipeline(cmsContext ctx, cmsUInt32Number in, cmsUInt32Number out) {
    cmsPipeline* lut = cmsPipelineAlloc(ctx, in, out);
    if (!lut) return NULL;
    
    // Add some basic stages to make the pipeline non-empty
    cmsStage* identity = cmsStageAllocIdentity(ctx, 3);
    if (identity) {
        cmsPipelineInsertStage(lut, cmsAT_END, identity);
    }
    
    return lut;
}

static void dummy_eval16(const cmsUInt16Number In[], cmsUInt16Number Out[], const void* Data) {
    // Simple identity transformation
    for (int i = 0; i < 3; i++) {
        Out[i] = In[i];
    }
}

static void* dummy_dup(cmsContext ContextID, const void* Data) {
    // Simple duplication that just returns the same pointer
    // Note: In real usage, this should allocate new memory
    return (void*)Data;
}

static void dummy_free(cmsContext ContextID, void* Data) {
    // Do nothing - just to satisfy the function signature
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = create_test_context();
    if (!ctx) return 0;
    
    // Use first byte to determine input/output channels (1-8 range)
    cmsUInt32Number in_channels = (Data[0] % 8) + 1;
    cmsUInt32Number out_channels = 3; // Default value
    
    // Only read Data[1] if we have at least 2 bytes
    if (Size >= 2) {
        out_channels = (Data[1] % 8) + 1;
    }
    
    // Create test pipelines
    cmsPipeline* lut1 = create_test_pipeline(ctx, in_channels, out_channels);
    cmsPipeline* lut2 = create_test_pipeline(ctx, out_channels, in_channels);
    
    if (!lut1 || !lut2) {
        if (lut1) cmsPipelineFree(lut1);
        if (lut2) cmsPipelineFree(lut2);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Test 1: _cmsPipelineSetOptimizationParameters
    // Use with caution as per instructions - using standard functions
    if (Size > 10) {
        _cmsPipelineSetOptimizationParameters(lut1, dummy_eval16, NULL, dummy_free, dummy_dup);
    }
    
    // Test 2: cmsPipelineInsertStage
    // Create a simple matrix stage
    cmsFloat64Number matrix[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    cmsStage* matrix_stage = cmsStageAllocMatrix(ctx, 3, 3, matrix, NULL);
    if (matrix_stage) {
        // Use data to determine insertion location if available
        cmsStageLoc loc = cmsAT_END; // Default
        if (Size >= 3) {
            loc = (Data[2] % 2) ? cmsAT_BEGIN : cmsAT_END;
        }
        cmsPipelineInsertStage(lut1, loc, matrix_stage);
    }
    
    // Test 3: cmsPipelineDup
    cmsPipeline* dup_lut = cmsPipelineDup(lut1);
    if (dup_lut) {
        // Test operations on duplicated pipeline
        cmsStage* identity2 = cmsStageAllocIdentity(ctx, 3);
        if (identity2) {
            cmsPipelineInsertStage(dup_lut, cmsAT_END, identity2);
        }
        cmsPipelineFree(dup_lut);
    }
    
    // Test 4: cmsPipelineCat
    // Only concatenate if channel counts match
    // Use API functions to get channel counts since struct is opaque
    if (cmsPipelineOutputChannels(lut1) == cmsPipelineInputChannels(lut2)) {
        cmsPipelineCat(lut1, lut2);
    }
    
    // Test 5: cmsPipelineCheckAndRetreiveStages
    if (lut1) {
        cmsStage* retrieved_stage = NULL;
        // Check for identity stage (common type)
        cmsPipelineCheckAndRetreiveStages(lut1, 1, cmsSigIdentityElemType, &retrieved_stage);
        
        // Try with matrix stage if we have enough data
        if (Size > 20) {
            cmsStage* stage1 = NULL;
            cmsStage* stage2 = NULL;
            // Look for two specific stage types
            cmsPipelineCheckAndRetreiveStages(lut1, 2, 
                cmsSigIdentityElemType, &stage1,
                cmsSigMatrixElemType, &stage2);
        }
    }
    
    // Test 6: Additional operations based on input data
    if (Size > 30) {
        // Create another pipeline for more complex testing
        cmsPipeline* lut3 = create_test_pipeline(ctx, 3, 3);
        if (lut3) {
            // Insert multiple stages based on input data
            size_t max_stages = Size % 5;
            for (size_t i = 0; i < max_stages; i++) {
                cmsStage* extra_stage = cmsStageAllocIdentity(ctx, 3);
                if (extra_stage) {
                    cmsPipelineInsertStage(lut3, cmsAT_END, extra_stage);
                }
            }
            
            // Duplicate and concatenate
            cmsPipeline* dup_lut3 = cmsPipelineDup(lut3);
            if (dup_lut3) {
                cmsPipelineCat(lut3, dup_lut3);
                cmsPipelineFree(dup_lut3);
            }
            
            cmsPipelineFree(lut3);
        }
    }
    
    // Cleanup
    cmsPipelineFree(lut1);
    cmsPipelineFree(lut2);
    cmsDeleteContext(ctx);
    
    return 0;
}