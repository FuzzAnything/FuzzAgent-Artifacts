// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageSampleCLutFloat at cmslut.c:813:19 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageSampleCLutFloat at cmslut.c:813:19 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsSliceSpace16 at cmslut.c:879:19 in lcms2.h
// cmsSliceSpaceFloat at cmslut.c:910:26 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Simple callback functions that don't access cmsStage internals
static int Sampler16(const cmsUInt16Number In[], cmsUInt16Number Out[], void* Cargo) {
    // Use input parameters to determine behavior
    cmsUInt32Number* params = (cmsUInt32Number*)Cargo;
    if (params == NULL) return 0;
    
    cmsUInt32Number inputChan = params[0];
    cmsUInt32Number outputChan = params[1];
    
    if (inputChan == 0 || outputChan == 0) return 0;
    
    // Simple transformation: copy input to output, cycling if needed
    for (cmsUInt32Number i = 0; i < outputChan; i++) {
        if (i < 16) { // Safety bound
            Out[i] = In[i % inputChan];
        }
    }
    return 1;
}

static int SamplerFloat(const cmsFloat32Number In[], cmsFloat32Number Out[], void* Cargo) {
    // Use input parameters to determine behavior
    cmsUInt32Number* params = (cmsUInt32Number*)Cargo;
    if (params == NULL) return 0;
    
    cmsUInt32Number inputChan = params[0];
    cmsUInt32Number outputChan = params[1];
    
    if (inputChan == 0 || outputChan == 0) return 0;
    
    // Simple transformation: copy input to output, cycling if needed
    for (cmsUInt32Number i = 0; i < outputChan; i++) {
        if (i < 16) { // Safety bound
            Out[i] = In[i % inputChan];
        }
    }
    return 1;
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) return 0;
    
    // Parse parameters from fuzzer input
    size_t pos = 0;
    
    // Get input and output channels (1-8 to keep tables small)
    cmsUInt32Number inputChan = (Data[pos++] % 8) + 1;
    if (inputChan == 0) inputChan = 1;
    
    cmsUInt32Number outputChan = (Data[pos++] % 8) + 1;
    if (outputChan == 0) outputChan = 1;
    
    // Get grid points (1-4 to keep tables small)
    cmsUInt32Number nGridPoints = (Data[pos++] % 4) + 1;
    if (nGridPoints == 0) nGridPoints = 1;
    
    // Create array for granular CLUT points
    cmsUInt32Number* clutPoints = NULL;
    if (inputChan > 0 && inputChan <= 8) {
        clutPoints = (cmsUInt32Number*)malloc(inputChan * sizeof(cmsUInt32Number));
        if (clutPoints) {
            for (cmsUInt32Number i = 0; i < inputChan; i++) {
                clutPoints[i] = (Data[pos++ % Size] % 4) + 1;
                if (clutPoints[i] == 0) clutPoints[i] = 1;
            }
        }
    }
    
    // Create parameter array for callbacks
    cmsUInt32Number callbackParams[2] = {inputChan, outputChan};
    
    // Test cmsStageAllocCLutFloatGranular
    if (clutPoints && inputChan > 0 && outputChan > 0) {
        // Calculate table size for granular CLUT
        cmsUInt32Number tableSizeGranular = 1;
        for (cmsUInt32Number i = 0; i < inputChan; i++) {
            tableSizeGranular *= clutPoints[i];
        }
        tableSizeGranular *= outputChan;
        
        // Create table data for granular CLUT
        cmsFloat32Number* tableGranular = NULL;
        if (tableSizeGranular > 0 && tableSizeGranular <= 4096) { // Limit to 4KB
            tableGranular = (cmsFloat32Number*)malloc(tableSizeGranular * sizeof(cmsFloat32Number));
            if (tableGranular) {
                for (cmsUInt32Number i = 0; i < tableSizeGranular; i++) {
                    tableGranular[i] = (cmsFloat32Number)(Data[(pos + i) % Size]) / 255.0f;
                }
                
                cmsStage* stage1 = cmsStageAllocCLutFloatGranular(context, clutPoints, inputChan, outputChan, tableGranular);
                if (stage1) {
                    // Test cmsStageSampleCLutFloat
                    cmsStageSampleCLutFloat(stage1, SamplerFloat, callbackParams, 0);
                    
                    // Test cmsStageSampleCLut16bit
                    cmsStageSampleCLut16bit(stage1, Sampler16, callbackParams, 0);
                    
                    cmsStageFree(stage1);
                }
                free(tableGranular);
            }
        }
    }
    
    // Test cmsStageAllocCLutFloat
    if (inputChan > 0 && outputChan > 0 && nGridPoints > 0) {
        // Calculate table size for uniform CLUT
        cmsUInt32Number tableSizeUniform = 1;
        for (cmsUInt32Number i = 0; i < inputChan; i++) {
            tableSizeUniform *= nGridPoints;
        }
        tableSizeUniform *= outputChan;
        
        // Create table data for uniform CLUT
        cmsFloat32Number* tableUniform = NULL;
        if (tableSizeUniform > 0 && tableSizeUniform <= 4096) { // Limit to 4KB
            tableUniform = (cmsFloat32Number*)malloc(tableSizeUniform * sizeof(cmsFloat32Number));
            if (tableUniform) {
                for (cmsUInt32Number i = 0; i < tableSizeUniform; i++) {
                    tableUniform[i] = (cmsFloat32Number)(Data[(pos + i) % Size]) / 255.0f;
                }
                
                cmsStage* stage2 = cmsStageAllocCLutFloat(context, nGridPoints, inputChan, outputChan, tableUniform);
                if (stage2) {
                    // Test cmsStageSampleCLutFloat
                    cmsStageSampleCLutFloat(stage2, SamplerFloat, callbackParams, 0);
                    
                    // Test cmsStageSampleCLut16bit
                    cmsStageSampleCLut16bit(stage2, Sampler16, callbackParams, 0);
                    
                    cmsStageFree(stage2);
                }
                free(tableUniform);
            }
        }
    }
    
    // Test cmsSliceSpace16
    if (clutPoints && inputChan > 0) {
        cmsSliceSpace16(inputChan, clutPoints, Sampler16, callbackParams);
    }
    
    // Test cmsSliceSpaceFloat
    if (clutPoints && inputChan > 0) {
        cmsSliceSpaceFloat(inputChan, clutPoints, SamplerFloat, callbackParams);
    }
    
    // Cleanup
    if (clutPoints) free(clutPoints);
    cmsDeleteContext(context);
    
    return 0;
}