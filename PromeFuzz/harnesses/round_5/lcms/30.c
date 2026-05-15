// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
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
}

static void* dummy_dup(cmsContext ContextID, const void* ptr) {
    return (void*)ptr; // Simple duplication for testing
}

static void dummy_eval16(const cmsUInt16Number In[],
                         cmsUInt16Number Out[],
                         const void* Data) {
    // Simple evaluation that copies input to output
    // We don't know channel count, so just copy first few
    for (int i = 0; i < 16; i++) {
        Out[i] = In[i];
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create some pipelines for testing
    cmsPipeline* pipe1 = cmsPipelineAlloc(ctx, 3, 3);
    cmsPipeline* pipe2 = cmsPipelineAlloc(ctx, 3, 3);
    
    if (!pipe1 || !pipe2) {
        if (pipe1) cmsPipelineFree(pipe1);
        if (pipe2) cmsPipelineFree(pipe2);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Test 1: _cmsPipelineSetOptimizationParameters
    // Use dummy functions for testing
    _cmsPipelineSetOptimizationParameters(pipe1, 
                                         dummy_eval16,
                                         NULL,
                                         dummy_free,
                                         dummy_dup);
    
    // Test 2: cmsPipelineCat
    // Only concatenate if we have enough data
    if (Size > 8) {
        cmsPipelineCat(pipe1, pipe2);
    }
    
    // Test 3: cmsPipelineDup
    cmsPipeline* dup_pipe = cmsPipelineDup(pipe1);
    if (dup_pipe) {
        // Test 4: cmsPipelineOutputChannels
        cmsUInt32Number out_channels = cmsPipelineOutputChannels(dup_pipe);
        
        // Test 5: cmsPipelineCheckAndRetreiveStages
        // Try to retrieve stages if we have any
        cmsStage* stage1 = NULL;
        cmsStage* stage2 = NULL;
        
        // Create a simple stage if we have enough data
        if (Size > 16) {
            cmsToneCurve* curves[3];
            for (int i = 0; i < 3; i++) {
                curves[i] = cmsBuildGamma(ctx, 2.2);
            }
            cmsStage* gamma_stage = cmsStageAllocToneCurves(ctx, 3, curves);
            for (int i = 0; i < 3; i++) {
                cmsFreeToneCurve(curves[i]);
            }
            
            if (gamma_stage) {
                cmsPipelineInsertStage(pipe1, cmsAT_END, gamma_stage);
                
                // Now try to retrieve it
                cmsPipelineCheckAndRetreiveStages(pipe1, 1, 
                                                 cmsSigCurveSetElemType, 
                                                 &stage1);
            }
        }
        
        cmsPipelineFree(dup_pipe);
    }
    
    // Test 6: cmsPipelineFree
    cmsPipelineFree(pipe1);
    cmsPipelineFree(pipe2);
    
    cmsDeleteContext(ctx);
    
    return 0;
}