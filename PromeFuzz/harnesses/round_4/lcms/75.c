// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineUnlinkStage at cmslut.c:1555:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineEvalReverseFloat at cmslut.c:1753:19 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "lcms2.h"

static cmsPipeline* create_test_pipeline(cmsContext ctx) {
    // Create a simple pipeline with 3 input and 3 output channels
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (!pipeline) return NULL;
    
    // Create a simple tone curve stage
    cmsToneCurve* curve = cmsBuildGamma(ctx, 2.2);
    if (!curve) {
        cmsPipelineFree(pipeline);
        return NULL;
    }
    
    cmsToneCurve* curves[3] = {curve, curve, curve};
    cmsStage* stage = cmsStageAllocToneCurves(ctx, 3, curves);
    cmsFreeToneCurve(curve);
    
    if (!stage) {
        cmsPipelineFree(pipeline);
        return NULL;
    }
    
    // Insert the stage
    if (!cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage)) {
        cmsStageFree(stage);
        cmsPipelineFree(pipeline);
        return NULL;
    }
    
    return pipeline;
}

static cmsStage* create_test_stage(cmsContext ctx) {
    // Create a simple tone curve for a test stage
    cmsToneCurve* curve = cmsBuildGamma(ctx, 1.8);
    if (!curve) return NULL;
    
    cmsToneCurve* curves[3] = {curve, curve, curve};
    cmsStage* stage = cmsStageAllocToneCurves(ctx, 3, curves);
    cmsFreeToneCurve(curve);
    
    return stage;
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create test pipeline
    cmsPipeline* pipeline = create_test_pipeline(ctx);
    if (!pipeline) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Test cmsPipelineEvalFloat
    if (Size >= 12) {
        cmsFloat32Number In[3];
        cmsFloat32Number Out[3];
        
        // Use first 12 bytes for input floats
        memcpy(In, Data, 12);
        
        cmsPipelineEvalFloat(In, Out, pipeline);
    }
    
    // Test cmsPipelineEvalReverseFloat
    if (Size >= 24) {
        cmsFloat32Number Target[3];
        cmsFloat32Number Result[3];
        cmsFloat32Number Hint[3];
        
        // Use next 24 bytes for target and hint
        memcpy(Target, Data + 12, 12);
        memcpy(Hint, Data + 24, 12);
        
        cmsPipelineEvalReverseFloat(Target, Result, Hint, pipeline);
    }
    
    // Test cmsPipelineInsertStage
    if (Size >= 28) {
        cmsStage* stage = create_test_stage(ctx);
        if (stage) {
            // Use byte 28 to determine location
            cmsStageLoc loc = (Data[28] & 1) ? cmsAT_BEGIN : cmsAT_END;
            cmsPipelineInsertStage(pipeline, loc, stage);
            
            // Don't free stage here as it's now owned by pipeline
        }
    }
    
    // Test cmsPipelineCheckAndRetreiveStages
    if (pipeline) {
        cmsStage* retrieved_stage = NULL;
        cmsPipelineCheckAndRetreiveStages(pipeline, 1, cmsSigCurveSetElemType, &retrieved_stage);
    }
    
    // Test cmsPipelineUnlinkStage
    if (Size >= 29) {
        cmsStage* unlinked_stage = NULL;
        // Use byte 29 to determine location
        cmsStageLoc loc = (Data[29] & 1) ? cmsAT_BEGIN : cmsAT_END;
        cmsPipelineUnlinkStage(pipeline, loc, &unlinked_stage);
        
        if (unlinked_stage) {
            cmsStageFree(unlinked_stage);
        }
    }
    
    // Clean up
    cmsPipelineFree(pipeline);
    cmsDeleteContext(ctx);
    
    return 0;
}