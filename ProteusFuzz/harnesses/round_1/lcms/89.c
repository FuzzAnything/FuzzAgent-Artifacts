// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_pipeline(cmsPipeline* p) {
    if (p != NULL) {
        cmsPipelineFree(p);
    }
}

static cmsPipeline* create_empty_pipeline(cmsContext ctx) {
    return cmsPipelineAlloc(ctx, 0, 0);
}

static cmsStage* create_dummy_stage(cmsContext ctx) {
    return cmsStageAllocIdentity(ctx, 3);
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    unsigned int seed = 0;
    if (Size >= 4) {
        memcpy(&seed, Data, 4);
    }

    cmsPipeline* p1 = create_empty_pipeline(ctx);
    cmsPipeline* p2 = create_empty_pipeline(ctx);
    cmsPipeline* p3 = NULL;
    cmsStage* stage = NULL;

    if (p1 == NULL || p2 == NULL) {
        if (p1) cleanup_pipeline(p1);
        if (p2) cleanup_pipeline(p2);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Insert stages into p1 and p2 based on seed
    if (seed % 3 == 0 && Size > 4) {
        stage = create_dummy_stage(ctx);
        if (stage != NULL) {
            cmsPipelineInsertStage(p1, cmsAT_END, stage);
        }
    }
    if (seed % 3 == 1 && Size > 4) {
        stage = create_dummy_stage(ctx);
        if (stage != NULL) {
            cmsPipelineInsertStage(p2, cmsAT_END, stage);
        }
    }

    // Test cmsPipelineDup and cmsPipelineSetSaveAs8bitsFlag
    if (seed % 5 == 0) {
        p3 = cmsPipelineDup(p1);
        if (p3 != NULL) {
            cmsBool on = (seed % 2) != 0;
            cmsBool prev = cmsPipelineSetSaveAs8bitsFlag(p3, on);
            (void)prev;
            // Do NOT free p3 here, let it be freed at the end
        }
    }

    // Test cmsPipelineCat
    if (seed % 5 == 1) {
        cmsBool res = cmsPipelineCat(p1, p2);
        (void)res;
    }

    // Test cmsPipelineSetSaveAs8bitsFlag on p1
    if (seed % 5 == 2) {
        cmsBool on = (seed % 2) != 0;
        cmsBool prev = cmsPipelineSetSaveAs8bitsFlag(p1, on);
        (void)prev;
    }

    // Test cmsPipelineInsertStage with different locations
    if (seed % 5 == 3 && Size > 4) {
        cmsStage* newStage = create_dummy_stage(ctx);
        if (newStage != NULL) {
            cmsStageLoc loc = (seed % 2 == 0) ? cmsAT_BEGIN : cmsAT_END;
            cmsBool res = cmsPipelineInsertStage(p1, loc, newStage);
            (void)res;
        }
    }

    // Test cmsPipelineCheckAndRetreiveStages
    if (seed % 5 == 4) {
        cmsStage* s1 = NULL;
        cmsStage* s2 = NULL;
        
        cmsBool res0 = cmsPipelineCheckAndRetreiveStages(p1, 0);
        (void)res0;

        cmsBool res1 = cmsPipelineCheckAndRetreiveStages(p1, 1, 0, &s1);
        (void)res1;
        
        cmsBool res2 = cmsPipelineCheckAndRetreiveStages(p1, 2, 0, 0, &s1, &s2);
        (void)res2;
    }

    // Cleanup
    cleanup_pipeline(p1);
    cleanup_pipeline(p2);
    if (p3 != NULL) {
        cleanup_pipeline(p3);
    }
    cmsDeleteContext(ctx);

    return 0;
}