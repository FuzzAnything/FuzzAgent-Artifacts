// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void free_stage(cmsStage* stage) {
    if (stage) {
        cmsStageFree(stage);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Extract parameters from input data
    uint32_t param1 = *(const uint32_t*)(Data);
    uint32_t param2 = Size > 4 ? *(const uint32_t*)(Data + 4) : 0;
    uint32_t param3 = Size > 8 ? *(const uint32_t*)(Data + 8) : 0;
    uint32_t param4 = Size > 12 ? *(const uint32_t*)(Data + 12) : 0;

    // Limit parameters to reasonable ranges to avoid excessive allocation
    if (param1 > 10) param1 = 10;
    if (param2 > 10) param2 = 10;
    if (param3 > 10) param3 = 10;
    if (param4 > 10) param4 = 10;

    cmsStage *stage = NULL;

    // Fuzz cmsStageAllocIdentity
    if (param1 <= 4) {
        stage = cmsStageAllocIdentity(ctx, param1);
        if (stage) {
            cmsStageInputChannels(stage);
            cmsStageOutputChannels(stage);
            free_stage(stage);
            stage = NULL;
        }
    }

    // Fuzz cmsStageAllocMatrix
    if (param1 <= 4 && param2 <= 4) {
        cmsFloat64Number matrix[16] = {0};
        cmsFloat64Number offset[4] = {0};
        // Initialize with some values from input if available
        if (Size > 16) {
            memcpy(matrix, Data + 16, sizeof(matrix));
            if (Size > 32) {
                memcpy(offset, Data + 32, sizeof(offset));
            }
        }
        stage = cmsStageAllocMatrix(ctx, param1, param2, matrix, offset);
        if (stage) {
            cmsStageInputChannels(stage);
            cmsStageOutputChannels(stage);
            free_stage(stage);
            stage = NULL;
        }
    }

    // Fuzz cmsStageAllocCLut16bit
    if (param1 <= 4 && param2 <= 4 && param3 <= 4) {
        uint32_t grid = param4;
        if (grid == 0) grid = 2;
        if (grid > 10) grid = 10;
        
        uint32_t table_size = grid * grid * grid * param2 * 2; // 16-bit values
        if (table_size > 10000) table_size = 10000; // Limit size
        
        cmsUInt16Number *table = NULL;
        if (Size > 16) {
            size_t available = Size - 16;
            size_t needed = table_size;
            if (available < needed) needed = available;
            
            table = (cmsUInt16Number*)malloc(needed);
            if (table) {
                memcpy(table, Data + 16, needed);
                // Pad with zeros if needed
                if (needed < table_size) {
                    memset(table + (needed/2), 0, table_size - needed);
                }
                
                stage = cmsStageAllocCLut16bit(ctx, grid, param1, param2, table);
                if (stage) {
                    cmsStageInputChannels(stage);
                    cmsStageOutputChannels(stage);
                    free_stage(stage);
                    stage = NULL;
                }
                free(table);
            }
        }
    }

    // Fuzz cmsStageAllocCLut16bitGranular
    if (param1 <= 4 && param2 <= 4 && param3 <= 4) {
        uint32_t points[4] = {1, 1, 1, 1};
        if (Size > 16) {
            for (int i = 0; i < 4 && 16 + i*4 < Size; i++) {
                points[i] = *(const uint32_t*)(Data + 16 + i*4);
                if (points[i] == 0) points[i] = 2;
                if (points[i] > 10) points[i] = 10;
            }
        }
        
        uint32_t table_size = 1;
        for (int i = 0; i < param1; i++) {
            table_size *= points[i];
        }
        table_size *= param2 * 2; // 16-bit values
        if (table_size > 10000) table_size = 10000;
        
        cmsUInt16Number *table = NULL;
        if (Size > 16) {
            size_t available = Size - 16;
            size_t needed = table_size;
            if (available < needed) needed = available;
            
            table = (cmsUInt16Number*)malloc(needed);
            if (table) {
                memcpy(table, Data + 16, needed);
                if (needed < table_size) {
                    memset(table + (needed/2), 0, table_size - needed);
                }
                
                stage = cmsStageAllocCLut16bitGranular(ctx, points, param1, param2, table);
                if (stage) {
                    cmsStageInputChannels(stage);
                    cmsStageOutputChannels(stage);
                    free_stage(stage);
                    stage = NULL;
                }
                free(table);
            }
        }
    }

    cmsDeleteContext(ctx);
    return 0;
}