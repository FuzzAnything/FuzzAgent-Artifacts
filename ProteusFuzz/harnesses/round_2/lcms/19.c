// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineStageCount at cmslut.c:1661:27 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void cleanup_pipeline(cmsPipeline* pipeline) {
    if (pipeline != NULL) {
        cmsPipelineFree(pipeline);
    }
}

static int fuzz_pipeline_queries(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsUInt32Number input_ch = 0;
    cmsUInt32Number output_ch = 0;
    cmsPipeline* pipeline = NULL;

    // Extract channel counts from input data if available
    if (Size >= 8) {
        input_ch = *(const cmsUInt32Number*)(Data);
        output_ch = *(const cmsUInt32Number*)(Data + 4);
    } else if (Size >= 4) {
        input_ch = *(const cmsUInt32Number*)(Data);
        output_ch = input_ch;
    } else {
        input_ch = 1;
        output_ch = 1;
    }

    // Clamp to reasonable limits to avoid allocation failures or asserts
    if (input_ch > cmsMAXCHANNELS) input_ch = cmsMAXCHANNELS;
    if (output_ch > cmsMAXCHANNELS) output_ch = cmsMAXCHANNELS;

    // Test cmsPipelineAlloc
    pipeline = cmsPipelineAlloc(ctx, input_ch, output_ch);
    if (pipeline != NULL) {
        // Test query functions on valid pipeline
        cmsUInt32Number in_ch = cmsPipelineInputChannels(pipeline);
        cmsUInt32Number out_ch = cmsPipelineOutputChannels(pipeline);
        cmsUInt32Number stage_count = cmsPipelineStageCount(pipeline);
        
        // Test cmsPipelineCheckAndRetreiveStages with empty list (0 stages expected)
        cmsBool match = cmsPipelineCheckAndRetreiveStages(pipeline, 0);
        
        cleanup_pipeline(pipeline);
        pipeline = NULL;
    }

    cmsDeleteContext(ctx);
    return 0;
}

static int fuzz_default_intents(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsPipeline* pipeline = NULL;
    cmsUInt32Number nProfiles = 0;
    cmsUInt32Number *Intents = NULL;
    cmsHPROFILE *hProfiles = NULL;
    cmsBool *BPC = NULL;
    cmsFloat64Number *AdaptationStates = NULL;
    cmsUInt32Number dwFlags = 0;

    if (Size >= 4) {
        nProfiles = *(const cmsUInt32Number*)(Data);
        if (nProfiles > 10) nProfiles = 10; // Limit for safety
    }

    // If nProfiles is 0, _cmsDefaultICCintents might behave unexpectedly or return NULL.
    // We only proceed if we can provide at least one profile handle, but since we don't have real profiles,
    // we will skip calling _cmsDefaultICCintents with NULL profiles to avoid the SEGV.
    // The function expects valid cmsHPROFILE handles. Passing NULLs leads to the crash.
    // To fuzz this safely without real profiles, we must either:
    // 1. Create dummy profiles (complex, requires more lcms API calls).
    // 2. Skip this specific path if we can't provide valid profiles.
    // Given the constraint, we skip the call if we can't provide valid profiles.
    // However, to maximize coverage, we can try to create a dummy profile if possible, 
    // but creating a valid ICC profile from scratch is non-trivial in a fuzzer.
    // Instead, we will simply not call _cmsDefaultICCintents with NULL profiles.
    // We can still test the memory allocation and setup logic.
    
    if (nProfiles > 0) {
        Intents = (cmsUInt32Number*)malloc(nProfiles * sizeof(cmsUInt32Number));
        hProfiles = (cmsHPROFILE*)malloc(nProfiles * sizeof(cmsHPROFILE));
        BPC = (cmsBool*)malloc(nProfiles * sizeof(cmsBool));
        AdaptationStates = (cmsFloat64Number*)malloc(nProfiles * sizeof(cmsFloat64Number));

        if (!Intents || !hProfiles || !BPC || !AdaptationStates) {
            free(Intents); free(hProfiles); free(BPC); free(AdaptationStates);
            cmsDeleteContext(ctx);
            return 0;
        }

        // Initialize with dummy values
        for (int i = 0; i < (int)nProfiles; i++) {
            Intents[i] = (Size > 0) ? (Data[i % Size] % 4) : 0; // 0-3 intents
            hProfiles[i] = NULL; // Cannot provide valid profiles in this simple fuzzer
            BPC[i] = (Size > 0) ? (Data[(i + 1) % Size] & 1) : 0;
            AdaptationStates[i] = 0.0;
        }

        dwFlags = (Size > 0) ? *(const cmsUInt32Number*)(Data + nProfiles * sizeof(cmsUInt32Number)) : 0;
        dwFlags &= 0xFF; 

        // DO NOT call _cmsDefaultICCintents with NULL profiles as it causes SEGV.
        // We skip this call to prevent the crash.
        // pipeline = _cmsDefaultICCintents(ctx, nProfiles, Intents, hProfiles, BPC, AdaptationStates, dwFlags);
        
        // Instead, we can just free the allocated memory and return.
        // This part of the code is effectively skipped to maintain stability.
        
        free(Intents);
        free(hProfiles);
        free(BPC);
        free(AdaptationStates);
    }

    cmsDeleteContext(ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Dispatch to different fuzzing scenarios based on input
    if (Data[0] & 0x01) {
        fuzz_pipeline_queries(Data, Size);
    }
    // We skip fuzz_default_intents because it requires valid profiles to avoid SEGV.
    // if (Data[0] & 0x02) {
    //     fuzz_default_intents(Data, Size);
    // }

    return 0;
}