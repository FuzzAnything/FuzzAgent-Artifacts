// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsGetPipelineContextID at cmslut.c:1407:22 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
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

static void dummy_cmsFree(void* ptr) {
    // Do nothing - we don't own the memory
}

static void* dummy_cmsDup(void* ptr) {
    return ptr;
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    // Create a context first
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Test cmsStageAllocCLutFloat with various parameters
    // Use first 4 bytes to determine parameters (with bounds checking)
    if (Size >= 4) {
        cmsUInt32Number nGridPoints = (Data[0] % 8) + 2;  // 2-9 grid points
        cmsUInt32Number inputChan = (Data[1] % 4) + 1;    // 1-4 input channels
        cmsUInt32Number outputChan = (Data[2] % 4) + 1;   // 1-4 output channels
        
        // Calculate table size
        size_t tableSize = 1;
        for (cmsUInt32Number i = 0; i < inputChan; i++) {
            tableSize *= nGridPoints;
        }
        tableSize *= outputChan;
        
        // Allocate and fill table with fuzzed data
        cmsFloat32Number* table = (cmsFloat32Number*)malloc(tableSize * sizeof(cmsFloat32Number));
        if (table) {
            // Fill table with data from fuzzer input
            size_t dataToCopy = (Size - 3) * sizeof(uint8_t);
            if (dataToCopy > tableSize * sizeof(cmsFloat32Number)) {
                dataToCopy = tableSize * sizeof(cmsFloat32Number);
            }
            
            if (dataToCopy > 0) {
                // Copy available data
                memcpy(table, Data + 3, dataToCopy);
                // Fill remainder with zeros if needed
                if (dataToCopy < tableSize * sizeof(cmsFloat32Number)) {
                    memset((uint8_t*)table + dataToCopy, 0, 
                           tableSize * sizeof(cmsFloat32Number) - dataToCopy);
                }
            } else {
                // No data available, fill with zeros
                memset(table, 0, tableSize * sizeof(cmsFloat32Number));
            }
            
            // Create the CLUT stage
            cmsStage* stage = cmsStageAllocCLutFloat(context, nGridPoints, inputChan, outputChan, table);
            
            if (stage) {
                // Test cmsGetStageContextID
                cmsContext stageContext = cmsGetStageContextID(stage);
                
                // Test cmsGetPipelineContextID by creating a pipeline
                cmsPipeline* pipeline = cmsPipelineAlloc(context, inputChan, outputChan);
                if (pipeline) {
                    cmsContext pipelineContext = cmsGetPipelineContextID(pipeline);
                    
                    // Add stage to pipeline
                    cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
                    
                    // Create dummy transform for testing
                    cmsHPROFILE profile = cmsCreate_sRGBProfile();
                    if (profile) {
                        // Test cmsGetProfileContextID
                        cmsContext profileContext = cmsGetProfileContextID(profile);
                        
                        // Create transform
                        cmsHTRANSFORM transform = cmsCreateTransform(profile, TYPE_RGB_8, 
                                                                     profile, TYPE_RGB_8, 
                                                                     INTENT_PERCEPTUAL, 0);
                        if (transform) {
                            // Test cmsGetTransformContextID
                            cmsContext transformContext = cmsGetTransformContextID(transform);
                            
                            // Clean up transform
                            cmsDeleteTransform(transform);
                        }
                        
                        // Clean up profile
                        cmsCloseProfile(profile);
                    }
                    
                    // Clean up pipeline (stage is owned by pipeline now)
                    cmsPipelineFree(pipeline);
                } else {
                    // If pipeline creation failed, free the stage directly
                    cmsStageFree(stage);
                }
            }
            
            free(table);
        }
    }
    
    // Clean up context
    cmsDeleteContext(context);
    
    return 0;
}