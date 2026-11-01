// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_stage(cmsStage* stage) {
    if (stage != NULL) {
        cmsStageFree(stage);
    }
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    // Need at least 4 bytes to safely read input parameters
    if (Size < 4) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsStage *stage = NULL;
    cmsUInt32Number inputChan, outputChan, gridPoints;
    cmsUInt32Number clutPoints[4];
    cmsUInt16Number *table16 = NULL;
    cmsFloat32Number *tableFloat = NULL;
    size_t tableSize16, tableSizeFloat;

    // Extract parameters from fuzz input safely
    inputChan = Data[0] % 5; // 0 to 4
    outputChan = Data[1] % 5; // 0 to 4
    gridPoints = (Data[2] % 10) + 1; // 1 to 10

    // Initialize granular points safely
    for (int i = 0; i < 4; i++) {
        // Ensure we don't read out of bounds
        if (3 + i < Size) {
            clutPoints[i] = (Data[3 + i] % 10) + 1;
        } else {
            // Fallback if input is too short
            clutPoints[i] = 1;
        }
    }

    // Ensure valid channel counts for allocation functions (must be > 0 and <= 4)
    if (inputChan == 0) inputChan = 1;
    if (outputChan == 0) outputChan = 1;
    if (inputChan > 4) inputChan = 4;
    if (outputChan > 4) outputChan = 4;

    // Calculate table sizes
    // FIX: cmsStageAllocCLut16bit sets ALL dimensions to gridPoints (up to 4).
    // The library calculates total elements as (gridPoints ^ 4) * outputChan.
    // We must match this calculation to prevent buffer overflow.
    size_t totalGridPoints = 1;
    for (int i = 0; i < 4; i++) { // Always iterate 4 times to match library behavior
        totalGridPoints *= gridPoints;
    }
    tableSize16 = totalGridPoints * outputChan * sizeof(cmsUInt16Number);
    tableSizeFloat = totalGridPoints * outputChan * sizeof(cmsFloat32Number);

    // Prevent excessive allocation
    if (tableSize16 > 1024 * 1024) tableSize16 = 0;
    if (tableSizeFloat > 1024 * 1024) tableSizeFloat = 0;

    // Allocate and initialize table data
    if (tableSize16 > 0) {
        table16 = (cmsUInt16Number *)malloc(tableSize16);
        if (table16) {
            memset(table16, 0, tableSize16);
            // Fill with some data from input if available
            size_t fillLen = (Size < tableSize16) ? Size : tableSize16;
            memcpy(table16, Data, fillLen);
        }
    }

    if (tableSizeFloat > 0) {
        tableFloat = (cmsFloat32Number *)malloc(tableSizeFloat);
        if (tableFloat) {
            memset(tableFloat, 0, tableSizeFloat);
            size_t fillLen = (Size < tableSizeFloat) ? Size : tableSizeFloat;
            memcpy(tableFloat, Data, fillLen);
        }
    }

    // Fuzz cmsStageAllocCLut16bit
    if (table16) {
        stage = cmsStageAllocCLut16bit(ctx, gridPoints, inputChan, outputChan, table16);
        if (stage) {
            cmsUInt32Number in = cmsStageInputChannels(stage);
            cmsUInt32Number out = cmsStageOutputChannels(stage);
            (void)in; (void)out; // Suppress unused warnings
            cleanup_stage(stage);
        }
    }

    // Fuzz cmsStageAllocCLut16bitGranular
    // For Granular, the library uses clutPoints array directly.
    // We need to ensure the allocated size matches the product of clutPoints * outputChan.
    // Re-calculate size specifically for Granular to be safe, though the previous 
    // allocation (based on gridPoints^4) is likely larger or equal if clutPoints <= gridPoints.
    // However, to be strictly correct for the Granular call:
    size_t granularTotalPoints = 1;
    for (int i = 0; i < inputChan; i++) {
        granularTotalPoints *= clutPoints[i];
    }
    size_t granularSize = granularTotalPoints * outputChan * sizeof(cmsUInt16Number);
    
    // If the previous allocation was smaller than what Granular expects (unlikely with gridPoints logic 
    // but possible if clutPoints > gridPoints), we might need to reallocate or ensure safety.
    // Since we allocated based on gridPoints^4, and clutPoints are derived from input, 
    // we must ensure table16 is large enough for the specific Granular call if we use the same buffer.
    // To be safe and simple: if granularSize > tableSize16, reallocate.
    if (table16 && granularSize > tableSize16) {
        table16 = (cmsUInt16Number *)realloc(table16, granularSize);
        if (table16) {
            memset(table16 + (tableSize16/sizeof(cmsUInt16Number)), 0, granularSize - tableSize16);
            // Re-copy data if needed, but memset is sufficient for fuzzing
        }
    }

    if (table16) {
        stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table16);
        if (stage) {
            cmsUInt32Number in = cmsStageInputChannels(stage);
            cmsUInt32Number out = cmsStageOutputChannels(stage);
            (void)in; (void)out;
            cleanup_stage(stage);
        }
    }

    // Fuzz cmsStageAllocCLutFloatGranular
    // Similar logic for float
    size_t floatGranularSize = granularTotalPoints * outputChan * sizeof(cmsFloat32Number);
    if (tableFloat && floatGranularSize > tableSizeFloat) {
        tableFloat = (cmsFloat32Number *)realloc(tableFloat, floatGranularSize);
        if (tableFloat) {
            memset(tableFloat + (tableSizeFloat/sizeof(cmsFloat32Number)), 0, floatGranularSize - tableSizeFloat);
        }
    }

    if (tableFloat) {
        stage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, tableFloat);
        if (stage) {
            cmsUInt32Number in = cmsStageInputChannels(stage);
            cmsUInt32Number out = cmsStageOutputChannels(stage);
            (void)in; (void)out;
            cleanup_stage(stage);
        }
    }

    // Fuzz cmsStageAllocIdentity
    cmsUInt32Number nChannels = (Data[0] % 5);
    if (nChannels == 0) nChannels = 1;
    if (nChannels > 4) nChannels = 4;
    
    stage = cmsStageAllocIdentity(ctx, nChannels);
    if (stage) {
        cmsUInt32Number in = cmsStageInputChannels(stage);
        cmsUInt32Number out = cmsStageOutputChannels(stage);
        (void)in; (void)out;
        cleanup_stage(stage);
    }

    // Cleanup
    free(table16);
    free(tableFloat);
    cmsDeleteContext(ctx);

    return 0;
}