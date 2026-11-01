// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsContext create_context(const uint8_t *Data, size_t Size) {
    (void)Data; (void)Size;
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return NULL;
    return ctx;
}

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static void cleanup_stage(cmsStage *stage) {
    if (stage != NULL) {
        cmsStageFree(stage);
    }
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = create_context(Data, Size);
    if (ctx == NULL) return 0;

    // Extract parameters from fuzz data
    cmsUInt32Number inputChan = Data[0] & 0x03; // Limit to 0-3 (max 4 channels)
    cmsUInt32Number outputChan = Data[1] & 0x03;
    cmsUInt32Number gridPoints = (Data[2] % 5) + 2; // 2 to 6
    cmsUInt32Number clutPoints[4] = {0};
    
    for (int i = 0; i < 4; i++) {
        if (Size > 3 + i) {
            clutPoints[i] = (Data[3 + i] % 5) + 2; // 2 to 6
        } else {
            clutPoints[i] = gridPoints;
        }
    }

    // Ensure channel counts are within reasonable limits for lcms (max 4 usually)
    if (inputChan > 4) inputChan = 4;
    if (outputChan > 4) outputChan = 4;
    if (inputChan == 0) inputChan = 1;
    if (outputChan == 0) outputChan = 1;

    // Calculate table sizes
    size_t tableSize16 = 1;
    size_t tableSizeFloat = 1;
    for (int i = 0; i < inputChan; i++) {
        tableSize16 *= clutPoints[i];
        tableSizeFloat *= clutPoints[i];
    }
    tableSize16 *= outputChan;
    tableSizeFloat *= outputChan;

    // Safety check to prevent integer overflow or huge allocations
    if (tableSize16 > 1000000 || tableSizeFloat > 1000000) {
        cleanup_context(ctx);
        return 0;
    }

    // Allocate tables
    cmsUInt16Number *table16 = NULL;
    cmsFloat32Number *tableFloat = NULL;

    if (tableSize16 > 0) {
        table16 = (cmsUInt16Number *)malloc(tableSize16 * sizeof(cmsUInt16Number));
        if (table16) {
            memset(table16, 0, tableSize16 * sizeof(cmsUInt16Number));
            // Fill with fuzz data if available
            size_t fillLen = (Size > 10) ? (Size - 10) : 0;
            if (fillLen > 0) {
                // Ensure we don't write past the allocated buffer
                size_t maxBytes = tableSize16 * sizeof(cmsUInt16Number);
                if (fillLen > maxBytes) fillLen = maxBytes;
                memcpy(table16, Data + 10, fillLen);
            }
        }
    }

    if (tableSizeFloat > 0) {
        tableFloat = (cmsFloat32Number *)malloc(tableSizeFloat * sizeof(cmsFloat32Number));
        if (tableFloat) {
            memset(tableFloat, 0, tableSizeFloat * sizeof(cmsFloat32Number));
            // Fill with fuzz data
            size_t fillLen = (Size > 10) ? (Size - 10) : 0;
            if (fillLen > 0) {
                // Ensure we don't write past the allocated buffer
                size_t maxBytes = tableSizeFloat * sizeof(cmsFloat32Number);
                if (fillLen > maxBytes) fillLen = maxBytes;
                memcpy(tableFloat, Data + 10, fillLen);
            }
        }
    }

    cmsStage *stage16 = NULL;
    cmsStage *stageFloat = NULL;
    cmsStage *stageFloatGranular = NULL;
    cmsStage *stagePlaceholder = NULL;

    // 1. Test cmsStageAllocCLut16bitGranular
    if (table16) {
        stage16 = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table16);
        if (stage16) {
            cmsUInt32Number in = cmsStageInputChannels(stage16);
            cmsUInt32Number out = cmsStageOutputChannels(stage16);
            (void)in; (void)out; // Suppress unused warnings
        }
    }

    // 2. Test cmsStageAllocCLutFloat
    // Note: cmsStageAllocCLutFloat expects a uniform grid, so we use gridPoints for all dimensions
    // The internal logic of lcms calculates the expected table size based on gridPoints^inputChan * outputChan
    // We must ensure tableFloat is large enough for this specific call, or the call will read out of bounds.
    // However, since we allocated tableFloat based on clutPoints (which might differ from gridPoints),
    // we need to be careful. The safest way is to only call this if the sizes match or if we reallocate.
    // To keep it simple and safe, we will only call this if the calculated size matches the allocated size.
    // Or better, we allocate a separate buffer specifically for the uniform grid call if needed.
    // But to avoid complexity, let's just ensure the buffer passed is large enough for the uniform grid case.
    // The uniform grid size is: gridPoints^inputChan * outputChan.
    size_t uniformGridSize = 1;
    for(int i=0; i<inputChan; i++) uniformGridSize *= gridPoints;
    uniformGridSize *= outputChan;

    if (tableFloat && uniformGridSize <= tableSizeFloat) {
        stageFloat = cmsStageAllocCLutFloat(ctx, gridPoints, inputChan, outputChan, tableFloat);
        if (stageFloat) {
            cmsUInt32Number in = cmsStageInputChannels(stageFloat);
            cmsUInt32Number out = cmsStageOutputChannels(stageFloat);
            (void)in; (void)out;
        }
    } else if (uniformGridSize > 0 && uniformGridSize <= 1000000) {
        // If the uniform grid size differs, allocate a temporary buffer for this specific call
        cmsFloat32Number *tempTable = (cmsFloat32Number *)malloc(uniformGridSize * sizeof(cmsFloat32Number));
        if (tempTable) {
            memset(tempTable, 0, uniformGridSize * sizeof(cmsFloat32Number));
            size_t fillLen = (Size > 10) ? (Size - 10) : 0;
            if (fillLen > 0) {
                size_t maxBytes = uniformGridSize * sizeof(cmsFloat32Number);
                if (fillLen > maxBytes) fillLen = maxBytes;
                memcpy(tempTable, Data + 10, fillLen);
            }
            stageFloat = cmsStageAllocCLutFloat(ctx, gridPoints, inputChan, outputChan, tempTable);
            if (stageFloat) {
                cmsUInt32Number in = cmsStageInputChannels(stageFloat);
                cmsUInt32Number out = cmsStageOutputChannels(stageFloat);
                (void)in; (void)out;
            }
            free(tempTable);
        }
    }

    // 3. Test cmsStageAllocCLutFloatGranular
    // This uses clutPoints, so the size we allocated (tableSizeFloat) is correct.
    if (tableFloat) {
        stageFloatGranular = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, tableFloat);
        if (stageFloatGranular) {
            cmsUInt32Number in = cmsStageInputChannels(stageFloatGranular);
            cmsUInt32Number out = cmsStageOutputChannels(stageFloatGranular);
            (void)in; (void)out;
        }
    }

    // 4. Test _cmsStageAllocPlaceholder
    // We pass NULL for function pointers to test allocation logic with minimal setup
    stagePlaceholder = _cmsStageAllocPlaceholder(ctx, 0, inputChan, outputChan, NULL, NULL, NULL, NULL);
    if (stagePlaceholder) {
        cmsUInt32Number in = cmsStageInputChannels(stagePlaceholder);
        cmsUInt32Number out = cmsStageOutputChannels(stagePlaceholder);
        (void)in; (void)out;
    }

    // Cleanup
    cleanup_stage(stage16);
    cleanup_stage(stageFloat);
    cleanup_stage(stageFloatGranular);
    cleanup_stage(stagePlaceholder);
    
    free(table16);
    free(tableFloat);
    cleanup_context(ctx);

    return 0;
}