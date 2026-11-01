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
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

// Corrected signature: const Input, Output
static cmsBool SampleCallback16(const cmsUInt16Number* In, cmsUInt16Number* Out, void* Cargo) {
    (void)In;
    (void)Cargo;
    if (Out) {
        Out[0] = 0;
    }
    return TRUE;
}

// Corrected signature: const Input, Output
static cmsBool SampleCallbackFloat(const cmsFloat32Number* In, cmsFloat32Number* Out, void* Cargo) {
    (void)In;
    (void)Cargo;
    if (Out) {
        Out[0] = 0.0f;
    }
    return TRUE;
}

static cmsContext createContextFromFuzz(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;
    
    (void)Data;
    (void)Size;
    return ctx;
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = createContextFromFuzz(Data, Size);
    if (!ctx) return 0;

    // Extract parameters from fuzz data
    uint32_t nGridPoints = (Data[0] % 32) + 2; // 2 to 33
    uint32_t inputChan = (Data[1] % 5) + 1;    // 1 to 5
    uint32_t outputChan = (Data[1] % 5) + 1;   // 1 to 5
    
    // Ensure channel limits are respected to avoid immediate crashes in allocators
    if (inputChan > 16) inputChan = 16;
    if (outputChan > 16) outputChan = 16;

    // Calculate total points carefully to avoid overflow
    // The library expects: totalPoints = nGridPoints ^ inputChan
    // We must allocate exactly that much for the table
    size_t totalPoints = 1;
    int overflow = 0;
    for (uint32_t i = 0; i < inputChan; i++) {
        if (totalPoints > 100000 / nGridPoints) {
            overflow = 1;
            break;
        }
        totalPoints *= nGridPoints;
    }
    
    // If overflow or too large, skip allocation to prevent OOM or huge allocations
    if (overflow || totalPoints > 100000) {
        cmsDeleteContext(ctx);
        return 0;
    }

    size_t tableSize16 = totalPoints * outputChan * sizeof(cmsUInt16Number);
    size_t tableSizeFloat = totalPoints * outputChan * sizeof(cmsFloat32Number);

    cmsUInt16Number* table16 = (cmsUInt16Number*)malloc(tableSize16);
    cmsFloat32Number* tableFloat = (cmsFloat32Number*)malloc(tableSizeFloat);

    if (!table16 || !tableFloat) {
        cmsDeleteContext(ctx);
        free(table16);
        free(tableFloat);
        return 0;
    }

    // Initialize tables with fuzz data or patterns
    memset(table16, 0, tableSize16);
    memset(tableFloat, 0, tableSizeFloat);
    
    // Fill with some data derived from input to vary state
    // Ensure we do not write beyond allocated memory
    size_t fillLimit16 = (Size < tableSize16 / sizeof(cmsUInt16Number)) ? Size : (tableSize16 / sizeof(cmsUInt16Number));
    for (size_t i = 0; i < fillLimit16; i++) {
        table16[i] = (cmsUInt16Number)(Data[i % Size] * 256);
    }
    
    size_t fillLimitFloat = (Size < tableSizeFloat / sizeof(cmsFloat32Number)) ? Size : (tableSizeFloat / sizeof(cmsFloat32Number));
    for (size_t i = 0; i < fillLimitFloat; i++) {
        tableFloat[i] = (cmsFloat32Number)(Data[i % Size] / 255.0f);
    }

    // 1. Test cmsStageAllocCLut16bit
    // This function expects the Table to have exactly (nGridPoints^inputChan * outputChan) elements
    cmsStage* stage16 = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table16);
    
    if (stage16) {
        // 2. Test cmsStageSampleCLut16bit
        cmsBool res1 = cmsStageSampleCLut16bit(stage16, SampleCallback16, NULL, 0);
        (void)res1; // Suppress unused warning

        // Cleanup stage16
        cmsStageFree(stage16);
    }

    // 3. Test cmsStageAllocCLutFloatGranular
    // Prepare clutPoints array for granular allocation
    cmsUInt32Number clutPoints[16];
    for (uint32_t i = 0; i < inputChan && i < 16; i++) {
        clutPoints[i] = nGridPoints;
    }
    
    cmsStage* stageFloat = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, tableFloat);
    
    if (stageFloat) {
        // 4. Test cmsStageSampleCLutFloat
        cmsBool res2 = cmsStageSampleCLutFloat(stageFloat, SampleCallbackFloat, NULL, 0);
        (void)res2;

        // Cleanup stageFloat
        cmsStageFree(stageFloat);
    }

    // 5. Test cmsSliceSpace16
    // Re-use clutPoints array
    cmsBool res3 = cmsSliceSpace16(inputChan, clutPoints, SampleCallback16, NULL);
    (void)res3;

    // 6. Test cmsSliceSpaceFloat
    cmsBool res4 = cmsSliceSpaceFloat(inputChan, clutPoints, SampleCallbackFloat, NULL);
    (void)res4;

    // Cleanup
    free(table16);
    free(tableFloat);
    cmsDeleteContext(ctx);

    return 0;
}