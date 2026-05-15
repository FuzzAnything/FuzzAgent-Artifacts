// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageSampleCLutFloat at cmslut.c:813:19 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageSampleCLutFloat at cmslut.c:813:19 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsSliceSpaceFloat at cmslut.c:910:26 in lcms2.h
// cmsSliceSpace16 at cmslut.c:879:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Sampler callbacks
static cmsBool myFloatSampler(const cmsFloat32Number In[], 
                               cmsFloat32Number Out[], 
                               void* Cargo) {
    // Just copy input to output as a simple transformation
    cmsUInt32Number i;
    for (i = 0; i < 16; i++) {  // Use reasonable max channels
        Out[i] = In[i];
    }
    return TRUE;
}

static cmsBool my16bitSampler(const cmsUInt16Number In[], 
                               cmsUInt16Number Out[], 
                               void* Cargo) {
    // Just copy input to output as a simple transformation
    cmsUInt32Number i;
    for (i = 0; i < 16; i++) {  // Use reasonable max channels
        Out[i] = In[i];
    }
    return TRUE;
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    // Use a default context
    cmsContext context = NULL;
    
    // Ensure we have minimum data
    if (Size < 4) return 0;
    
    // Parse some parameters from fuzzer input
    cmsUInt32Number nInputs = (Data[0] % 8) + 1;  // 1-8 inputs
    cmsUInt32Number nGridPoints = (Data[1] % 16) + 2;  // 2-17 grid points
    cmsUInt32Number inputChan = (Data[2] % 8) + 1;  // 1-8 input channels
    cmsUInt32Number outputChan = (Data[3] % 8) + 1;  // 1-8 output channels
    
    // Limit to reasonable values to avoid excessive memory
    if (nInputs > 8) nInputs = 8;
    if (nGridPoints > 16) nGridPoints = 16;
    if (inputChan > 8) inputChan = 8;
    if (outputChan > 8) outputChan = 8;
    
    // Create clutPoints array
    cmsUInt32Number clutPoints[8];
    for (cmsUInt32Number i = 0; i < nInputs; i++) {
        if (i + 4 < Size) {
            clutPoints[i] = (Data[i + 4] % 16) + 2;  // 2-17 points per dimension
        } else {
            clutPoints[i] = nGridPoints;
        }
        if (clutPoints[i] > 16) clutPoints[i] = 16;
    }
    
    // Calculate table size for float CLUT
    size_t tableSize = 1;
    for (cmsUInt32Number i = 0; i < nInputs; i++) {
        tableSize *= clutPoints[i];
    }
    tableSize *= inputChan * outputChan;
    
    // Allocate and initialize table data
    cmsFloat32Number* table = NULL;
    if (tableSize > 0 && tableSize < 1000000) {  // Reasonable limit
        table = (cmsFloat32Number*)malloc(tableSize * sizeof(cmsFloat32Number));
        if (table) {
            // Initialize with fuzzer data or zeros
            size_t dataToCopy = Size > (tableSize * sizeof(cmsFloat32Number)) ? 
                               (tableSize * sizeof(cmsFloat32Number)) : Size;
            memcpy(table, Data, dataToCopy);
            
            // Fill remaining with zeros if needed
            if (dataToCopy < tableSize * sizeof(cmsFloat32Number)) {
                memset((uint8_t*)table + dataToCopy, 0, 
                       tableSize * sizeof(cmsFloat32Number) - dataToCopy);
            }
        }
    }
    
    // Test cmsSliceSpaceFloat
    if (nInputs > 0 && nInputs <= 8) {
        cmsSliceSpaceFloat(nInputs, clutPoints, myFloatSampler, NULL);
    }
    
    // Test cmsSliceSpace16
    if (nInputs > 0 && nInputs <= 8) {
        cmsSliceSpace16(nInputs, clutPoints, my16bitSampler, NULL);
    }
    
    // Test cmsStageAllocCLutFloatGranular
    if (table && nInputs > 0 && nInputs <= 8 && inputChan > 0 && outputChan > 0) {
        cmsStage* stage1 = cmsStageAllocCLutFloatGranular(context, clutPoints, 
                                                         inputChan, outputChan, table);
        if (stage1) {
            // Test cmsStageSampleCLutFloat
            cmsStageSampleCLutFloat(stage1, myFloatSampler, NULL, 0);
            
            // Test cmsStageSampleCLut16bit
            cmsStageSampleCLut16bit(stage1, my16bitSampler, NULL, 0);
            
            // Clean up
            cmsStageFree(stage1);
        }
    }
    
    // Test cmsStageAllocCLutFloat (uniform grid)
    if (table && inputChan > 0 && outputChan > 0) {
        // Calculate uniform table size
        size_t uniformTableSize = 1;
        for (cmsUInt32Number i = 0; i < nInputs; i++) {
            uniformTableSize *= nGridPoints;
        }
        uniformTableSize *= inputChan * outputChan;
        
        if (uniformTableSize > 0 && uniformTableSize < 1000000) {
            cmsStage* stage2 = cmsStageAllocCLutFloat(context, nGridPoints, 
                                                     inputChan, outputChan, table);
            if (stage2) {
                // Test sampling
                cmsStageSampleCLutFloat(stage2, myFloatSampler, NULL, 0);
                cmsStageSampleCLut16bit(stage2, my16bitSampler, NULL, 0);
                
                // Clean up
                cmsStageFree(stage2);
            }
        }
    }
    
    // Clean up
    if (table) {
        free(table);
    }
    
    return 0;
}