// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static cmsContext create_context(const uint8_t *Data, size_t Size) {
    if (Size > 0 && Data[0] % 2 == 0) {
        return cmsCreateContext(NULL, NULL);
    }
    return NULL;
}

static void cleanup_stage(cmsStage *stage) {
    if (stage) {
        cmsStageFree(stage);
    }
}

static void cleanup_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = create_context(Data, Size);
    cmsStage *stage = NULL;
    cmsStage *stage2 = NULL;
    cmsStage *stage3 = NULL;
    cmsStage *stage4 = NULL;
    cmsUInt32Number channels = 0;
    cmsUInt32Number gridPoints = 0;
    cmsUInt32Number inputChan = 0;
    cmsUInt32Number outputChan = 0;

    // Extract parameters from fuzz input with bounds checking
    if (Size >= 4) {
        channels = *(const uint32_t *)(Data);
        if (channels > 16) channels = 16; // Reasonable limit
    }
    if (Size >= 8) {
        gridPoints = *(const uint32_t *)(Data + 4);
        if (gridPoints > 10) gridPoints = 10; // Limit to prevent OOM
    }
    if (Size >= 12) {
        inputChan = *(const uint32_t *)(Data + 8);
        if (inputChan > 16) inputChan = 16;
    }
    if (Size >= 16) {
        outputChan = *(const uint32_t *)(Data + 12);
        if (outputChan > 16) outputChan = 16;
    }

    // Test 1: cmsStageAllocIdentity
    stage = cmsStageAllocIdentity(ctx, channels);
    if (stage) {
        cmsStageInputChannels(stage);
        cmsStageOutputChannels(stage);
        cleanup_stage(stage);
    }

    // Test 2: cmsStageAllocCLutFloat
    if (gridPoints > 0 && inputChan > 0 && outputChan > 0) {
        size_t tableSize = 1;
        for (uint32_t i = 0; i < inputChan; i++) tableSize *= gridPoints;
        tableSize *= outputChan;
        
        cmsFloat32Number *table = (cmsFloat32Number *)malloc(tableSize * sizeof(cmsFloat32Number));
        if (table) {
            memset(table, 0, tableSize * sizeof(cmsFloat32Number));
            stage2 = cmsStageAllocCLutFloat(ctx, gridPoints, inputChan, outputChan, table);
            if (stage2) {
                cmsStageInputChannels(stage2);
                cmsStageOutputChannels(stage2);
                cleanup_stage(stage2);
            }
            free(table);
        }
    }

    // Test 3: cmsStageAllocCLut16bit
    if (gridPoints > 0 && inputChan > 0 && outputChan > 0) {
        size_t tableSize = 1;
        for (uint32_t i = 0; i < inputChan; i++) tableSize *= gridPoints;
        tableSize *= outputChan;
        
        cmsUInt16Number *table16 = (cmsUInt16Number *)malloc(tableSize * sizeof(cmsUInt16Number));
        if (table16) {
            memset(table16, 0, tableSize * sizeof(cmsUInt16Number));
            stage3 = cmsStageAllocCLut16bit(ctx, gridPoints, inputChan, outputChan, table16);
            if (stage3) {
                cmsStageInputChannels(stage3);
                cmsStageOutputChannels(stage3);
                cleanup_stage(stage3);
            }
            free(table16);
        }
    }

    // Test 4: cmsStageAllocCLutFloatGranular
    if (gridPoints > 0 && inputChan > 0 && outputChan > 0 && inputChan <= 16) {
        cmsUInt32Number clutPoints[16];
        for (uint32_t i = 0; i < inputChan; i++) {
            clutPoints[i] = gridPoints;
        }
        
        size_t tableSize = 1;
        for (uint32_t i = 0; i < inputChan; i++) tableSize *= clutPoints[i];
        tableSize *= outputChan;
        
        cmsFloat32Number *tableGranular = (cmsFloat32Number *)malloc(tableSize * sizeof(cmsFloat32Number));
        if (tableGranular) {
            memset(tableGranular, 0, tableSize * sizeof(cmsFloat32Number));
            stage4 = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, tableGranular);
            if (stage4) {
                cmsStageInputChannels(stage4);
                cmsStageOutputChannels(stage4);
                cleanup_stage(stage4);
            }
            free(tableGranular);
        }
    }

    cleanup_context(ctx);
    return 0;
}