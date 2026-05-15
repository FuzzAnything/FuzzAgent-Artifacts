// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    // Create a context for the pipeline operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Use first 8 bytes to determine input/output channels (limit to reasonable values)
    cmsUInt32Number inputChannels = 1;
    cmsUInt32Number outputChannels = 1;
    
    if (Size >= 8) {
        // Limit channels to reasonable values (1-16) to avoid excessive memory usage
        inputChannels = (Data[0] % 16) + 1;
        outputChannels = (Data[1] % 16) + 1;
    }

    // Test cmsPipelineAlloc
    cmsPipeline *pipeline = cmsPipelineAlloc(ctx, inputChannels, outputChannels);
    if (!pipeline) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Test cmsPipelineInputChannels
    cmsUInt32Number retrievedInputChannels = cmsPipelineInputChannels(pipeline);
    (void)retrievedInputChannels; // Use variable to avoid unused warning

    // Test cmsPipelineOutputChannels
    cmsUInt32Number retrievedOutputChannels = cmsPipelineOutputChannels(pipeline);
    (void)retrievedOutputChannels; // Use variable to avoid unused warning

    // Test cmsPipelineStageCount on empty pipeline
    cmsUInt32Number stageCount = cmsPipelineStageCount(pipeline);
    (void)stageCount; // Use variable to avoid unused warning

    // Test cmsPipelineCheckAndRetreiveStages with empty pipeline
    // Test with 0 expected stages (should return TRUE for empty pipeline)
    cmsBool result = cmsPipelineCheckAndRetreiveStages(pipeline, 0);
    (void)result; // Use variable to avoid unused warning

    // Test with 1 expected stage (should return FALSE for empty pipeline)
    cmsStage *stagePtr = NULL;
    cmsStageSignature stageType = cmsSigMatrixElemType;
    result = cmsPipelineCheckAndRetreiveStages(pipeline, 1, stageType, &stagePtr);
    (void)result; // Use variable to avoid unused warning

    // Test cmsPipelineDup
    cmsPipeline *dupPipeline = cmsPipelineDup(pipeline);
    if (dupPipeline) {
        // Test all functions on duplicated pipeline
        (void)cmsPipelineInputChannels(dupPipeline);
        (void)cmsPipelineOutputChannels(dupPipeline);
        (void)cmsPipelineStageCount(dupPipeline);
        
        // Test cmsPipelineCheckAndRetreiveStages on duplicated pipeline
        result = cmsPipelineCheckAndRetreiveStages(dupPipeline, 0);
        (void)result;
        
        // Clean up duplicated pipeline
        cmsPipelineFree(dupPipeline);
    }

    // Test with different parameters based on fuzzer input
    if (Size >= 16) {
        // Create another pipeline with different channel counts
        cmsUInt32Number inputChannels2 = (Data[2] % 16) + 1;
        cmsUInt32Number outputChannels2 = (Data[3] % 16) + 1;
        
        cmsPipeline *pipeline2 = cmsPipelineAlloc(ctx, inputChannels2, outputChannels2);
        if (pipeline2) {
            // Test functions on second pipeline
            (void)cmsPipelineInputChannels(pipeline2);
            (void)cmsPipelineOutputChannels(pipeline2);
            (void)cmsPipelineStageCount(pipeline2);
            
            // Test duplication of second pipeline
            cmsPipeline *dupPipeline2 = cmsPipelineDup(pipeline2);
            if (dupPipeline2) {
                (void)cmsPipelineInputChannels(dupPipeline2);
                (void)cmsPipelineOutputChannels(dupPipeline2);
                cmsPipelineFree(dupPipeline2);
            }
            
            cmsPipelineFree(pipeline2);
        }
    }

    // Test cmsPipelineCheckAndRetreiveStages with multiple expected stages
    if (Size >= 24) {
        cmsStage *stage1 = NULL;
        cmsStage *stage2 = NULL;
        cmsStageSignature type1 = cmsSigMatrixElemType;
        cmsStageSignature type2 = cmsSigCLutElemType;
        
        result = cmsPipelineCheckAndRetreiveStages(pipeline, 2, type1, &stage1, type2, &stage2);
        (void)result;
    }

    // Clean up
    cmsPipelineFree(pipeline);
    cmsDeleteContext(ctx);

    return 0;
}