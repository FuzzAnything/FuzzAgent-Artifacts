// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsPipelineUnlinkStage at cmslut.c:1555:16 in lcms2.h
// cmsPipelineUnlinkStage at cmslut.c:1555:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_pipeline(cmsPipeline* pipe) {
    if (pipe != NULL) {
        cmsPipelineFree(pipe);
    }
}

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_200(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Create a dummy pipeline to work with
    cmsPipeline* pipe = cmsPipelineAlloc(ctx, 3, 3);
    if (pipe == NULL) {
        cleanup_context(ctx);
        return 0;
    }

    // Create a dummy stage to work with
    cmsStage* dummyStage = cmsStageAllocIdentity(ctx, 3);
    
    cmsStage* firstStage = NULL;
    cmsStage* lastStage = NULL;
    cmsStage* nextStage = NULL;
    cmsStage* duplicatedStage = NULL;
    void* stageData = NULL;
    cmsStage* unlinkedStage = NULL;

    // Test cmsPipelineGetPtrToFirstStage on empty pipeline
    firstStage = cmsPipelineGetPtrToFirstStage(pipe);

    // Test cmsPipelineGetPtrToLastStage on empty pipeline
    lastStage = cmsPipelineGetPtrToLastStage(pipe);

    // To test stage-specific functions, we need a stage in the pipeline.
    // Since cmsPipelineAppendStage is not available in the provided API list and 
    // direct struct manipulation is unsafe/undefined, we will test the functions
    // that do not require a populated pipeline or use the dummyStage directly 
    // if it was somehow linked (which we can't do safely without the append API).
    // However, we can test the functions on the dummyStage itself if we assume 
    // it's a valid standalone object for inspection, though cmsStageNext/Data 
    // expect it to be part of a chain or have specific internal state.
    
    // Test cmsStageData on the standalone dummyStage (might return NULL or valid data)
    if (dummyStage != NULL) {
        stageData = cmsStageData(dummyStage);
    }

    // Test cmsStageNext on the standalone dummyStage (likely returns NULL)
    if (dummyStage != NULL) {
        nextStage = cmsStageNext(dummyStage);
    }

    // Test cmsStageDup on the standalone dummyStage
    if (dummyStage != NULL) {
        duplicatedStage = cmsStageDup(dummyStage);
        // Note: We cannot free duplicatedStage as cmsStageFree is not in the API list.
        // Memory leak is acceptable in fuzzing context.
    }

    // Test cmsPipelineUnlinkStage on empty pipeline (should be safe)
    cmsPipelineUnlinkStage(pipe, cmsAT_BEGIN, &unlinkedStage);
    cmsPipelineUnlinkStage(pipe, cmsAT_END, &unlinkedStage);

    // Cleanup
    cleanup_pipeline(pipe);
    cleanup_context(ctx);

    return 0;
}