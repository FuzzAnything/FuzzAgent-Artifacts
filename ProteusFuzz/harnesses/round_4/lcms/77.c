// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_pipeline(cmsPipeline* pipeline) {
    if (pipeline != NULL) {
        cmsPipelineFree(pipeline);
    }
}

static int fuzz_pipeline_api(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsPipeline *pipeline = NULL;
    cmsPipeline *default_pipeline = NULL;
    cmsUInt32Number input_ch = 0;
    cmsUInt32Number output_ch = 0;
    cmsUInt32Number stage_count = 0;
    cmsBool result = FALSE;
    cmsStage *stages[10];
    cmsStageSignature types[10];

    // Ensure minimum size for basic operations
    if (Size < 2) {
        cmsDeleteContext(ctx);
        return 0;
    }

    input_ch = Data[0] % 10; // 0-9
    output_ch = Data[1] % 10; // 0-9

    // 1. Test cmsPipelineAlloc
    if (input_ch > 0 && output_ch > 0) {
        pipeline = cmsPipelineAlloc(ctx, input_ch, output_ch);
    }

    // 2. Test cmsPipelineInputChannels
    if (pipeline != NULL) {
        cmsUInt32Number in_ch = cmsPipelineInputChannels(pipeline);
        (void)in_ch; // Use value to avoid unused warning
    } else {
        // Intentionally passing NULL to trigger assertion in debug builds if applicable
        cmsUInt32Number in_ch_null = cmsPipelineInputChannels(NULL);
        (void)in_ch_null;
    }

    // 3. Test cmsPipelineOutputChannels
    if (pipeline != NULL) {
        cmsUInt32Number out_ch = cmsPipelineOutputChannels(pipeline);
        (void)out_ch;
    } else {
        cmsUInt32Number out_ch_null = cmsPipelineOutputChannels(NULL);
        (void)out_ch_null;
    }

    // 4. Test cmsPipelineStageCount
    if (pipeline != NULL) {
        stage_count = cmsPipelineStageCount(pipeline);
    } else {
        stage_count = cmsPipelineStageCount(NULL);
    }

    // 5. Test _cmsDefaultICCintents
    cmsUInt32Number nProfiles = 0;
    cmsUInt32Number intents[1] = {0};
    cmsHPROFILE hProfiles[1] = {NULL};
    cmsBool bpc[1] = {FALSE};
    cmsFloat64Number adaptation[1] = {0.0};
    cmsUInt32Number flags = 0;

    if (Size > 2) {
        nProfiles = Data[2] % 2; // 0 or 1
        if (nProfiles > 0) {
            intents[0] = Data[3] % 4; // 0-3 intents
            flags = Data[4] % 100;
        }
    }

    // Attempt to create a default pipeline
    default_pipeline = _cmsDefaultICCintents(ctx, nProfiles, intents, hProfiles, bpc, adaptation, flags);

    if (default_pipeline != NULL) {
        cmsUInt32Number def_in = cmsPipelineInputChannels(default_pipeline);
        cmsUInt32Number def_out = cmsPipelineOutputChannels(default_pipeline);
        cmsUInt32Number def_stages = cmsPipelineStageCount(default_pipeline);
        (void)def_in; (void)def_out; (void)def_stages;
    }

    // 6. Test cmsPipelineCheckAndRetreiveStages
    cmsPipeline *target_pipeline = (pipeline != NULL) ? pipeline : default_pipeline;
    
    if (target_pipeline != NULL) {
        cmsUInt32Number expected_n = 0;
        if (Size > 5) expected_n = Data[5] % 5; // 0 to 4 stages expected

        if (expected_n > 0) {
            // Prepare types and stage pointers
            for (uint32_t i = 0; i < expected_n; i++) {
                types[i] = (cmsStageSignature)(Data[6 + i] % 100);
                stages[i] = NULL;
            }

            if (expected_n == 1) {
                result = cmsPipelineCheckAndRetreiveStages(target_pipeline, 1, types[0], &stages[0]);
            } else if (expected_n == 2) {
                result = cmsPipelineCheckAndRetreiveStages(target_pipeline, 2, types[0], types[1], &stages[0], &stages[1]);
            } else if (expected_n == 3) {
                result = cmsPipelineCheckAndRetreiveStages(target_pipeline, 3, types[0], types[1], types[2], &stages[0], &stages[1], &stages[2]);
            } else if (expected_n == 4) {
                result = cmsPipelineCheckAndRetreiveStages(target_pipeline, 4, types[0], types[1], types[2], types[3], &stages[0], &stages[1], &stages[2], &stages[3]);
            } else {
                result = cmsPipelineCheckAndRetreiveStages(target_pipeline, 0);
            }
        } else {
            result = cmsPipelineCheckAndRetreiveStages(target_pipeline, 0);
        }
        
        (void)result;
    } else {
        result = cmsPipelineCheckAndRetreiveStages(NULL, 0);
        (void)result;
    }

    // Cleanup
    cleanup_pipeline(pipeline);
    cleanup_pipeline(default_pipeline);
    cmsDeleteContext(ctx);

    return 0;
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    return fuzz_pipeline_api(Data, Size);
}