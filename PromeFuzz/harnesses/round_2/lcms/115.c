// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void destroy_test_context(cmsContext context) {
    if (context) {
        cmsDeleteContext(context);
    }
}

static cmsPipeline* create_test_pipeline(cmsContext ctx, cmsUInt32Number input, cmsUInt32Number output) {
    return cmsPipelineAlloc(ctx, input, output);
}

static void test_pipeline_functions(cmsPipeline* pipeline) {
    if (!pipeline) return;
    
    cmsUInt32Number input = cmsPipelineInputChannels(pipeline);
    cmsUInt32Number output = cmsPipelineOutputChannels(pipeline);
    cmsUInt32Number stage_count = cmsPipelineStageCount(pipeline);
    
    // Test duplication
    cmsPipeline* dup = cmsPipelineDup(pipeline);
    if (dup) {
        cmsPipelineInputChannels(dup);
        cmsPipelineOutputChannels(dup);
        cmsPipelineStageCount(dup);
        cmsPipelineFree(dup);
    }
    
    // Test stage retrieval with various combinations
    cmsStage* stage1 = NULL;
    cmsStage* stage2 = NULL;
    cmsStage* stage3 = NULL;
    
    // Test with 0 expected stages
    cmsPipelineCheckAndRetreiveStages(pipeline, 0);
    
    // Test with 1 expected stage (unlikely to match empty pipeline)
    cmsPipelineCheckAndRetreiveStages(pipeline, 1, 0, &stage1);
    
    // Test with 2 expected stages
    cmsPipelineCheckAndRetreiveStages(pipeline, 2, 0, &stage1, 1, &stage2);
    
    // Test with 3 expected stages
    cmsPipelineCheckAndRetreiveStages(pipeline, 3, 0, &stage1, 1, &stage2, 2, &stage3);
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    // Use first 4 bytes for input channels, next 4 for output channels
    cmsUInt32Number input_channels = *(const cmsUInt32Number*)Data;
    cmsUInt32Number output_channels = *(const cmsUInt32Number*)(Data + 4);
    
    // Limit to reasonable values to avoid excessive memory allocation
    if (input_channels > 100) input_channels = 100;
    if (output_channels > 100) output_channels = 100;
    if (input_channels == 0) input_channels = 1;
    if (output_channels == 0) output_channels = 1;
    
    cmsContext ctx = create_test_context();
    if (!ctx) return 0;
    
    cmsPipeline* pipeline = create_test_pipeline(ctx, input_channels, output_channels);
    
    if (pipeline) {
        test_pipeline_functions(pipeline);
        cmsPipelineFree(pipeline);
    }
    
    destroy_test_context(ctx);
    
    return 0;
}