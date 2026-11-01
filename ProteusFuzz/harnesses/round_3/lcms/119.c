// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsSliceSpace16 at cmslut.c:879:19 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageSampleCLutFloat at cmslut.c:813:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsSliceSpaceFloat at cmslut.c:910:26 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsContext g_context = NULL;

static cmsBool CMSEXPORT dummy_sampler16(const cmsUInt16Number *In, cmsUInt16Number *Out, void *Cargo) {
    (void)In;
    (void)Out;
    (void)Cargo;
    return TRUE;
}

static cmsBool CMSEXPORT dummy_sampler_float(const cmsFloat32Number *In, cmsFloat32Number *Out, void *Cargo) {
    (void)In;
    (void)Out;
    (void)Cargo;
    return TRUE;
}

static void cleanup_fuzz(void) {
    if (g_context != NULL) {
        cmsDeleteContext(g_context);
        g_context = NULL;
    }
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    // Need at least 4 bytes for basic parameters
    if (Size < 4) return 0;

    // Ensure context is initialized
    if (g_context == NULL) {
        g_context = cmsCreateContext(NULL, NULL);
        if (g_context == NULL) return 0;
    }

    // Extract parameters from input data
    uint32_t nInputs = (Data[0] % 8) + 1; // 1 to 8
    uint32_t gridPoints = (Data[1] % 5) + 1; // 1 to 5
    uint32_t inputChan = (Data[2] % 8) + 1; // 1 to 8
    uint32_t outputChan = (Data[3] % 8) + 1; // 1 to 8

    // Create clutPoints array dynamically based on nInputs
    cmsUInt32Number *clutPoints = NULL;
    if (nInputs > 0) {
        clutPoints = (cmsUInt32Number *)malloc(nInputs * sizeof(cmsUInt32Number));
        if (!clutPoints) return 0;
        
        // Ensure we don't read past the buffer
        size_t availableData = Size - 4;
        for (uint32_t i = 0; i < nInputs; i++) {
            if (i < availableData) {
                clutPoints[i] = (Data[4 + i] % 5) + 1;
            } else {
                // Fallback if input data is insufficient
                clutPoints[i] = (i % 5) + 1;
            }
        }
    }

    // Calculate total points for table allocation based on clutPoints
    uint64_t totalPoints = 1;
    if (clutPoints) {
        for (uint32_t i = 0; i < nInputs; i++) {
            totalPoints *= clutPoints[i];
            if (totalPoints > 1000000) { // Prevent huge allocations
                totalPoints = 1000000;
                break;
            }
        }
    }

    // Allocate tables
    cmsUInt16Number *table16 = NULL;
    cmsFloat32Number *tableFloat = NULL;

    size_t table16Size = totalPoints * outputChan * sizeof(cmsUInt16Number);
    size_t tableFloatSize = totalPoints * outputChan * sizeof(cmsFloat32Number);

    // Limit allocation size to prevent OOM or huge memory usage
    if (table16Size > 0 && table16Size < (1024 * 1024 * 10)) {
        table16 = (cmsUInt16Number *)malloc(table16Size);
        if (table16) {
            memset(table16, 0, table16Size);
            // Fill with some pattern based on input data if available
            // Fix: Ensure we only read up to available data and write up to table16Size
            size_t fillLen = (Size > 4) ? (Size - 4) : 0;
            size_t writeCount = (table16Size / sizeof(cmsUInt16Number));
            
            for (size_t i = 0; i < writeCount; i++) {
                if (i < fillLen) {
                    table16[i] = Data[4 + i];
                } else {
                    // Fallback pattern if input data is exhausted
                    table16[i] = (uint16_t)(i % 65536);
                }
            }
        }
    }

    if (tableFloatSize > 0 && tableFloatSize < (1024 * 1024 * 10)) {
        tableFloat = (cmsFloat32Number *)malloc(tableFloatSize);
        if (tableFloat) {
            memset(tableFloat, 0, tableFloatSize);
            size_t fillLen = (Size > 4) ? (Size - 4) : 0;
            size_t writeCount = (tableFloatSize / sizeof(cmsFloat32Number));
            
            for (size_t i = 0; i < writeCount; i++) {
                if (i < fillLen) {
                    tableFloat[i] = (float)(Data[4 + i] / 255.0f);
                } else {
                    // Fallback pattern
                    tableFloat[i] = (float)(i % 256) / 255.0f;
                }
            }
        }
    }

    // Test 1: cmsSliceSpace16
    if (clutPoints && nInputs > 0) {
        cmsSliceSpace16(nInputs, clutPoints, dummy_sampler16, NULL);
    }

    // Test 2: cmsStageAllocCLutFloatGranular
    cmsStage *stageFloat = NULL;
    if (clutPoints && tableFloat && nInputs > 0) {
        // Ensure nInputs matches the number of dimensions in clutPoints
        stageFloat = cmsStageAllocCLutFloatGranular(g_context, clutPoints, nInputs, outputChan, tableFloat);
    }

    // Test 3: cmsStageSampleCLutFloat
    if (stageFloat) {
        cmsStageSampleCLutFloat(stageFloat, dummy_sampler_float, NULL, 0);
        cmsStageFree(stageFloat);
        stageFloat = NULL;
    }

    // Test 4: cmsStageAllocCLut16bit
    // For cmsStageAllocCLut16bit, the table size must match: gridPoints^nInputs * outputChan
    // We need to recalculate totalPoints based on gridPoints for this specific function
    uint64_t totalPoints16 = 1;
    for (uint32_t i = 0; i < nInputs; i++) {
        totalPoints16 *= gridPoints;
        if (totalPoints16 > 1000000) {
            totalPoints16 = 1000000;
            break;
        }
    }
    
    cmsStage *stage16 = NULL;
    if (table16 && gridPoints > 0) {
        // Ensure table16 has enough data for the expected grid size
        size_t expectedSize16 = totalPoints16 * outputChan * sizeof(cmsUInt16Number);
        if (table16Size >= expectedSize16) {
            stage16 = cmsStageAllocCLut16bit(g_context, gridPoints, nInputs, outputChan, table16);
        }
    }

    // Test 5: cmsStageSampleCLut16bit
    if (stage16) {
        cmsStageSampleCLut16bit(stage16, dummy_sampler16, NULL, 0);
        cmsStageFree(stage16);
        stage16 = NULL;
    }

    // Test 6: cmsSliceSpaceFloat
    if (clutPoints && nInputs > 0) {
        cmsSliceSpaceFloat(nInputs, clutPoints, dummy_sampler_float, NULL);
    }

    // Cleanup allocated memory
    if (clutPoints) free(clutPoints);
    if (table16) free(table16);
    if (tableFloat) free(tableFloat);

    return 0;
}