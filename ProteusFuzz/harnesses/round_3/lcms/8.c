// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsContext CreateContext(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return NULL;
    
    // Try to use fuzz data to create a profile if possible, though we mainly need the context
    // For pipeline operations, we often need stages. We can try to create a simple pipeline.
    return ctx;
}

static void CleanupContext(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsPipeline* CreateSimplePipeline(cmsContext ctx, const uint8_t *Data, size_t Size) {
    cmsPipeline* p = NULL;
    cmsStage* s = NULL;
    
    // Try to create a simple stage if we have enough data
    if (Size > 0) {
        // Create a dummy stage (e.g., a matrix stage with minimal data)
        // We use cmsStageCreateMatrix with a small matrix to avoid complex data parsing
        // However, cmsStageCreateMatrix requires a specific size. Let's try a 1x1 matrix for simplicity if possible
        // But lcms usually expects at least 3x3 or similar. 
        // Instead, let's try to create a stage from a small amount of data if we can, 
        // or just create a pipeline with no stages (empty) which is valid.
        
        // Attempt to create a pipeline with no stages first (valid)
        p = cmsPipelineAlloc(ctx, 3, 3); // 3 in, 3 out
        if (p == NULL) return NULL;
        
        // If we have data, try to create a stage. 
        // Since parsing arbitrary data into a stage is hard without a profile, 
        // we will mostly rely on empty pipelines or simple duplications.
        // We can try to create a stage using cmsStageCreateIdentity if available, 
        // but that's not in the provided API list. 
        // We will stick to creating empty pipelines and manipulating them.
    } else {
        p = cmsPipelineAlloc(ctx, 3, 3);
    }
    
    return p;
}

static void CleanupPipeline(cmsPipeline* p) {
    if (p) {
        cmsPipelineFree(p);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = CreateContext(Data, Size);
    if (ctx == NULL) return 0;

    // Create initial pipelines
    cmsPipeline *p1 = CreateSimplePipeline(ctx, Data, Size);
    cmsPipeline *p2 = CreateSimplePipeline(ctx, Data, Size);
    
    if (p1 == NULL && p2 == NULL) {
        CleanupContext(ctx);
        return 0;
    }

    // Fuzz cmsPipelineDup
    cmsPipeline *pDup = NULL;
    if (p1) {
        pDup = cmsPipelineDup(p1);
        if (pDup) {
            // Free the duplicate immediately to test cleanup
            cmsPipelineFree(pDup);
        }
    }

    // Fuzz cmsPipelineCat
    if (p1 && p2) {
        cmsPipeline *pCat = cmsPipelineDup(p1); // Create a copy to modify
        if (pCat) {
            cmsPipelineCat(pCat, p2);
            cmsPipelineFree(pCat);
        }
    }

    // Fuzz cmsPipelineCheckAndRetreiveStages
    cmsStage *s1 = NULL, *s2 = NULL;
    if (p1) {
        // Try with 0 stages expected
        cmsPipelineCheckAndRetreiveStages(p1, 0);
        // Try with 1 stage expected (likely to fail if empty, but tests the function)
        cmsPipelineCheckAndRetreiveStages(p1, 1, &s1);
        // Try with 2 stages
        cmsPipelineCheckAndRetreiveStages(p1, 2, &s1, &s2);
    }

    // Fuzz cmsPipelineEval16
    cmsUInt16Number in[3] = {0, 0, 0};
    cmsUInt16Number out[3] = {0, 0, 0};
    if (p1) {
        // Use fuzz data to seed input if available
        if (Size >= 6) {
            in[0] = (Data[0] << 8) | Data[1];
            in[1] = (Data[2] << 8) | Data[3];
            in[2] = (Data[4] << 8) | Data[5];
        }
        cmsPipelineEval16(in, out, p1);
    }

    // Fuzz cmsPipelineSetSaveAs8bitsFlag
    if (p1) {
        cmsBool prev = cmsPipelineSetSaveAs8bitsFlag(p1, (Size % 2) != 0);
        (void)prev; // Suppress unused warning
        cmsPipelineSetSaveAs8bitsFlag(p1, (Size % 3) != 0);
    }

    // Cleanup
    CleanupPipeline(p1);
    CleanupPipeline(p2);
    CleanupContext(ctx);

    return 0;
}