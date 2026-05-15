// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineUnlinkStage at cmslut.c:1555:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineEvalReverseFloat at cmslut.c:1753:19 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"

static cmsPipeline* create_test_pipeline(cmsContext context, cmsUInt32Number inputChannels, cmsUInt32Number outputChannels) {
    cmsPipeline* pipeline = cmsPipelineAlloc(context, inputChannels, outputChannels);
    if (!pipeline) return NULL;
    
    // Create tone curves for each output channel
    cmsToneCurve* curves[4] = {NULL, NULL, NULL, NULL};
    cmsStage* stage = NULL;
    
    for (cmsUInt32Number i = 0; i < outputChannels && i < 4; i++) {
        curves[i] = cmsBuildGamma(context, 2.2);
        if (!curves[i]) {
            // Clean up any curves we already created
            for (cmsUInt32Number j = 0; j < i; j++) {
                cmsFreeToneCurve(curves[j]);
            }
            cmsPipelineFree(pipeline);
            return NULL;
        }
    }
    
    stage = cmsStageAllocToneCurves(context, outputChannels, curves);
    
    // Free the individual curves after creating the stage
    for (cmsUInt32Number i = 0; i < outputChannels && i < 4; i++) {
        cmsFreeToneCurve(curves[i]);
    }
    
    if (!stage) {
        cmsPipelineFree(pipeline);
        return NULL;
    }
    
    cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage);
    return pipeline;
}

static cmsStage* create_test_stage(cmsContext context, cmsUInt32Number channels) {
    cmsToneCurve* curves[4] = {NULL, NULL, NULL, NULL};
    cmsStage* stage = NULL;
    
    for (cmsUInt32Number i = 0; i < channels && i < 4; i++) {
        curves[i] = cmsBuildGamma(context, 1.8);
        if (!curves[i]) {
            // Clean up any curves we already created
            for (cmsUInt32Number j = 0; j < i; j++) {
                cmsFreeToneCurve(curves[j]);
            }
            return NULL;
        }
    }
    
    stage = cmsStageAllocToneCurves(context, channels, curves);
    
    // Free the individual curves after creating the stage
    for (cmsUInt32Number i = 0; i < channels && i < 4; i++) {
        cmsFreeToneCurve(curves[i]);
    }
    
    return stage;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Use first byte to determine pipeline configuration
    uint8_t config = Data[0];
    cmsUInt32Number inputChannels = (config & 0x01) ? 4 : 3;
    cmsUInt32Number outputChannels = (config & 0x02) ? 4 : 3;
    
    // Ensure channels are within valid range
    if (inputChannels < 1 || inputChannels > 4) inputChannels = 3;
    if (outputChannels < 1 || outputChannels > 4) outputChannels = 3;
    
    // Create test pipeline
    cmsPipeline* pipeline = create_test_pipeline(context, inputChannels, outputChannels);
    if (!pipeline) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Test cmsPipelineCheckAndRetreiveStages
    if (Size > 1) {
        cmsStage* retrieved_stage = NULL;
        cmsStage* retrieved_stage2 = NULL;
        cmsPipelineCheckAndRetreiveStages(pipeline, 1, cmsSigCurveSetElemType, &retrieved_stage);
        
        // Try with multiple expected types
        if (Size > 2) {
            cmsPipelineCheckAndRetreiveStages(pipeline, 2, cmsSigCurveSetElemType, &retrieved_stage, 
                                              cmsSigMatrixElemType, &retrieved_stage2);
        }
    }
    
    // Test cmsPipelineInsertStage
    if (Size > 3) {
        cmsStage* new_stage = create_test_stage(context, outputChannels);
        if (new_stage) {
            cmsStageLoc loc = (Data[1] & 0x01) ? cmsAT_BEGIN : cmsAT_END;
            cmsPipelineInsertStage(pipeline, loc, new_stage);
        }
    }
    
    // Test cmsPipelineUnlinkStage
    if (Size > 4) {
        cmsStage* unlinked_stage = NULL;
        cmsStageLoc loc = (Data[2] & 0x01) ? cmsAT_BEGIN : cmsAT_END;
        cmsPipelineUnlinkStage(pipeline, loc, &unlinked_stage);
        
        // If we got a stage back, free it
        if (unlinked_stage) {
            cmsStageFree(unlinked_stage);
        }
    }
    
    // Test cmsPipelineEvalFloat
    if (Size >= sizeof(cmsFloat32Number) * inputChannels + 5) {
        cmsFloat32Number in[4] = {0};
        cmsFloat32Number out[4] = {0};
        
        // Ensure we don't read beyond input buffer
        size_t float_size = sizeof(cmsFloat32Number) * inputChannels;
        if (Size >= 5 + float_size) {
            memcpy(in, Data + 5, float_size);
            cmsPipelineEvalFloat(in, out, pipeline);
        }
    }
    
    // Test cmsPipelineEvalReverseFloat
    if (Size >= sizeof(cmsFloat32Number) * outputChannels * 2 + 10) {
        cmsFloat32Number target[4] = {0};
        cmsFloat32Number result[4] = {0};
        cmsFloat32Number hint[4] = {0};
        
        size_t offset = 10;
        size_t target_size = sizeof(cmsFloat32Number) * outputChannels;
        size_t hint_size = sizeof(cmsFloat32Number) * outputChannels;
        
        if (Size >= offset + target_size + hint_size) {
            memcpy(target, Data + offset, target_size);
            memcpy(hint, Data + offset + target_size, hint_size);
            
            // Only call for 3->3 or 4->3 LUTs as documented
            if ((inputChannels == 3 || inputChannels == 4) && outputChannels == 3) {
                cmsPipelineEvalReverseFloat(target, result, hint, pipeline);
            }
        }
    }
    
    // Test cmsPipelineFree
    cmsPipelineFree(pipeline);
    
    cmsDeleteContext(context);
    return 0;
}