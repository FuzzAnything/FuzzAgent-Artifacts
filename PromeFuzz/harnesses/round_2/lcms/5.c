// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_dummy_profile(const uint8_t* data, size_t size) {
    if (size == 0) return NULL;
    
    // Write data to a temporary file as some profile functions may need it
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return NULL;
    fwrite(data, 1, size, f);
    fclose(f);
    
    // Try to open profile from memory first, then from file
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
        hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    }
    return hProfile;
}

static cmsPipeline* create_dummy_pipeline(const uint8_t* data, size_t size) {
    if (size < 4) return NULL;
    
    // Use first bytes to determine channel counts
    cmsUInt32Number inputChannels = (data[0] % 16) + 1;  // 1-16 channels
    cmsUInt32Number outputChannels = (data[1] % 16) + 1; // 1-16 channels
    
    cmsPipeline* pipeline = cmsPipelineAlloc(NULL, inputChannels, outputChannels);
    if (!pipeline) return NULL;
    
    // Try to add a simple stage if we have enough data
    if (size >= 8 && (data[2] % 2)) {
        cmsStage* stage = cmsStageAllocIdentity(NULL, inputChannels);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        }
    }
    
    return pipeline;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create some dummy pipelines for testing cmsPipelineFree
    cmsPipeline* pipelines[12];
    for (int i = 0; i < 12; i++) {
        size_t offset = (i * 10) % Size;
        size_t chunk_size = Size - offset;
        if (chunk_size > 100) chunk_size = 100;
        
        pipelines[i] = create_dummy_pipeline(Data + offset, chunk_size);
    }
    
    // Call cmsPipelineFree 12 times as specified
    for (int i = 0; i < 12; i++) {
        cmsPipelineFree(pipelines[i]);
    }
    
    // Create profiles for black point detection
    cmsHPROFILE profiles[4];
    cmsCIEXYZ blackPoints[4];
    
    for (int i = 0; i < 4; i++) {
        size_t offset = (i * 50) % Size;
        size_t chunk_size = Size - offset;
        if (chunk_size > 200) chunk_size = 200;
        
        profiles[i] = create_dummy_profile(Data + offset, chunk_size);
        
        // Call cmsDetectDestinationBlackPoint 4 times
        if (profiles[i]) {
            cmsUInt32Number intent = (Data[offset] % 4);  // 0-3 for different intents
            cmsUInt32Number flags = (Data[offset + 1] % 256);
            
            cmsDetectDestinationBlackPoint(&blackPoints[i], profiles[i], intent, flags);
        }
    }
    
    // Call cmsDetectTAC on the first valid profile
    for (int i = 0; i < 4; i++) {
        if (profiles[i]) {
            cmsDetectTAC(profiles[i]);
            break;
        }
    }
    
    // Clean up profiles
    for (int i = 0; i < 4; i++) {
        if (profiles[i]) {
            cmsCloseProfile(profiles[i]);
        }
    }
    
    // Clean up temporary file
    remove("./dummy_file");
    
    return 0;
}