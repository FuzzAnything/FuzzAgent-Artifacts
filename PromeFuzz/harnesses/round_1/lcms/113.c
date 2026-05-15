// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// _cmsPipelineSetOptimizationParameters at cmslut.c:1674:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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

static void dummy_eval16(const cmsUInt16Number In[], cmsUInt16Number Out[], const void* Data) {
    // Do nothing
}

static void* dummy_dup(cmsContext ContextID, const void* Data) {
    return NULL;
}

static void dummy_free(cmsContext ContextID, void* Data) {
    // Do nothing
}

static cmsPipeline* create_test_pipeline(cmsContext ctx, cmsUInt32Number inputChannels, cmsUInt32Number outputChannels, cmsUInt32Number stageCount) {
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, inputChannels, outputChannels);
    if (!pipeline) return NULL;
    
    for (cmsUInt32Number i = 0; i < stageCount && i < 5; i++) {
        cmsStage* stage = cmsStageAllocIdentity(ctx, outputChannels);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        }
    }
    return pipeline;
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Use first few bytes to determine parameters
    cmsUInt32Number params[6];
    size_t paramSize = sizeof(params) < Size ? sizeof(params) : Size;
    memcpy(params, Data, paramSize);
    
    cmsUInt32Number inputChannels1 = (params[0] % 16) + 1;
    cmsUInt32Number outputChannels1 = (params[1] % 16) + 1;
    cmsUInt32Number inputChannels2 = (params[2] % 16) + 1;
    cmsUInt32Number outputChannels2 = (params[3] % 16) + 1;
    cmsUInt32Number stageCount1 = params[4] % 5;
    cmsUInt32Number stageCount2 = params[5] % 5;
    
    // Test 1: cmsPipelineOutputChannels
    cmsPipeline* lut1 = create_test_pipeline(ctx, inputChannels1, outputChannels1, stageCount1);
    if (lut1) {
        cmsUInt32Number outChannels = cmsPipelineOutputChannels(lut1);
        (void)outChannels; // Use result to avoid unused warning
    }
    
    // Test 2: cmsPipelineCat
    cmsPipeline* lut2 = create_test_pipeline(ctx, inputChannels2, outputChannels2, stageCount2);
    if (lut1 && lut2) {
        cmsBool catResult = cmsPipelineCat(lut1, lut2);
        (void)catResult;
    }
    
    // Test 3: cmsPipelineCheckAndRetreiveStages
    if (lut1 && stageCount1 > 0) {
        cmsStage* stage = NULL;
        cmsStageSignature sig = cmsSigIdentityElemType;
        cmsPipelineCheckAndRetreiveStages(lut1, 1, sig, &stage);
    }
    
    // Test 4: cmsPipelineDup
    cmsPipeline* dupLut = NULL;
    if (lut1) {
        dupLut = cmsPipelineDup(lut1);
    }
    
    // Test 5: _cmsPipelineSetOptimizationParameters
    if (lut1) {
        _cmsPipelineSetOptimizationParameters(lut1, dummy_eval16, NULL, dummy_free, dummy_dup);
    }
    
    // Test 6: cmsPipelineFree
    cmsPipelineFree(lut1);
    cmsPipelineFree(lut2);
    cmsPipelineFree(dupLut);
    
    // Test with NULL inputs
    cmsPipelineOutputChannels(NULL);
    cmsPipelineCat(NULL, NULL);
    cmsPipelineFree(NULL);
    cmsPipelineDup(NULL);
    _cmsPipelineSetOptimizationParameters(NULL, dummy_eval16, NULL, dummy_free, dummy_dup);
    
    cmsDeleteContext(ctx);
    return 0;
}