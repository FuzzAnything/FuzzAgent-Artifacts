// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

static void cleanup_stage(cmsStage *stage) {
    if (stage != NULL) {
        cmsStageFree(stage);
    }
}

static void cleanup_pipeline(cmsPipeline *pipeline) {
    if (pipeline != NULL) {
        cmsPipelineFree(pipeline);
    }
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Create a dummy pipeline to work with
    cmsPipeline *pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Test cmsPipelineGetPtrToFirstStage
    cmsStage *first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
    if (first_stage != NULL) {
        cmsStage *current = first_stage;
        while (current != NULL) {
            // Test cmsStageData
            void *data = cmsStageData(current);
            (void)data; // Use the data pointer to avoid unused variable warning

            // Test cmsStageDup
            cmsStage *dup_stage = cmsStageDup(current);
            if (dup_stage != NULL) {
                cleanup_stage(dup_stage);
            }

            // Move to next stage
            current = cmsStageNext(current);
        }
    }

    // Test cmsPipelineGetPtrToLastStage
    cmsStage *last_stage = cmsPipelineGetPtrToLastStage(pipeline);
    if (last_stage != NULL) {
        // Test cmsStageData on last stage
        void *last_data = cmsStageData(last_stage);
        (void)last_data;

        // Test cmsStageDup on last stage
        cmsStage *dup_last = cmsStageDup(last_stage);
        if (dup_last != NULL) {
            cleanup_stage(dup_last);
        }
    }

    // Cleanup
    cleanup_pipeline(pipeline);
    cmsDeleteContext(ctx);

    return 0;
}