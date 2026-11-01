// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_stages(cmsStage* stages[], int count) {
    for (int i = 0; i < count; i++) {
        if (stages[i] != NULL) {
            cmsStageFree(stages[i]);
        }
    }
}

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Extract parameters from input
    uint32_t param1 = *(const uint32_t*)Data;
    uint32_t param2 = (Size > 4) ? *(const uint32_t*)(Data + 4) : 0;
    uint32_t param3 = (Size > 8) ? *(const uint32_t*)(Data + 8) : 0;
    uint32_t param4 = (Size > 12) ? *(const uint32_t*)(Data + 12) : 0;

    // Limit parameters to reasonable ranges to avoid crashes or excessive memory usage
    if (param1 > 256) param1 = 256;
    if (param2 > 16) param2 = 16;
    if (param3 > 16) param3 = 16;
    if (param4 > 16) param4 = 16;

    cmsStage* stages[5] = {NULL, NULL, NULL, NULL, NULL};
    int stage_count = 0;

    // Test cmsStageAllocIdentity
    if (param2 > 0 && param2 <= 16) {
        stages[stage_count++] = cmsStageAllocIdentity(ctx, param2);
    }

    // Test cmsStageAllocCLutFloat
    if (param1 > 0 && param1 <= 256 && param2 > 0 && param3 > 0 && param2 <= 16 && param3 <= 16) {
        size_t table_size = param1 * param1 * param2 * param3 * sizeof(cmsFloat32Number);
        if (Size >= 16 + table_size) {
            const cmsFloat32Number* table = (const cmsFloat32Number*)(Data + 16);
            stages[stage_count++] = cmsStageAllocCLutFloat(ctx, param1, param2, param3, table);
        }
    }

    // Test cmsStageAllocCLut16bit
    if (param1 > 0 && param1 <= 256 && param2 > 0 && param3 > 0 && param2 <= 16 && param3 <= 16) {
        size_t table_size = param1 * param1 * param2 * param3 * sizeof(cmsUInt16Number);
        if (Size >= 16 + table_size) {
            const cmsUInt16Number* table = (const cmsUInt16Number*)(Data + 16);
            stages[stage_count++] = cmsStageAllocCLut16bit(ctx, param1, param2, param3, table);
        }
    }

    // Test cmsStageAllocCLutFloatGranular
    if (param1 > 0 && param1 <= 256 && param2 > 0 && param3 > 0 && param2 <= 16 && param3 <= 16) {
        cmsUInt32Number clutPoints[16];
        for (int i = 0; i < param2 && i < 16; i++) {
            clutPoints[i] = param1;
        }
        size_t table_size = param1 * param1 * param2 * param3 * sizeof(cmsFloat32Number);
        if (Size >= 16 + table_size) {
            const cmsFloat32Number* table = (const cmsFloat32Number*)(Data + 16);
            stages[stage_count++] = cmsStageAllocCLutFloatGranular(ctx, clutPoints, param2, param3, table);
        }
    }

    // Test cmsStageInputChannels and cmsStageOutputChannels
    for (int i = 0; i < stage_count; i++) {
        if (stages[i] != NULL) {
            cmsUInt32Number input_channels = cmsStageInputChannels(stages[i]);
            cmsUInt32Number output_channels = cmsStageOutputChannels(stages[i]);
            (void)input_channels; // Suppress unused variable warning
            (void)output_channels;
        }
    }

    // Cleanup
    cleanup_stages(stages, stage_count);
    cleanup_context(ctx);

    return 0;
}