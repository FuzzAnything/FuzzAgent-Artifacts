// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetLogErrorHandler at cmserr.c:503:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsGetPipelineContextID at cmslut.c:1407:22 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_log_error_handler(cmsContext context, cmsUInt32Number error_code, const char* text) {
    // Do nothing - suppress error output during fuzzing
    (void)context;
    (void)error_code;
    (void)text;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0; // Need minimum data for parameters
    
    // Extract parameters from fuzzed data
    size_t offset = 0;
    
    // For cmsCreateContext - use dummy plugin and user data
    void* plugin = NULL;
    void* user_data = NULL;
    
    // Create context
    cmsContext context = cmsCreateContext(plugin, user_data);
    if (!context) return 0;
    
    // Set error handler to suppress output
    cmsSetLogErrorHandler(dummy_log_error_handler);
    
    // Extract parameters for cmsStageAllocCLutFloat
    cmsUInt32Number nGridPoints = 2; // Default small value
    cmsUInt32Number inputChan = 1;
    cmsUInt32Number outputChan = 1;
    
    if (Size - offset >= 12) {
        nGridPoints = *(cmsUInt32Number*)(Data + offset);
        offset += 4;
        inputChan = *(cmsUInt32Number*)(Data + offset);
        offset += 4;
        outputChan = *(cmsUInt32Number*)(Data + offset);
        offset += 4;
        
        // Limit values to reasonable ranges to avoid excessive memory allocation
        if (nGridPoints < 2) nGridPoints = 2;
        if (nGridPoints > 16) nGridPoints = 16;
        if (inputChan < 1) inputChan = 1;
        if (inputChan > 8) inputChan = 8;
        if (outputChan < 1) outputChan = 1;
        if (outputChan > 8) outputChan = 8;
    }
    
    // Calculate table size
    size_t table_size = 1;
    for (cmsUInt32Number i = 0; i < inputChan; i++) {
        table_size *= nGridPoints;
    }
    table_size *= outputChan;
    
    // Allocate and fill table with fuzzed data
    cmsFloat32Number* table = (cmsFloat32Number*)malloc(table_size * sizeof(cmsFloat32Number));
    if (!table) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Fill table with fuzzed data, using 0.0 if we run out
    for (size_t i = 0; i < table_size; i++) {
        if (offset + 4 <= Size) {
            table[i] = *(cmsFloat32Number*)(Data + offset);
            offset += 4;
        } else {
            table[i] = 0.0f;
        }
    }
    
    // Create CLUT stage
    cmsStage* stage = cmsStageAllocCLutFloat(context, nGridPoints, inputChan, outputChan, table);
    
    if (stage) {
        // Test cmsGetStageContextID
        cmsContext stage_context = cmsGetStageContextID(stage);
        (void)stage_context; // Use result to avoid unused variable warning
        
        // Create a dummy pipeline to test cmsGetPipelineContextID
        cmsPipeline* pipeline = cmsPipelineAlloc(context, inputChan, outputChan);
        if (pipeline) {
            // Test cmsGetPipelineContextID
            cmsContext pipeline_context = cmsGetPipelineContextID(pipeline);
            (void)pipeline_context;
            
            // Add stage to pipeline
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
            
            // Test with profile operations
            FILE* dummy_file = fopen("./dummy_file", "wb");
            if (dummy_file) {
                // Write some minimal ICC profile data
                uint8_t dummy_profile[128] = {0};
                if (Size > 0) {
                    size_t copy_size = Size < 128 ? Size : 128;
                    memcpy(dummy_profile, Data, copy_size);
                }
                fwrite(dummy_profile, 1, 128, dummy_file);
                fclose(dummy_file);
                
                // Try to open profile
                cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
                if (profile) {
                    // Test cmsGetProfileContextID
                    cmsContext profile_context = cmsGetProfileContextID(profile);
                    (void)profile_context;
                    
                    // Create transform
                    cmsHTRANSFORM transform = cmsCreateTransform(profile, TYPE_RGB_8, 
                                                                 profile, TYPE_RGB_8, 
                                                                 INTENT_PERCEPTUAL, 0);
                    if (transform) {
                        // Test cmsGetTransformContextID
                        cmsContext transform_context = cmsGetTransformContextID(transform);
                        (void)transform_context;
                        
                        cmsDeleteTransform(transform);
                    }
                    
                    cmsCloseProfile(profile);
                }
                
                remove("./dummy_file");
            }
            
            cmsPipelineFree(pipeline);
        } else {
            // If pipeline allocation failed, free stage directly
            cmsStageFree(stage);
        }
    }
    
    // Cleanup
    free(table);
    cmsDeleteContext(context);
    
    return 0;
}