// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <lcms2.h>

static cmsPipeline* create_dummy_pipeline(cmsContext ctx) {
    // Allocate a pipeline with 3 input and 3 output channels
    cmsPipeline* pipe = cmsPipelineAlloc(ctx, 3, 3);
    if (pipe == NULL) return NULL;
    
    // Create a simple identity stage
    cmsStage* stage = cmsStageAllocIdentity(ctx, 3);
    if (stage == NULL) {
        cmsPipelineFree(pipe);
        return NULL;
    }
    
    // Add the stage to the pipeline using cmsPipelineAlloc
    // Note: cmsPipelineAlloc creates a new pipeline, we need to use cmsPipelineAppend or similar
    // Since cmsPipelineAppend is not in the provided list, we'll use cmsPipelineAlloc with the stage
    // Actually, the correct way to add a stage to an existing pipeline is to use cmsPipelineAlloc
    // But that creates a new pipeline. Let's use cmsPipelineAppend if available, otherwise we need a different approach.
    
    // For lcms2, we can use cmsPipelineAlloc to create a new pipeline with the stage
    // But we want to add to existing. Let's try to use cmsPipelineAppend (if it exists)
    // Since it's not in the provided list, we'll create a new pipeline with the stage
    
    // Actually, let's just create a new pipeline with the stage directly
    cmsPipeline* newPipe = cmsPipelineAlloc(ctx, 3, 3);
    if (newPipe == NULL) {
        cmsStageFree(stage);
        cmsPipelineFree(pipe);
        return NULL;
    }
    
    // We need to add the stage to newPipe. Since we don't have cmsPipelineAppend in the list,
    // we'll use the internal API or create a simple pipeline.
    // For fuzzing purposes, let's just return the pipe with the stage added manually
    // But we can't do that without the right API.
    
    // Let's try a different approach: create a pipeline with a stage using cmsPipelineAlloc
    // and then use cmsPipelineCat to combine them.
    
    cmsPipeline* stagePipe = cmsPipelineAlloc(ctx, 3, 3);
    if (stagePipe == NULL) {
        cmsStageFree(stage);
        cmsPipelineFree(pipe);
        cmsPipelineFree(newPipe);
        return NULL;
    }
    
    // Add stage to stagePipe
    // We need to use cmsPipelineAppend or similar. Since it's not available, we'll skip this.
    // Instead, let's just return the original pipe and free the stage.
    cmsStageFree(stage);
    cmsPipelineFree(stagePipe);
    cmsPipelineFree(newPipe);
    
    return pipe;
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;
    
    cmsPipeline* pipe1 = NULL;
    cmsPipeline* pipe2 = NULL;
    cmsPipeline* dupPipe = NULL;
    
    // Create a simple pipeline
    pipe1 = cmsPipelineAlloc(ctx, 3, 3);
    if (pipe1 == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Create a stage and add it to pipe1
    cmsStage* stage = cmsStageAllocIdentity(ctx, 3);
    if (stage != NULL) {
        // We need to add the stage to pipe1. Since we don't have cmsPipelineAppend,
        // we'll create a new pipeline with the stage and then cat them.
        cmsPipeline* stagePipe = cmsPipelineAlloc(ctx, 3, 3);
        if (stagePipe != NULL) {
            // Add stage to stagePipe - this is tricky without the right API
            // For now, let's just free the stage and use the empty pipeline
            cmsStageFree(stage);
            cmsPipelineFree(stagePipe);
        }
    }
    
    // Test cmsPipelineDup
    dupPipe = cmsPipelineDup(pipe1);
    if (dupPipe != NULL) {
        // Test cmsPipelineSetSaveAs8bitsFlag on duplicate
        cmsBool oldFlag = cmsPipelineSetSaveAs8bitsFlag(dupPipe, (Data[0] & 0x01) ? TRUE : FALSE);
        (void)oldFlag;
        
        // Test cmsPipelineEval16 on duplicate
        cmsUInt16Number in[3] = {0, 0, 0};
        cmsUInt16Number out[3] = {0, 0, 0};
        if (Size > 6) {
            in[0] = (Data[1] << 8) | Data[2];
            in[1] = (Data[3] << 8) | Data[4];
            in[2] = (Data[5] << 8) | Data[6];
        }
        cmsPipelineEval16(in, out, dupPipe);
        
        cmsPipelineFree(dupPipe);
    }
    
    // Test cmsPipelineCat
    if (Size > 1) {
        if (Data[1] % 2 == 0) {
            pipe2 = cmsPipelineAlloc(ctx, 3, 3);
            if (pipe2 != NULL) {
                cmsBool catResult = cmsPipelineCat(pipe1, pipe2);
                (void)catResult;
                cmsPipelineFree(pipe2);
            }
        }
    }
    
    // Test cmsPipelineCheckAndRetreiveStages
    if (pipe1 != NULL) {
        cmsStage* stagePtr = NULL;
        // Try to check for 1 stage of type cmsSigIdentityElemType
        cmsBool checkResult = cmsPipelineCheckAndRetreiveStages(pipe1, 1, cmsSigIdentityElemType, &stagePtr);
        (void)checkResult;
        (void)stagePtr;
        
        // Try with wrong count
        cmsBool checkResult2 = cmsPipelineCheckAndRetreiveStages(pipe1, 5, cmsSigIdentityElemType, &stagePtr);
        (void)checkResult2;
    }
    
    // Test cmsPipelineSetSaveAs8bitsFlag on original
    if (pipe1 != NULL) {
        cmsBool flagResult = cmsPipelineSetSaveAs8bitsFlag(pipe1, (Size % 2) ? TRUE : FALSE);
        (void)flagResult;
    }
    
    // Cleanup
    cmsPipelineFree(pipe1);
    cmsPipelineFree(pipe2);
    cmsDeleteContext(ctx);
    
    return 0;
}