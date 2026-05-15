// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsGetPipelineContextID at cmslut.c:1407:22 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcms2.h"

static cmsPipeline* create_test_pipeline(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 2) return NULL;
    
    cmsUInt32Number input_channels = (data[0] % 16) + 1;
    cmsUInt32Number output_channels = (data[1] % 16) + 1;
    
    return cmsPipelineAlloc(ctx, input_channels, output_channels);
}

static void add_dummy_stage(cmsPipeline* pipeline) {
    if (!pipeline) return;
    
    cmsContext ctx = cmsGetPipelineContextID(pipeline);
    cmsToneCurve* curve = cmsBuildGamma(ctx, 1.0);
    if (!curve) return;
    
    cmsUInt32Number nChannels = cmsPipelineInputChannels(pipeline);
    if (nChannels > 16) nChannels = 16;
    
    cmsToneCurve* curves[16];
    for (cmsUInt32Number i = 0; i < nChannels; i++) {
        curves[i] = curve;
    }
    
    cmsStage* stage = cmsStageAllocToneCurves(ctx, nChannels, curves);
    if (stage) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
    }
    
    cmsFreeToneCurve(curve);
}

int LLVMFuzzerTestOneInput_34(const uint8_t *data, size_t size) {
    if (size < 2) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    cmsPipeline* pipeline = create_test_pipeline(context, data, size);
    if (!pipeline) {
        cmsDeleteContext(context);
        return 0;
    }
    
    size_t offset = 2;
    size_t stages_to_add = (size > 2) ? (data[2] % 5) : 0;
    
    for (size_t i = 0; i < stages_to_add; i++) {
        add_dummy_stage(pipeline);
    }
    
    cmsUInt32Number stage_count = cmsPipelineStageCount(pipeline);
    cmsUInt32Number input_channels = cmsPipelineInputChannels(pipeline);
    cmsUInt32Number output_channels = cmsPipelineOutputChannels(pipeline);
    
    cmsPipeline* dup_pipeline = cmsPipelineDup(pipeline);
    if (dup_pipeline) {
        cmsUInt32Number dup_stage_count = cmsPipelineStageCount(dup_pipeline);
        cmsUInt32Number dup_input_channels = cmsPipelineInputChannels(dup_pipeline);
        cmsUInt32Number dup_output_channels = cmsPipelineOutputChannels(dup_pipeline);
        
        if (size > offset + sizeof(cmsStageSignature)) {
            cmsStage* retrieved_stage = NULL;
            cmsStageSignature expected_type = cmsSigCurveSetElemType;
            
            cmsBool ret = cmsPipelineCheckAndRetreiveStages(
                pipeline, 
                1, 
                expected_type, 
                &retrieved_stage
            );
            
            if (dup_pipeline) {
                cmsStage* dup_retrieved_stage = NULL;
                cmsBool dup_ret = cmsPipelineCheckAndRetreiveStages(
                    dup_pipeline, 
                    1, 
                    expected_type, 
                    &dup_retrieved_stage
                );
            }
        }
        
        cmsPipelineFree(dup_pipeline);
    }
    
    cmsPipelineFree(pipeline);
    cmsDeleteContext(context);
    
    return 0;
}