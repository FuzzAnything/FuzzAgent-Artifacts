// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
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

// Dummy functions for _cmsStageAllocPlaceholder with correct signatures
static void dummyEval(const float* in, float* out, const cmsStage* mpe) {
    // Do nothing
}

static void* dummyDup(cmsStage* mpe) {
    return NULL;
}

static void dummyFree(cmsStage* mpe) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Use fuzzer data to generate parameters
    size_t offset = 0;
    
    // Extract parameters from fuzzer input with bounds checking
    cmsUInt32Number nGridPoints = 2;
    cmsUInt32Number inputChan = 1;
    cmsUInt32Number outputChan = 1;
    cmsUInt32Number nChannels = 1;
    
    if (Size >= 4) {
        nGridPoints = (Data[0] % 8) + 2;  // 2-9 grid points
        inputChan = (Data[1] % 8) + 1;    // 1-8 input channels
        outputChan = (Data[2] % 8) + 1;   // 1-8 output channels
        nChannels = (Data[3] % 8) + 1;    // 1-8 channels for identity
        offset = 4;
    }
    
    // Limit to reasonable sizes to avoid excessive memory usage
    if (nGridPoints > 10) nGridPoints = 10;
    if (inputChan > 16) inputChan = 16;
    if (outputChan > 16) outputChan = 16;
    if (nChannels > 16) nChannels = 16;
    
    // Test cmsStageAllocCLut16bit
    {
        // Calculate table size
        size_t tableSize = 1;
        for (cmsUInt32Number i = 0; i < inputChan; i++) {
            tableSize *= nGridPoints;
        }
        tableSize *= outputChan;
        
        // Allocate and initialize table
        cmsUInt16Number* table = (cmsUInt16Number*)calloc(tableSize, sizeof(cmsUInt16Number));
        if (table) {
            // Fill table with fuzzer data
            for (size_t i = 0; i < tableSize && offset < Size; i++, offset++) {
                table[i] = (cmsUInt16Number)(Data[offset % Size] << 8) | Data[(offset + 1) % Size];
            }
            
            cmsStage* stage = cmsStageAllocCLut16bit(context, nGridPoints, inputChan, outputChan, table);
            if (stage) {
                // Test query functions
                cmsUInt32Number inCh = cmsStageInputChannels(stage);
                cmsUInt32Number outCh = cmsStageOutputChannels(stage);
                (void)inCh;  // Use to avoid unused variable warning
                (void)outCh; // Use to avoid unused variable warning
                
                cmsStageFree(stage);
            }
            free(table);
        }
    }
    
    // Test cmsStageAllocIdentity
    {
        cmsStage* stage = cmsStageAllocIdentity(context, nChannels);
        if (stage) {
            cmsUInt32Number inCh = cmsStageInputChannels(stage);
            cmsUInt32Number outCh = cmsStageOutputChannels(stage);
            (void)inCh;
            (void)outCh;
            
            cmsStageFree(stage);
        }
    }
    
    // Test cmsStageAllocCLut16bitGranular
    {
        cmsUInt32Number clutPoints[16];
        for (cmsUInt32Number i = 0; i < inputChan && i < 16; i++) {
            clutPoints[i] = (Data[(offset + i) % Size] % 8) + 2;
            if (clutPoints[i] > 10) clutPoints[i] = 10;
        }
        offset += inputChan;
        
        // Calculate table size
        size_t tableSize = 1;
        for (cmsUInt32Number i = 0; i < inputChan; i++) {
            tableSize *= clutPoints[i];
        }
        tableSize *= outputChan;
        
        // Allocate and initialize table
        cmsUInt16Number* table = (cmsUInt16Number*)calloc(tableSize, sizeof(cmsUInt16Number));
        if (table) {
            // Fill table with fuzzer data
            for (size_t i = 0; i < tableSize && offset < Size; i++, offset++) {
                table[i] = (cmsUInt16Number)(Data[offset % Size] << 8) | Data[(offset + 1) % Size];
            }
            
            cmsStage* stage = cmsStageAllocCLut16bitGranular(context, clutPoints, inputChan, outputChan, table);
            if (stage) {
                cmsUInt32Number inCh = cmsStageInputChannels(stage);
                cmsUInt32Number outCh = cmsStageOutputChannels(stage);
                (void)inCh;
                (void)outCh;
                
                cmsStageFree(stage);
            }
            free(table);
        }
    }
    
    // Test _cmsStageAllocPlaceholder
    {
        cmsStageSignature type = (cmsStageSignature)(Data[offset % Size] % 256);
        offset++;
        
        cmsStage* stage = _cmsStageAllocPlaceholder(context, type, inputChan, outputChan, 
                                                   dummyEval, dummyDup, dummyFree, NULL);
        if (stage) {
            cmsUInt32Number inCh = cmsStageInputChannels(stage);
            cmsUInt32Number outCh = cmsStageOutputChannels(stage);
            (void)inCh;
            (void)outCh;
            
            cmsStageFree(stage);
        }
    }
    
    // Cleanup
    cmsDeleteContext(context);
    
    return 0;
}