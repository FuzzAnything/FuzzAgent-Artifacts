// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static cmsContext create_context(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;
    return ctx;
}

static void cleanup_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static void cleanup_stage(cmsStage *stage) {
    if (stage) {
        cmsStageFree(stage);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = create_context(Data, Size);
    if (!ctx) return 0;
    
    // Extract parameters from fuzzing data
    uint8_t *ptr = (uint8_t *)Data;
    uint32_t inputChan = 1;
    uint32_t outputChan = 1;
    uint32_t nGridPoints = 2;
    uint32_t clutPoints[4] = {2, 2, 2, 2};
    
    if (Size >= 4) {
        inputChan = ((uint32_t)ptr[0]) % 4 + 1; // 1-4 channels
        outputChan = ((uint32_t)ptr[1]) % 4 + 1; // 1-4 channels
        nGridPoints = ((uint32_t)ptr[2]) % 10 + 1; // 1-10 grid points
        clutPoints[0] = ((uint32_t)ptr[3]) % 10 + 1;
        if (Size >= 5) clutPoints[1] = ((uint32_t)ptr[4]) % 10 + 1;
        if (Size >= 6) clutPoints[2] = ((uint32_t)ptr[5]) % 10 + 1;
        if (Size >= 7) clutPoints[3] = ((uint32_t)ptr[6]) % 10 + 1;
    }
    
    // Limit channels to reasonable values (max 4 for lcms)
    if (inputChan > 4) inputChan = 4;
    if (outputChan > 4) outputChan = 4;
    
    // Calculate table sizes carefully to avoid overflow
    size_t totalPoints = 1;
    for (int i = 0; i < inputChan && i < 4; i++) {
        if (clutPoints[i] > 100) clutPoints[i] = 100; // Cap grid points
        if (totalPoints > 1000000 / clutPoints[i]) {
            totalPoints = 0; // Overflow protection
            break;
        }
        totalPoints *= clutPoints[i];
    }
    
    if (totalPoints == 0) return 0; // Skip if overflow detected
    
    // Allocate tables
    cmsFloat32Number *floatTable = NULL;
    cmsUInt16Number *uint16Table = NULL;
    
    size_t floatSize = totalPoints * outputChan * sizeof(cmsFloat32Number);
    size_t uint16Size = totalPoints * outputChan * sizeof(cmsUInt16Number);
    
    // Safety check for allocation size
    if (floatSize < 100000000 && floatSize > 0) {
        floatTable = (cmsFloat32Number *)malloc(floatSize);
        if (floatTable) {
            memset(floatTable, 0, floatSize);
            // Fill with some pattern based on input data
            size_t fillLen = (Size > totalPoints * outputChan) ? totalPoints * outputChan : Size;
            for (size_t i = 0; i < fillLen; i++) {
                floatTable[i] = (float)((ptr[i] % 100) / 100.0);
            }
            // Fill remaining with 0.5
            for (size_t i = fillLen; i < totalPoints * outputChan; i++) {
                floatTable[i] = 0.5f;
            }
        }
    }
    
    if (uint16Size < 100000000 && uint16Size > 0) {
        uint16Table = (cmsUInt16Number *)malloc(uint16Size);
        if (uint16Table) {
            memset(uint16Table, 0, uint16Size);
            // Fill with some pattern based on input data
            size_t fillLen = (Size > totalPoints * outputChan) ? totalPoints * outputChan : Size;
            for (size_t i = 0; i < fillLen; i++) {
                uint16Table[i] = (uint16_t)(ptr[i] * 255);
            }
            // Fill remaining with 32768 (midpoint)
            for (size_t i = fillLen; i < totalPoints * outputChan; i++) {
                uint16Table[i] = 32768;
            }
        }
    }
    
    cmsStage *stage = NULL;
    
    // Test cmsStageAllocIdentity
    stage = cmsStageAllocIdentity(ctx, inputChan);
    if (stage) {
        cmsUInt32Number channels = cmsStageInputChannels(stage);
        (void)channels; // Use the return value
        cleanup_stage(stage);
        stage = NULL;
    }
    
    // Test cmsStageAllocCLutFloat
    // Ensure nGridPoints matches the expected grid size for uniform CLUT
    // The library expects nGridPoints^inputChan * outputChan elements
    if (floatTable) {
        // Re-calculate expected size for uniform grid to ensure table is large enough
        size_t uniformTotalPoints = 1;
        for (int i = 0; i < inputChan; i++) {
            if (uniformTotalPoints > 1000000 / nGridPoints) {
                uniformTotalPoints = 0;
                break;
            }
            uniformTotalPoints *= nGridPoints;
        }
        
        if (uniformTotalPoints > 0) {
            size_t requiredFloatSize = uniformTotalPoints * outputChan * sizeof(cmsFloat32Number);
            if (requiredFloatSize <= floatSize) {
                stage = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, floatTable);
                if (stage) {
                    cmsUInt32Number channels = cmsStageInputChannels(stage);
                    (void)channels;
                    cleanup_stage(stage);
                    stage = NULL;
                }
            }
        }
    }
    
    // Test cmsStageAllocCLutFloatGranular
    // Ensure clutPoints match the expected grid size for granular CLUT
    if (floatTable) {
        size_t granularTotalPoints = 1;
        for (int i = 0; i < inputChan; i++) {
            if (granularTotalPoints > 1000000 / clutPoints[i]) {
                granularTotalPoints = 0;
                break;
            }
            granularTotalPoints *= clutPoints[i];
        }
        
        if (granularTotalPoints > 0) {
            size_t requiredFloatSize = granularTotalPoints * outputChan * sizeof(cmsFloat32Number);
            if (requiredFloatSize <= floatSize) {
                stage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, floatTable);
                if (stage) {
                    cmsUInt32Number channels = cmsStageInputChannels(stage);
                    (void)channels;
                    cleanup_stage(stage);
                    stage = NULL;
                }
            }
        }
    }
    
    // Test cmsStageAllocCLut16bit
    if (uint16Table) {
        size_t uniformTotalPoints = 1;
        for (int i = 0; i < inputChan; i++) {
            if (uniformTotalPoints > 1000000 / nGridPoints) {
                uniformTotalPoints = 0;
                break;
            }
            uniformTotalPoints *= nGridPoints;
        }
        
        if (uniformTotalPoints > 0) {
            size_t requiredUint16Size = uniformTotalPoints * outputChan * sizeof(cmsUInt16Number);
            if (requiredUint16Size <= uint16Size) {
                stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, uint16Table);
                if (stage) {
                    cmsUInt32Number channels = cmsStageInputChannels(stage);
                    (void)channels;
                    cleanup_stage(stage);
                    stage = NULL;
                }
            }
        }
    }
    
    // Test cmsStageAllocCLut16bitGranular
    if (uint16Table) {
        size_t granularTotalPoints = 1;
        for (int i = 0; i < inputChan; i++) {
            if (granularTotalPoints > 1000000 / clutPoints[i]) {
                granularTotalPoints = 0;
                break;
            }
            granularTotalPoints *= clutPoints[i];
        }
        
        if (granularTotalPoints > 0) {
            size_t requiredUint16Size = granularTotalPoints * outputChan * sizeof(cmsUInt16Number);
            if (requiredUint16Size <= uint16Size) {
                stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, uint16Table);
                if (stage) {
                    cmsUInt32Number channels = cmsStageInputChannels(stage);
                    (void)channels;
                    cleanup_stage(stage);
                    stage = NULL;
                }
            }
        }
    }
    
    // Cleanup
    free(floatTable);
    free(uint16Table);
    cleanup_context(ctx);
    
    return 0;
}