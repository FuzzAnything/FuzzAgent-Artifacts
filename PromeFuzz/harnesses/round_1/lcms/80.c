// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsSliceSpace16 at cmslut.c:879:19 in lcms2.h
// cmsSliceSpaceFloat at cmslut.c:910:26 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
// cmsStageSampleCLutFloat at cmslut.c:813:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"

typedef struct {
    cmsUInt32Number inputChannels;
    cmsUInt32Number outputChannels;
} SamplerContext;

static cmsBool dummySampler16(const cmsUInt16Number In[], cmsUInt16Number Out[], void* Cargo) {
    SamplerContext* ctx = (SamplerContext*)Cargo;
    if (!ctx) return FALSE;
    
    cmsUInt32Number i;
    cmsUInt32Number maxChannels = ctx->inputChannels < ctx->outputChannels ? 
                                  ctx->inputChannels : ctx->outputChannels;
    
    for (i = 0; i < maxChannels; i++) {
        Out[i] = In[i];
    }
    return TRUE;
}

static cmsBool dummySamplerFloat(const cmsFloat32Number In[], cmsFloat32Number Out[], void* Cargo) {
    SamplerContext* ctx = (SamplerContext*)Cargo;
    if (!ctx) return FALSE;
    
    cmsUInt32Number i;
    cmsUInt32Number maxChannels = ctx->inputChannels < ctx->outputChannels ? 
                                  ctx->inputChannels : ctx->outputChannels;
    
    for (i = 0; i < maxChannels; i++) {
        Out[i] = In[i];
    }
    return TRUE;
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Parse input data for parameters
    size_t offset = 0;
    
    // Read nInputs (1-16)
    cmsUInt32Number nInputs = (Data[offset] % 16) + 1;
    offset++;
    
    // Read clutPoints array
    cmsUInt32Number clutPoints[16];
    for (cmsUInt32Number i = 0; i < nInputs && offset < Size; i++) {
        clutPoints[i] = (Data[offset] % 10) + 2; // 2-11 grid points
        offset++;
    }
    
    // Read grid points for uniform CLUT
    cmsUInt32Number nGridPoints = (Data[offset] % 10) + 2;
    offset++;
    
    // Read input and output channels
    cmsUInt32Number inputChan = (Data[offset] % 8) + 1;
    offset++;
    cmsUInt32Number outputChan = (Data[offset] % 8) + 1;
    offset++;
    
    // Read dwFlags
    cmsUInt32Number dwFlags = 0;
    if (offset < Size) {
        dwFlags = Data[offset];
        offset++;
    }
    
    // Calculate table size for 16-bit CLUT
    cmsUInt32Number tableSize = 1;
    for (cmsUInt32Number i = 0; i < inputChan; i++) {
        tableSize *= nGridPoints;
    }
    tableSize *= outputChan;
    
    // Allocate and initialize table data
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(tableSize * sizeof(cmsUInt16Number));
    if (!table) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Fill table with fuzzer data
    for (cmsUInt32Number i = 0; i < tableSize && offset < Size; i++) {
        if (offset + 1 < Size) {
            table[i] = (Data[offset] << 8) | Data[offset + 1];
            offset += 2;
        } else {
            table[i] = 0;
        }
    }
    
    // Create sampler contexts
    SamplerContext ctx1 = {nInputs, nInputs}; // For slice space functions
    SamplerContext ctx2 = {inputChan, outputChan}; // For stage functions
    
    // Test cmsSliceSpace16
    cmsSliceSpace16(nInputs, clutPoints, dummySampler16, &ctx1);
    
    // Test cmsSliceSpaceFloat
    cmsSliceSpaceFloat(nInputs, clutPoints, dummySamplerFloat, &ctx1);
    
    // Test cmsStageAllocCLut16bit
    cmsStage* stage1 = cmsStageAllocCLut16bit(context, nGridPoints, inputChan, outputChan, table);
    if (stage1) {
        // Test cmsStageSampleCLut16bit
        cmsStageSampleCLut16bit(stage1, dummySampler16, &ctx2, dwFlags);
        
        // Test cmsStageSampleCLutFloat
        cmsStageSampleCLutFloat(stage1, dummySamplerFloat, &ctx2, dwFlags);
        
        cmsStageFree(stage1);
    }
    
    // Test cmsStageAllocCLut16bitGranular
    cmsStage* stage2 = cmsStageAllocCLut16bitGranular(context, clutPoints, inputChan, outputChan, table);
    if (stage2) {
        cmsStageFree(stage2);
    }
    
    // Cleanup
    free(table);
    cmsDeleteContext(context);
    
    return 0;
}