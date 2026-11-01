// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsPipelineGetPtrToLastStage at cmslut.c:1651:21 in lcms2.h
// cmsStageData at cmslut.c:1233:17 in lcms2.h
// cmsStageNext at cmslut.c:1243:22 in lcms2.h
// cmsStageDup at cmslut.c:1250:21 in lcms2.h
// cmsPipelineUnlinkStage at cmslut.c:1555:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
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

static cmsContext create_dummy_context(void) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;
    return ctx;
}

static cmsPipeline* create_dummy_pipeline(cmsContext ctx) {
    if (!ctx) return NULL;
    cmsPipeline* pipe = cmsPipelineAlloc(ctx, 3, 3);
    return pipe;
}

static void cleanup_pipeline(cmsPipeline* pipe) {
    if (pipe) {
        cmsPipelineFree(pipe);
    }
}

static void cleanup_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_206(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = create_dummy_context();
    if (!ctx) return 0;

    cmsPipeline* pipe = create_dummy_pipeline(ctx);
    if (!pipe) {
        cleanup_context(ctx);
        return 0;
    }

    // Fuzzing logic for cmsPipelineGetPtrToFirstStage
    cmsStage* first_stage = cmsPipelineGetPtrToFirstStage(pipe);

    // Fuzzing logic for cmsPipelineGetPtrToLastStage
    cmsStage* last_stage = cmsPipelineGetPtrToLastStage(pipe);

    // Fuzzing logic for cmsStageData
    if (first_stage) {
        void* data = cmsStageData(first_stage);
        (void)data; // Use data to avoid unused variable warning
    }

    // Fuzzing logic for cmsStageNext
    if (first_stage) {
        cmsStage* next_stage = cmsStageNext(first_stage);
        (void)next_stage; // Use next_stage to avoid unused variable warning
    }

    // Fuzzing logic for cmsStageDup
    cmsStage* dup_stage = NULL;
    if (first_stage) {
        dup_stage = cmsStageDup(first_stage);
    }

    // Fuzzing logic for cmsPipelineUnlinkStage
    cmsStage* unlinked_stage = NULL;
    if (pipe) {
        cmsPipelineUnlinkStage(pipe, 0, &unlinked_stage); // 0 represents cmsStageLoc::cmsStageStart
        if (unlinked_stage) {
            cmsStageFree(unlinked_stage);
        }
    }

    // Cleanup duplicated stage if it exists
    if (dup_stage) {
        cmsStageFree(dup_stage);
    }

    // Cleanup pipeline
    cleanup_pipeline(pipe);

    // Cleanup context
    cleanup_context(ctx);

    return 0;
}