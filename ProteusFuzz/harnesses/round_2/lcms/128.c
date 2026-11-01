// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineEvalReverseFloat at cmslut.c:1753:19 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsPipeline* CreateEmptyPipeline(cmsContext ctx) {
    cmsPipeline *pipe = cmsPipelineAlloc(ctx, 3, 3);
    if (!pipe) return NULL;
    return pipe;
}

static cmsStage* CreateDummyStage(cmsContext ctx) {
    cmsStage *stage = cmsStageAllocIdentity(ctx, 3);
    return stage;
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Create two pipelines to test concatenation
    cmsPipeline *pipe1 = CreateEmptyPipeline(ctx);
    cmsPipeline *pipe2 = CreateEmptyPipeline(ctx);

    if (!pipe1 || !pipe2) {
        if (pipe1) cmsPipelineFree(pipe1);
        if (pipe2) cmsPipelineFree(pipe2);
        cmsDeleteContext(ctx);
        return 0;
    }

    // 1. Test cmsPipelineSetSaveAs8bitsFlag
    cmsBool prevFlag = cmsPipelineSetSaveAs8bitsFlag(pipe1, (Size % 2) ? TRUE : FALSE);
    (void)prevFlag;

    // 2. Test cmsPipelineInsertStage
    cmsStage *stage1 = CreateDummyStage(ctx);
    cmsStage *stage2 = CreateDummyStage(ctx);
    
    if (stage1) {
        // Try inserting at beginning
        cmsPipelineInsertStage(pipe1, cmsAT_BEGIN, stage1);
        // Try inserting at end
        cmsPipelineInsertStage(pipe1, cmsAT_END, stage2);
    }

    // 3. Test cmsPipelineCat
    cmsPipelineCat(pipe1, pipe2);

    // 4. Test cmsPipelineEvalFloat
    cmsFloat32Number inData[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    cmsFloat32Number outData[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    
    // Seed input data from fuzz buffer if available
    if (Size >= 16) {
        memcpy(inData, Data, 16);
    }
    
    if (pipe1) {
        cmsPipelineEvalFloat(inData, outData, pipe1);
    }

    // 5. Test cmsPipelineEvalReverseFloat
    cmsFloat32Number target[3] = {0.5f, 0.5f, 0.5f};
    cmsFloat32Number result[3] = {0.0f, 0.0f, 0.0f};
    cmsFloat32Number hint[3] = {0.3f, 0.3f, 0.3f};
    
    if (Size >= 12) {
        memcpy(target, Data, 12);
    }

    if (pipe1) {
        cmsBool revRes = cmsPipelineEvalReverseFloat(target, result, hint, pipe1);
        (void)revRes;
    }

    // 6. Test cmsPipelineCheckAndRetreiveStages
    // Using 0 expected stages to safely exercise the function logic without needing internal enums
    cmsBool checkRes = cmsPipelineCheckAndRetreiveStages(pipe1, 0);
    (void)checkRes;

    // Cleanup
    if (pipe1) cmsPipelineFree(pipe1);
    if (pipe2) cmsPipelineFree(pipe2);
    cmsDeleteContext(ctx);

    return 0;
}