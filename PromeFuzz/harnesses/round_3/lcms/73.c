// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHANNELS 16
#define MAX_GRID_POINTS 256
#define MAX_TABLE_SIZE (MAX_GRID_POINTS * MAX_GRID_POINTS * MAX_GRID_POINTS * MAX_CHANNELS)

static cmsUInt16Number dummy_table_16[MAX_TABLE_SIZE];
static cmsFloat32Number dummy_table_float[MAX_TABLE_SIZE];

static void initialize_tables(void) {
    for (size_t i = 0; i < MAX_TABLE_SIZE; i++) {
        dummy_table_16[i] = (cmsUInt16Number)(i % 65536);
        dummy_table_float[i] = (cmsFloat32Number)(i % 256) / 255.0f;
    }
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    static int tables_initialized = 0;
    if (!tables_initialized) {
        initialize_tables();
        tables_initialized = 1;
    }

    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Use first few bytes to determine parameters
    size_t offset = 0;
    
    // Extract parameters from fuzzing input
    cmsUInt32Number inputChan = (Data[offset++] % MAX_CHANNELS) + 1;
    cmsUInt32Number outputChan = (Data[offset++] % MAX_CHANNELS) + 1;
    cmsUInt32Number nGridPoints = (Data[offset++] % (MAX_GRID_POINTS - 2)) + 2;
    
    // Create granular clutPoints array
    cmsUInt32Number clutPoints[MAX_CHANNELS];
    for (cmsUInt32Number i = 0; i < inputChan; i++) {
        if (offset < Size) {
            clutPoints[i] = (Data[offset++] % (MAX_GRID_POINTS - 2)) + 2;
        } else {
            clutPoints[i] = nGridPoints;
        }
    }

    // Test cmsStageAllocCLut16bitGranular
    cmsStage* stage1 = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, dummy_table_16);
    if (stage1) {
        cmsUInt32Number inChannels = cmsStageInputChannels(stage1);
        cmsUInt32Number outChannels = cmsStageOutputChannels(stage1);
        cmsStageFree(stage1);
    }

    // Test cmsStageAllocCLutFloatGranular
    cmsStage* stage2 = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, dummy_table_float);
    if (stage2) {
        cmsUInt32Number inChannels = cmsStageInputChannels(stage2);
        cmsUInt32Number outChannels = cmsStageOutputChannels(stage2);
        cmsStageFree(stage2);
    }

    // Test cmsStageAllocCLut16bit
    cmsStage* stage3 = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, dummy_table_16);
    if (stage3) {
        cmsUInt32Number inChannels = cmsStageInputChannels(stage3);
        cmsUInt32Number outChannels = cmsStageOutputChannels(stage3);
        cmsStageFree(stage3);
    }

    // Test cmsStageAllocCLutFloat
    cmsStage* stage4 = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, dummy_table_float);
    if (stage4) {
        cmsUInt32Number inChannels = cmsStageInputChannels(stage4);
        cmsUInt32Number outChannels = cmsStageOutputChannels(stage4);
        cmsStageFree(stage4);
    }

    cmsDeleteContext(ctx);
    return 0;
}