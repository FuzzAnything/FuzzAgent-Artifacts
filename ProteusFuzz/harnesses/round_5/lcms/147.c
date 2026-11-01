// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageSampleCLutFloat at cmslut.c:813:19 in lcms2.h
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
#include <lcms2.h>

// Static helper to avoid name conflicts
// Note: The actual signature for cmsSAMPLER16 in lcms2.h is:
// int (*cmsSAMPLER16)(const cmsUInt16Number* In, cmsUInt16Number* Out, void* Cargo)
// The nInputs parameter is NOT in the callback signature for the Stage/Slice functions
// as the grid iteration handles that. The callback receives the flattened row.
// However, looking at the error, the user's provided signature in the prompt included nInputs.
// But the compiler error shows the actual type expected is: int (*)(const unsigned short *, unsigned short *, void *)
// So we must match the actual header definition, not the prompt's potentially simplified description.

static int FuzzSampler16(const cmsUInt16Number* In, cmsUInt16Number* Out, void* Cargo) {
    if (Cargo == NULL) return 0;
    // Cargo is used to store the max channels or a flag
    cmsUInt32Number max_out = *(cmsUInt32Number*)Cargo;
    
    if (In != NULL && Out != NULL && max_out > 0) {
        // We don't know the exact channel count here without context, 
        // but we can copy a safe amount or rely on the caller's logic.
        // For fuzzing, just copy a few bytes to ensure execution.
        for (int i = 0; i < max_out && i < 10; i++) {
            Out[i] = In[i];
        }
    }
    return 1;
}

static int FuzzSamplerFloat(const cmsFloat32Number* In, cmsFloat32Number* Out, void* Cargo) {
    if (Cargo == NULL) return 0;
    cmsUInt32Number max_out = *(cmsUInt32Number*)Cargo;
    
    if (In != NULL && Out != NULL && max_out > 0) {
        for (int i = 0; i < max_out && i < 10; i++) {
            Out[i] = In[i];
        }
    }
    return 1;
}

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Extract parameters from fuzz data
    cmsUInt32Number nGridPoints = (Data[0] % 10) + 2; // 2 to 11
    cmsUInt32Number inputChan = (Data[1] % 5) + 1;   // 1 to 5
    cmsUInt32Number outputChan = (Data[2] % 5) + 1;  // 1 to 5
    cmsUInt32Number flags = Data[3];
    
    // Prepare grid points array (max 4 dimensions for safety)
    cmsUInt32Number clutPoints[4];
    for (int i = 0; i < 4; i++) {
        clutPoints[i] = (Data[4 + i] % 10) + 2;
    }

    // Calculate buffer sizes
    // For 16-bit: (nGridPoints ^ 4) * outputChan * sizeof(cmsUInt16Number)
    size_t gridSize16 = 1;
    for (int i = 0; i < 4; i++) gridSize16 *= clutPoints[i];
    size_t tableSize16 = gridSize16 * outputChan * sizeof(cmsUInt16Number);

    // For float: same logic but float size
    size_t tableSizeFloat = gridSize16 * outputChan * sizeof(cmsFloat32Number);

    // Allocate tables
    cmsUInt16Number* table16 = NULL;
    cmsFloat32Number* tableFloat = NULL;
    cmsStage* stage16 = NULL;
    cmsStage* stageFloat = NULL;

    if (tableSize16 > 0 && tableSize16 < 1024 * 1024 * 10) { // Limit size
        table16 = (cmsUInt16Number*)malloc(tableSize16);
        if (table16) {
            memset(table16, 0, tableSize16);
            // Fill with some data from fuzz input if available
            size_t fillLen = (Size > 10) ? (Size - 10) : 0;
            size_t copyLen = (fillLen < tableSize16) ? fillLen : tableSize16;
            memcpy(table16, Data + 10, copyLen);
        }
    }

    if (tableSizeFloat > 0 && tableSizeFloat < 1024 * 1024 * 10) {
        tableFloat = (cmsFloat32Number*)malloc(tableSizeFloat);
        if (tableFloat) {
            memset(tableFloat, 0, tableSizeFloat);
            size_t fillLen = (Size > 10) ? (Size - 10) : 0;
            size_t copyLen = (fillLen < tableSizeFloat) ? fillLen : tableSizeFloat;
            memcpy(tableFloat, Data + 10, copyLen);
        }
    }

    cmsUInt32Number cargoVal = 10; // Max channels for sampler

    // Test 1: cmsStageAllocCLut16bit
    if (table16 != NULL) {
        stage16 = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table16);
        if (stage16 != NULL) {
            // Test cmsStageSampleCLut16bit
            cmsStageSampleCLut16bit(stage16, (cmsSAMPLER16)FuzzSampler16, &cargoVal, flags);
            cmsStageFree(stage16);
        }
    }

    // Test 2: cmsStageAllocCLutFloatGranular
    if (tableFloat != NULL) {
        stageFloat = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, tableFloat);
        if (stageFloat != NULL) {
            // Test cmsStageSampleCLutFloat
            cmsStageSampleCLutFloat(stageFloat, (cmsSAMPLERFLOAT)FuzzSamplerFloat, &cargoVal, flags);
            cmsStageFree(stageFloat);
        }
    }

    // Test 3: cmsSliceSpace16
    // Note: cmsSliceSpace16 expects a callback that matches cmsSAMPLER16 signature
    cmsSliceSpace16(inputChan, clutPoints, (cmsSAMPLER16)FuzzSampler16, &cargoVal);

    // Test 4: cmsSliceSpaceFloat
    cmsSliceSpaceFloat(inputChan, clutPoints, (cmsSAMPLERFLOAT)FuzzSamplerFloat, &cargoVal);

    // Cleanup
    if (table16) free(table16);
    if (tableFloat) free(tableFloat);
    cmsDeleteContext(ctx);

    return 0;
}