// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void safe_free_stage(cmsStage* stage) {
    if (stage != NULL) {
        cmsStageFree(stage);
    }
}

static void safe_free_pipeline(cmsPipeline* pipeline) {
    if (pipeline != NULL) {
        cmsPipelineFree(pipeline);
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Create a dummy pipeline to get stages from
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Create a simple identity stage to attach to the pipeline
    cmsStage* stage = cmsStageAllocIdentity(ctx, 3);
    if (stage == NULL) {
        safe_free_pipeline(pipeline);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Attach stage to pipeline using cmsPipelineAllocStage and cmsPipelineAppendStage
    // Since cmsPipelineAppendStage is not available in the provided API list, 
    // we will manually manage the pipeline stages by allocating and linking them.
    // However, for simplicity and to avoid complex internal structure manipulation,
    // we will just use the stage directly without attaching it to the pipeline.

    // 1. Test cmsPipelineGetPtrToFirstStage (will return NULL as no stages are attached)
    cmsStage* first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
    if (first_stage != NULL) {
        // 2. Test cmsStageData
        void* data = cmsStageData(first_stage);
        (void)data; // Use data to avoid unused variable warning

        // 3. Test cmsStageDup
        cmsStage* dup_stage = cmsStageDup(first_stage);
        if (dup_stage != NULL) {
            // 4. Test cmsStageNext on the duplicated stage (should be NULL as it's a single stage)
            cmsStage* next_stage = cmsStageNext(dup_stage);
            (void)next_stage;

            // 5. Test cmsStageFree on the duplicated stage
            safe_free_stage(dup_stage);
        }

        // 6. Test cmsStageNext on the original first stage
        cmsStage* next_first = cmsStageNext(first_stage);
        (void)next_first;
    }

    // 7. Test cmsPipelineGetPtrToLastStage (will return NULL as no stages are attached)
    cmsStage* last_stage = cmsPipelineGetPtrToLastStage(pipeline);
    if (last_stage != NULL) {
        // Test cmsStageData on last stage
        void* last_data = cmsStageData(last_stage);
        (void)last_data;

        // Test cmsStageDup on last stage
        cmsStage* dup_last = cmsStageDup(last_stage);
        if (dup_last != NULL) {
            safe_free_stage(dup_last);
        }

        // Test cmsStageNext on last stage
        cmsStage* next_last = cmsStageNext(last_stage);
        (void)next_last;
    }

    // Test cmsStageData, cmsStageDup, cmsStageNext, and cmsStageFree on the standalone stage
    if (stage != NULL) {
        void* stage_data = cmsStageData(stage);
        (void)stage_data;

        cmsStage* dup_stage = cmsStageDup(stage);
        if (dup_stage != NULL) {
            cmsStage* next_stage = cmsStageNext(dup_stage);
            (void)next_stage;
            safe_free_stage(dup_stage);
        }

        cmsStage* next_stage = cmsStageNext(stage);
        (void)next_stage;

        safe_free_stage(stage);
    }

    // Cleanup
    safe_free_pipeline(pipeline);
    cmsDeleteContext(ctx);

    return 0;
}