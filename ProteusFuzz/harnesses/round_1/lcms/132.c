// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_pipeline(cmsPipeline *lut) {
    if (lut != NULL) {
        cmsPipelineFree(lut);
    }
}

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Extract parameters from fuzz data
    cmsUInt32Number inputChannels = Data[0] % 16; // Limit to reasonable range
    cmsUInt32Number outputChannels = (Size > 1) ? (Data[1] % 16) : 0;
    cmsUInt32Number nProfiles = (Size > 2) ? (Data[2] % 5) : 0;
    cmsUInt32Number nStages = (Size > 3) ? (Data[3] % 10) : 0;

    // Test cmsPipelineAlloc
    cmsPipeline *lut = cmsPipelineAlloc(ctx, inputChannels, outputChannels);
    if (lut == NULL) {
        cleanup_context(ctx);
        return 0;
    }

    // Test cmsPipelineInputChannels and cmsPipelineOutputChannels
    cmsUInt32Number inCh = cmsPipelineInputChannels(lut);
    cmsUInt32Number outCh = cmsPipelineOutputChannels(lut);
    (void)inCh;
    (void)outCh;

    // Test cmsPipelineStageCount
    cmsUInt32Number stageCount = cmsPipelineStageCount(lut);
    (void)stageCount;

    // Test cmsPipelineCheckAndRetreiveStages with varying stage counts
    cmsStage *stages[10];
    cmsStageSignature types[10];
    for (int i = 0; i < 10; i++) {
        stages[i] = NULL;
        types[i] = (cmsStageSignature)(i + 1); // Arbitrary types
    }

    // Try with different expected counts
    for (int i = 0; i <= nStages && i <= 10; i++) {
        cmsBool result = cmsPipelineCheckAndRetreiveStages(lut, i, types, stages);
        (void)result;
    }

    // Test _cmsDefaultICCintents
    // Only call if nProfiles is 0 to avoid passing NULL pointers to internal functions
    // that expect valid profiles, or if we can create dummy profiles (complex, so skip for now)
    if (nProfiles == 0) {
        cmsPipeline *defaultLut = _cmsDefaultICCintents(ctx, 0, NULL, NULL, NULL, NULL, 0);
        if (defaultLut != NULL) {
            cmsPipelineFree(defaultLut);
        }
    }

    cleanup_pipeline(lut);
    cleanup_context(ctx);

    return 0;
}