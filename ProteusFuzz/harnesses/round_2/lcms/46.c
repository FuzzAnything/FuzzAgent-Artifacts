// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"

static cmsContext CreateContext(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    return cmsCreateContext(NULL, NULL);
}

static void CleanupContext(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsPipeline* CreateEmptyPipeline(cmsContext ctx) {
    return cmsPipelineAlloc(ctx, 3, 3);
}

static void CleanupPipeline(cmsContext ctx, cmsPipeline **p) {
    if (p && *p) {
        cmsPipelineFree(*p);
        *p = NULL;
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = CreateContext(Data, Size);
    if (!ctx) return 0;

    cmsPipeline *p1 = NULL;
    cmsPipeline *p2 = NULL;
    cmsPipeline *p3 = NULL;

    // Initialize pipelines
    p1 = CreateEmptyPipeline(ctx);
    p2 = CreateEmptyPipeline(ctx);
    p3 = CreateEmptyPipeline(ctx);

    if (!p1 || !p2 || !p3) {
        CleanupContext(ctx);
        return 0;
    }

    // Fuzz cmsPipelineSetSaveAs8bitsFlag
    cmsBool flag1 = cmsPipelineSetSaveAs8bitsFlag(p1, (Data[0] & 1) ? TRUE : FALSE);
    (void)flag1;
    cmsBool flag2 = cmsPipelineSetSaveAs8bitsFlag(p2, (Data[1] & 1) ? TRUE : FALSE);
    (void)flag2;

    // Fuzz cmsPipelineDup
    cmsPipeline *dupP1 = cmsPipelineDup(p1);
    cmsPipeline *dupP2 = cmsPipelineDup(p2);
    cmsPipeline *dupP3 = cmsPipelineDup(p3);

    // Fuzz cmsPipelineCat
    // Try concatenating various combinations
    cmsPipelineCat(p1, p2);
    cmsPipelineCat(p1, p3);
    cmsPipelineCat(p2, p1);
    cmsPipelineCat(p3, p2);
    if (dupP1) cmsPipelineCat(p1, dupP1);
    if (dupP2) cmsPipelineCat(p2, dupP2);
    if (dupP3) cmsPipelineCat(p3, dupP3);

    // Fuzz cmsPipelineInsertStage
    // Create a NEW stage for EACH pipeline to avoid sharing ownership.
    // cmsPipelineInsertStage takes ownership of the stage.
    
    cmsStage *s1 = cmsStageAllocIdentity(ctx, 3);
    if (s1) {
        cmsPipelineInsertStage(p1, cmsAT_BEGIN, s1);
    }

    cmsStage *s2 = cmsStageAllocIdentity(ctx, 3);
    if (s2) {
        cmsPipelineInsertStage(p2, cmsAT_BEGIN, s2);
    }

    cmsStage *s3 = cmsStageAllocIdentity(ctx, 3);
    if (s3) {
        cmsPipelineInsertStage(p3, cmsAT_BEGIN, s3);
    }

    // Fuzz cmsPipelineCheckAndRetreiveStages
    cmsStage *outStage1 = NULL, *outStage2 = NULL;
    cmsBool checkRes = cmsPipelineCheckAndRetreiveStages(p1, 0); 
    (void)checkRes;
    // Use cmsSigIdentityElemType instead of cmsSigIdentityStage
    checkRes = cmsPipelineCheckAndRetreiveStages(p1, 1, cmsSigIdentityElemType, &outStage1);
    (void)checkRes;
    checkRes = cmsPipelineCheckAndRetreiveStages(p1, 2, cmsSigIdentityElemType, &outStage1, cmsSigIdentityElemType, &outStage2);
    (void)checkRes;

    // Fuzz cmsPipelineEval16
    cmsUInt16Number in[3] = {0, 0, 0};
    cmsUInt16Number out[3] = {0, 0, 0};
    
    if (Size >= 6) {
        in[0] = Data[2] | (Data[3] << 8);
        in[1] = Data[4] | (Data[5] << 8);
        in[2] = (Size > 6 ? Data[6] : 0) | ((Size > 7 ? Data[7] : 0) << 8);
    }

    cmsPipelineEval16(in, out, p1);
    cmsPipelineEval16(in, out, p2);
    cmsPipelineEval16(in, out, p3);
    if (dupP1) cmsPipelineEval16(in, out, dupP1);
    if (dupP2) cmsPipelineEval16(in, out, dupP2);
    if (dupP3) cmsPipelineEval16(in, out, dupP3);

    // Cleanup
    // The pipelines will free the stages inserted into them.
    CleanupPipeline(ctx, &p1);
    CleanupPipeline(ctx, &p2);
    CleanupPipeline(ctx, &p3);
    CleanupPipeline(ctx, &dupP1);
    CleanupPipeline(ctx, &dupP2);
    CleanupPipeline(ctx, &dupP3);

    CleanupContext(ctx);

    return 0;
}