// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineEvalReverseFloat at cmslut.c:1753:19 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineUnlinkStage at cmslut.c:1555:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsPipeline* create_test_pipeline(cmsContext ctx, cmsUInt32Number inputChannels, cmsUInt32Number outputChannels) {
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, inputChannels, outputChannels);
    if (!pipeline) return NULL;
    
    // Create a simple identity tone curve stage
    cmsToneCurve* curve = cmsBuildGamma(ctx, 1.0);
    if (!curve) {
        cmsPipelineFree(pipeline);
        return NULL;
    }
    
    cmsStage* stage = cmsStageAllocToneCurves(ctx, 1, &curve);
    cmsFreeToneCurve(curve);
    
    if (!stage) {
        cmsPipelineFree(pipeline);
        return NULL;
    }
    
    cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage);
    return pipeline;
}

static cmsStage* create_test_stage(cmsContext ctx) {
    // Create a simple matrix stage
    cmsFloat64Number matrix[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    return cmsStageAllocMatrix(ctx, 3, 3, matrix, NULL);
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Use first byte to determine pipeline configuration
    uint8_t config = Data[0];
    cmsUInt32Number inputChannels = (config & 0x03) + 3;  // 3-6 channels
    cmsUInt32Number outputChannels = ((config >> 2) & 0x03) + 3;  // 3-6 channels
    
    // Create test pipeline
    cmsPipeline* pipeline = create_test_pipeline(ctx, inputChannels, outputChannels);
    if (!pipeline) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Test cmsPipelineEvalReverseFloat
    if (Size >= 48) {
        cmsFloat32Number Target[4], Result[4], Hint[4];
        
        // Extract Target array (up to 4 floats)
        size_t targetSize = (inputChannels < 4 ? inputChannels : 4) * sizeof(cmsFloat32Number);
        memcpy(Target, Data + 1, targetSize > 32 ? 32 : targetSize);
        
        // Extract Hint array (up to 4 floats)
        size_t hintOffset = 1 + targetSize;
        size_t hintSize = (outputChannels < 4 ? outputChannels : 4) * sizeof(cmsFloat32Number);
        if (hintOffset + hintSize <= Size) {
            memcpy(Hint, Data + hintOffset, hintSize);
            cmsPipelineEvalReverseFloat(Target, Result, Hint, pipeline);
        }
    }
    
    // Test cmsPipelineInsertStage
    if (Size >= 8) {
        cmsStage* stage = create_test_stage(ctx);
        if (stage) {
            cmsStageLoc loc = (Data[1] & 0x01) ? cmsAT_BEGIN : cmsAT_END;
            cmsPipelineInsertStage(pipeline, loc, stage);
            // Stage is now owned by pipeline
        }
    }
    
    // Test cmsPipelineUnlinkStage
    if (Size >= 12) {
        cmsStage* unlinkedStage = NULL;
        cmsStageLoc loc = (Data[2] & 0x01) ? cmsAT_BEGIN : cmsAT_END;
        cmsPipelineUnlinkStage(pipeline, loc, &unlinkedStage);
        if (unlinkedStage) {
            cmsStageFree(unlinkedStage);
        }
    }
    
    // Test cmsPipelineEvalFloat
    if (Size >= 24) {
        cmsFloat32Number In[4], Out[4];
        
        // Extract In array (up to 4 floats)
        size_t inSize = (inputChannels < 4 ? inputChannels : 4) * sizeof(cmsFloat32Number);
        if (3 + inSize <= Size) {
            memcpy(In, Data + 3, inSize);
            cmsPipelineEvalFloat(In, Out, pipeline);
        }
    }
    
    // Test cmsPipelineCheckAndRetreiveStages
    if (Size >= 16) {
        cmsStage* stage1 = NULL;
        cmsStage* stage2 = NULL;
        
        // Check for tone curve stage (common in our test pipeline)
        cmsPipelineCheckAndRetreiveStages(pipeline, 1, 
                                         cmsSigCurveSetElemType, &stage1);
        
        // Try checking for multiple stage types
        if (Data[3] & 0x01) {
            cmsPipelineCheckAndRetreiveStages(pipeline, 2,
                                             cmsSigCurveSetElemType, &stage1,
                                             cmsSigMatrixElemType, &stage2);
        }
    }
    
    // Cleanup
    cmsPipelineFree(pipeline);
    cmsDeleteContext(ctx);
    
    return 0;
}