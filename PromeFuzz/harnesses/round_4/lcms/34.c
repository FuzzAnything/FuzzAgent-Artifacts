// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsPipeline* create_test_pipeline_from_data(const uint8_t* data, size_t size, cmsContext ctx) {
    if (size < 2) return NULL;
    
    cmsUInt32Number inputChannels = (data[0] % 16) + 1;
    cmsUInt32Number outputChannels = (data[1] % 16) + 1;
    
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, inputChannels, outputChannels);
    if (!pipeline) return NULL;
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    cmsPipeline* pipeline = create_test_pipeline_from_data(Data, Size, context);
    if (!pipeline) {
        cmsDeleteContext(context);
        return 0;
    }
    
    cmsUInt32Number stageCount = cmsPipelineStageCount(pipeline);
    cmsUInt32Number inputChannels = cmsPipelineInputChannels(pipeline);
    cmsUInt32Number outputChannels = cmsPipelineOutputChannels(pipeline);
    
    cmsPipeline* dupPipeline = cmsPipelineDup(pipeline);
    if (dupPipeline) {
        cmsUInt32Number dupStageCount = cmsPipelineStageCount(dupPipeline);
        cmsUInt32Number dupInputChannels = cmsPipelineInputChannels(dupPipeline);
        cmsUInt32Number dupOutputChannels = cmsPipelineOutputChannels(dupPipeline);
        
        cmsPipelineFree(dupPipeline);
    }
    
    if (stageCount == 0) {
        cmsStage* stage1 = NULL;
        cmsStage* stage2 = NULL;
        
        cmsBool result = cmsPipelineCheckAndRetreiveStages(
            pipeline, 
            0
        );
        
        result = cmsPipelineCheckAndRetreiveStages(
            pipeline, 
            1,
            cmsSigCurveSetElemType,
            &stage1
        );
        
        result = cmsPipelineCheckAndRetreiveStages(
            pipeline, 
            2,
            cmsSigCurveSetElemType,
            &stage1,
            cmsSigMatrixElemType,
            &stage2
        );
    }
    
    cmsPipelineFree(pipeline);
    cmsDeleteContext(context);
    
    return 0;
}