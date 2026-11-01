// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void cleanup_pipeline(cmsPipeline* p) {
    if (p != NULL) {
        cmsPipelineFree(p);
    }
}

static void cleanup_profiles(cmsHPROFILE* profiles, int count) {
    if (profiles == NULL) return;
    for (int i = 0; i < count; i++) {
        if (profiles[i] != NULL) {
            cmsCloseProfile(profiles[i]);
            profiles[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Extract parameters from fuzz data
    uint32_t in_ch = Data[0] % 10; // Limit to reasonable channel count
    uint32_t out_ch = Data[1] % 10;
    
    // Create a pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, in_ch, out_ch);
    
    if (pipeline != NULL) {
        // Test cmsPipelineInputChannels
        cmsUInt32Number input_ch = cmsPipelineInputChannels(pipeline);
        (void)input_ch; // Use to avoid unused warning

        // Test cmsPipelineOutputChannels
        cmsUInt32Number output_ch = cmsPipelineOutputChannels(pipeline);
        (void)output_ch;

        // Test cmsPipelineStageCount
        cmsUInt32Number stage_count = cmsPipelineStageCount(pipeline);
        (void)stage_count;

        // Test cmsPipelineCheckAndRetreiveStages
        // We need at least one stage to test this, but alloc creates empty.
        // We can try to check for 0 stages.
        cmsStage* stage_ptr = NULL;
        cmsBool result = cmsPipelineCheckAndRetreiveStages(pipeline, 0);
        (void)result;

        // Try with 1 expected stage (will likely fail as pipeline is empty)
        cmsBool result2 = cmsPipelineCheckAndRetreiveStages(pipeline, 1, &stage_ptr);
        (void)result2;

        cleanup_pipeline(pipeline);
    }

    // Test _cmsDefaultICCintents
    // We need valid profiles. Since we cannot easily create valid ICC profiles from raw fuzz data
    // and cmsCreateProfile is not available/declared in the provided context, we will attempt
    // to load a profile from a dummy file. If it fails, we skip this specific test to avoid
    // passing NULL pointers as per requirements.
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (hProfile != NULL) {
        cmsUInt32Number nProfiles = 1;
        cmsUInt32Number intents[1] = {0}; // Perceptual
        cmsHPROFILE hProfiles[1] = {hProfile};
        cmsBool bpc[1] = {FALSE};
        cmsFloat64Number adaptation[1] = {0.0};
        cmsUInt32Number flags = 0;

        cmsPipeline* intent_pipeline = _cmsDefaultICCintents(ctx, nProfiles, intents, hProfiles, bpc, adaptation, flags);
        if (intent_pipeline != NULL) {
            cleanup_pipeline(intent_pipeline);
        }
        
        cmsCloseProfile(hProfile);
    }

    cmsDeleteContext(ctx);
    return 0;
}