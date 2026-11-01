// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_pipelines(cmsPipeline *p1, cmsPipeline *p2, cmsPipeline *p3) {
    if (p1) cmsPipelineFree(p1);
    if (p2) cmsPipelineFree(p2);
    if (p3) cmsPipelineFree(p3);
}

int LLVMFuzzerTestOneInput_194(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsPipeline *p1 = NULL;
    cmsPipeline *p2 = NULL;
    cmsPipeline *p3 = NULL;

    // Create a dummy profile to generate a pipeline
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        unsigned char dummy[128] = {0};
        fwrite(dummy, 1, 128, f);
        fclose(f);
    }

    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (hProfile) {
        // cmsPipelineAlloc takes ContextID, InputChannels, OutputChannels
        // We create a pipeline with 3 input and 3 output channels (RGB)
        cmsPipeline *temp = cmsPipelineAlloc(ctx, 3, 3);
        if (temp) {
            p1 = cmsPipelineDup(temp);
            p2 = cmsPipelineDup(temp);
            cmsPipelineFree(temp);
        }
        cmsCloseProfile(hProfile);
    }

    // Test cmsPipelineDup with NULL
    cmsPipeline *dupNull = cmsPipelineDup(NULL);
    (void)dupNull; // Should be NULL

    // Test cmsPipelineCheckAndRetreiveStages
    cmsStage *s1 = NULL, *s2 = NULL;
    if (p1) {
        // Try to check stages, expecting 0
        cmsBool res = cmsPipelineCheckAndRetreiveStages(p1, 0);
        (void)res;
        
        // Try with 1 stage
        res = cmsPipelineCheckAndRetreiveStages(p1, 1, &s1);
        (void)res;
        
        // Try with 2 stages
        res = cmsPipelineCheckAndRetreiveStages(p1, 2, &s1, &s2);
        (void)res;
    }

    // Test cmsPipelineCat
    if (p1 && p2) {
        cmsBool catRes = cmsPipelineCat(p1, p2);
        if (catRes) {
            // If successful, p1 is modified. p2 is still valid.
            // Try to duplicate the result
            p3 = cmsPipelineDup(p1);
        }
    }

    // Test cmsPipelineSetSaveAs8bitsFlag
    if (p1) {
        cmsBool oldFlag = cmsPipelineSetSaveAs8bitsFlag(p1, TRUE);
        (void)oldFlag;
        oldFlag = cmsPipelineSetSaveAs8bitsFlag(p1, FALSE);
        (void)oldFlag;
    }

    // Test cmsPipelineEval16
    if (p1) {
        cmsUInt16Number in[4] = {0, 0, 0, 0};
        cmsUInt16Number out[4] = {0, 0, 0, 0};
        
        // Use input data from fuzzer if available
        if (Size >= 8) {
            memcpy(in, Data, sizeof(in));
        }
        
        cmsPipelineEval16(in, out, p1);
    }

    // Cleanup
    cleanup_pipelines(p1, p2, p3);
    cmsDeleteContext(ctx);

    return 0;
}