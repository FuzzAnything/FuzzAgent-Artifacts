// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineEvalReverseFloat at cmslut.c:1753:19 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineEvalReverseFloat at cmslut.c:1753:19 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

// Helper to create a simple pipeline for testing
static cmsPipeline* create_simple_pipeline(cmsContext ctx) {
    // Create a pipeline with 3 input and 3 output channels
    cmsPipeline* p = cmsPipelineAlloc(ctx, 3, 3);
    return p;
}

int LLVMFuzzerTestOneInput_176(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Initialize variables from fuzz input safely
    cmsBool loc = (Data[0] & 0x01) ? cmsAT_BEGIN : cmsAT_END;
    cmsBool on_flag = (Data[0] & 0x02) ? TRUE : FALSE;
    
    cmsFloat32Number target[4] = {0};
    cmsFloat32Number result[4] = {0};
    cmsFloat32Number hint[4] = {0};
    cmsFloat32Number in[4] = {0};
    cmsFloat32Number out[4] = {0};

    // Parse floats from input if available, ensuring bounds checks
    if (Size >= 16) {
        target[0] = *(const cmsFloat32Number*)(Data + 1);
        target[1] = *(const cmsFloat32Number*)(Data + 5);
        target[2] = *(const cmsFloat32Number*)(Data + 9);
        target[3] = *(const cmsFloat32Number*)(Data + 13);
    }
    if (Size >= 32) {
        hint[0] = *(const cmsFloat32Number*)(Data + 17);
        hint[1] = *(const cmsFloat32Number*)(Data + 21);
        hint[2] = *(const cmsFloat32Number*)(Data + 25);
        hint[3] = *(const cmsFloat32Number*)(Data + 29);
    }
    if (Size >= 48) {
        in[0] = *(const cmsFloat32Number*)(Data + 33);
        in[1] = *(const cmsFloat32Number*)(Data + 37);
        in[2] = *(const cmsFloat32Number*)(Data + 41);
        in[3] = *(const cmsFloat32Number*)(Data + 45);
    }

    // Test 1: cmsPipelineInsertStage with NULL stage (expected to return FALSE)
    cmsPipeline* p1 = create_simple_pipeline(ctx);
    if (p1) {
        cmsBool res1 = cmsPipelineInsertStage(p1, loc, NULL);
        (void)res1;
        cmsPipelineFree(p1);
    }

    // Test 2: cmsPipelineCheckAndRetreiveStages
    cmsPipeline* p2 = create_simple_pipeline(ctx);
    if (p2) {
        // Call with n=0 to test minimal case
        cmsBool res2 = cmsPipelineCheckAndRetreiveStages(p2, 0);
        (void)res2;
        cmsPipelineFree(p2);
    }

    // Test 3: cmsPipelineEvalReverseFloat
    cmsPipeline* p3 = create_simple_pipeline(ctx);
    if (p3) {
        cmsBool res3 = cmsPipelineEvalReverseFloat(target, result, hint, p3);
        (void)res3;
        cmsPipelineFree(p3);
    }
    
    // Test with NULL pipeline
    cmsBool res3_null = cmsPipelineEvalReverseFloat(target, result, hint, NULL);
    (void)res3_null;

    // Test 4: cmsPipelineCat
    cmsPipeline* p4a = create_simple_pipeline(ctx);
    cmsPipeline* p4b = create_simple_pipeline(ctx);
    if (p4a && p4b) {
        cmsBool res4 = cmsPipelineCat(p4a, p4b);
        (void)res4;
        cmsPipelineFree(p4a);
        cmsPipelineFree(p4b);
    }
    
    // Test with NULL second argument
    cmsPipeline* p4c = create_simple_pipeline(ctx);
    if (p4c) {
        cmsBool res4_null = cmsPipelineCat(p4c, NULL);
        (void)res4_null;
        cmsPipelineFree(p4c);
    }

    // Test 5: cmsPipelineSetSaveAs8bitsFlag
    cmsPipeline* p5 = create_simple_pipeline(ctx);
    if (p5) {
        cmsBool res5 = cmsPipelineSetSaveAs8bitsFlag(p5, on_flag);
        (void)res5;
        cmsPipelineFree(p5);
    }

    // Test 6: cmsPipelineEvalFloat
    cmsPipeline* p6 = create_simple_pipeline(ctx);
    if (p6) {
        cmsPipelineEvalFloat(in, out, p6);
        cmsPipelineFree(p6);
    }
    
    // Test with NULL pipeline (might assert/crash, valid for fuzzing)
    // cmsPipelineEvalFloat(in, out, NULL);

    // Cleanup
    cmsDeleteContext(ctx);

    return 0;
}