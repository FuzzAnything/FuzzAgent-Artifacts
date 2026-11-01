// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_pipeline(cmsPipeline* pipeline) {
    if (pipeline != NULL) {
        cmsPipelineFree(pipeline);
    }
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    // Extract parameters from input data
    cmsUInt32Number InputChannels = 0;
    cmsUInt32Number OutputChannels = 0;
    cmsUInt32Number nProfiles = 0;
    cmsUInt32Number nStages = 0;

    if (Size >= 4) {
        InputChannels = *(const cmsUInt32Number*)(Data);
        if (Size >= 8) {
            OutputChannels = *(const cmsUInt32Number*)(Data + 4);
            if (Size >= 12) {
                nProfiles = *(const cmsUInt32Number*)(Data + 8);
                if (Size >= 16) {
                    nStages = *(const cmsUInt32Number*)(Data + 12);
                }
            }
        }
    }

    // Validate channel counts to avoid invalid allocations
    if (InputChannels > 16 || OutputChannels > 16) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Test cmsPipelineAlloc
    cmsPipeline* pipeline = cmsPipelineAlloc(ContextID, InputChannels, OutputChannels);
    if (pipeline != NULL) {
        // Test cmsPipelineInputChannels
        cmsUInt32Number inCh = cmsPipelineInputChannels(pipeline);
        (void)inCh; // Suppress unused variable warning

        // Test cmsPipelineOutputChannels
        cmsUInt32Number outCh = cmsPipelineOutputChannels(pipeline);
        (void)outCh; // Suppress unused variable warning

        // Test cmsPipelineStageCount
        cmsUInt32Number stageCount = cmsPipelineStageCount(pipeline);
        (void)stageCount; // Suppress unused variable warning

        // Test cmsPipelineCheckAndRetreiveStages with varying stage counts
        if (stageCount > 0) {
            cmsStage* stagePtrs[10];
            cmsUInt32Number types[10];
            cmsUInt32Number testCount = (stageCount < 10) ? stageCount : 10;
            
            // Initialize types with dummy values
            for (cmsUInt32Number i = 0; i < testCount; i++) {
                types[i] = (cmsUInt32Number)(i + 1);
                stagePtrs[i] = NULL;
            }

            // Manually construct the varargs call for cmsPipelineCheckAndRetreiveStages
            // We pass 'testCount', then 'testCount' types, then 'testCount' pointers
            cmsBool result = cmsPipelineCheckAndRetreiveStages(pipeline, testCount, 
                types[0], types[1], types[2], types[3], types[4], types[5], types[6], types[7], types[8], types[9],
                &stagePtrs[0], &stagePtrs[1], &stagePtrs[2], &stagePtrs[3], &stagePtrs[4], 
                &stagePtrs[5], &stagePtrs[6], &stagePtrs[7], &stagePtrs[8], &stagePtrs[9]);
            (void)result; // Suppress unused variable warning
        }

        cleanup_pipeline(pipeline);
    }

    // Test _cmsDefaultICCintents
    // FIX: _cmsDefaultICCintents expects valid profile handles if nProfiles > 0.
    // Passing NULL handles causes a crash in cmsGetColorSpace.
    // We skip this test if we don't have valid profiles to pass.
    // In a real fuzzing scenario, one would need to create dummy profiles or skip this API call
    // when valid profiles are not available. Here we simply skip to prevent the crash.
    if (nProfiles > 0 && Size >= 20) {
        // Since we cannot easily create valid ICC profiles from raw fuzz data without complex parsing,
        // and passing NULL causes a crash in the library, we skip this specific test case.
        // Alternatively, one could implement a check to ensure hProfiles are valid before calling.
        // For the purpose of fixing the crash in the driver:
        // We do not call _cmsDefaultICCintents with NULL profiles.
    }

    cmsDeleteContext(ContextID);
    return 0;
}