// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsPipeline* create_test_pipeline(cmsContext ctx, cmsUInt32Number in_channels, cmsUInt32Number out_channels) {
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, in_channels, out_channels);
    if (!pipeline) return NULL;
    
    // Add a simple matrix stage if possible
    if (in_channels == out_channels && in_channels == 3) {
        float matrix[9] = {1.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 1.0f};
        cmsStage* stage = cmsStageAllocMatrix(ctx, 3, 3, matrix, NULL);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        }
    }
    
    return pipeline;
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a simple sRGB profile as dummy
    return cmsCreate_sRGBProfileTHR(ctx);
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Use first bytes to determine parameters
    size_t offset = 0;
    cmsUInt32Number seed = 0;
    if (Size >= sizeof(seed)) {
        memcpy(&seed, Data, sizeof(seed));
        offset += sizeof(seed);
    }
    
    // Create test pipelines
    cmsUInt32Number in_channels = (seed % 8) + 1;
    cmsUInt32Number out_channels = ((seed >> 8) % 8) + 1;
    
    cmsPipeline* pipeline1 = create_test_pipeline(ctx, in_channels, out_channels);
    cmsPipeline* pipeline2 = create_test_pipeline(ctx, out_channels, in_channels);
    
    if (pipeline1 && pipeline2) {
        // Test cmsPipelineCat
        cmsBool cat_result = cmsPipelineCat(pipeline1, pipeline2);
        (void)cat_result;
        
        // Test cmsPipelineDup
        cmsPipeline* dup_pipeline = cmsPipelineDup(pipeline1);
        if (dup_pipeline) {
            cmsPipelineFree(dup_pipeline);
        }
        
        // Test channel count functions
        cmsUInt32Number input_chans = cmsPipelineInputChannels(pipeline1);
        cmsUInt32Number output_chans = cmsPipelineOutputChannels(pipeline1);
        (void)input_chans;
        (void)output_chans;
        
        // Test cmsPipelineCheckAndRetreiveStages
        if (in_channels == 3 && out_channels == 3) {
            cmsStage* stage = NULL;
            cmsPipelineCheckAndRetreiveStages(pipeline1, 1, cmsSigMatrixElemType, &stage);
        }
    }
    
    // Test _cmsDefaultICCintents
    if (Size - offset >= 16) {
        cmsUInt32Number nProfiles = 2;
        cmsUInt32Number Intents[2] = {INTENT_PERCEPTUAL, INTENT_RELATIVE_COLORIMETRIC};
        cmsBool BPC[2] = {FALSE, FALSE};
        cmsFloat64Number AdaptationStates[2] = {1.0, 1.0};
        cmsUInt32Number dwFlags = 0;
        
        cmsHPROFILE hProfiles[2];
        hProfiles[0] = create_dummy_profile(ctx);
        hProfiles[1] = create_dummy_profile(ctx);
        
        if (hProfiles[0] && hProfiles[1]) {
            cmsPipeline* default_pipeline = _cmsDefaultICCintents(ctx, nProfiles, Intents, hProfiles, BPC, AdaptationStates, dwFlags);
            if (default_pipeline) {
                cmsPipelineFree(default_pipeline);
            }
        }
        
        if (hProfiles[0]) cmsCloseProfile(hProfiles[0]);
        if (hProfiles[1]) cmsCloseProfile(hProfiles[1]);
    }
    
    // Cleanup
    if (pipeline1) cmsPipelineFree(pipeline1);
    if (pipeline2) cmsPipelineFree(pipeline2);
    
    cmsDeleteContext(ctx);
    return 0;
}