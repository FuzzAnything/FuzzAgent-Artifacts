// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void free_test_context(cmsContext ctx) {
    cmsDeleteContext(ctx);
}

static cmsStage* create_test_stage(cmsContext ctx) {
    // Create a simple identity matrix stage for testing
    cmsFloat64Number identity[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    return cmsStageAllocMatrix(ctx, 3, 3, identity, NULL);
}

static cmsUInt32Number parse_uint32(const uint8_t* data, size_t size, size_t* offset) {
    cmsUInt32Number value = 0;
    if (*offset + 4 <= size) {
        value = *(cmsUInt32Number*)(data + *offset);
        *offset += 4;
    }
    return value;
}

static void fill_table_float(cmsFloat32Number* table, size_t table_size, const uint8_t* data, size_t size, size_t* offset) {
    for (size_t i = 0; i < table_size && *offset + 4 <= size; i++) {
        table[i] = *(cmsFloat32Number*)(data + *offset);
        *offset += 4;
    }
}

static void fill_table_16bit(cmsUInt16Number* table, size_t table_size, const uint8_t* data, size_t size, size_t* offset) {
    for (size_t i = 0; i < table_size && *offset + 2 <= size; i++) {
        table[i] = *(cmsUInt16Number*)(data + *offset);
        *offset += 2;
    }
}

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    cmsContext ctx = create_test_context();
    if (!ctx) return 0;
    
    size_t offset = 0;
    
    // Test cmsStageInputChannels and cmsStageOutputChannels
    cmsStage* test_stage = create_test_stage(ctx);
    if (test_stage) {
        cmsUInt32Number in_channels = cmsStageInputChannels(test_stage);
        cmsUInt32Number out_channels = cmsStageOutputChannels(test_stage);
        (void)in_channels;
        (void)out_channels;
        cmsStageFree(test_stage);
    }
    
    // Parse parameters from fuzzer input
    cmsUInt32Number nGridPoints = parse_uint32(Data, Size, &offset) % 10 + 2;
    cmsUInt32Number inputChan = parse_uint32(Data, Size, &offset) % 8 + 1;
    cmsUInt32Number outputChan = parse_uint32(Data, Size, &offset) % 8 + 1;
    
    // Test cmsStageAllocCLutFloat
    size_t table_size_float = 1;
    for (cmsUInt32Number i = 0; i < inputChan; i++) {
        table_size_float *= nGridPoints;
    }
    table_size_float *= outputChan;
    
    if (table_size_float < 1000000) {
        cmsFloat32Number* table_float = (cmsFloat32Number*)calloc(table_size_float, sizeof(cmsFloat32Number));
        if (table_float) {
            fill_table_float(table_float, table_size_float, Data, Size, &offset);
            cmsStage* stage_float = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, table_float);
            if (stage_float) {
                cmsStageFree(stage_float);
            }
            free(table_float);
        }
    }
    
    // Test cmsStageAllocCLut16bit
    size_t table_size_16bit = 1;
    for (cmsUInt32Number i = 0; i < inputChan; i++) {
        table_size_16bit *= nGridPoints;
    }
    table_size_16bit *= outputChan;
    
    if (table_size_16bit < 1000000) {
        cmsUInt16Number* table_16bit = (cmsUInt16Number*)calloc(table_size_16bit, sizeof(cmsUInt16Number));
        if (table_16bit) {
            fill_table_16bit(table_16bit, table_size_16bit, Data, Size, &offset);
            cmsStage* stage_16bit = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table_16bit);
            if (stage_16bit) {
                cmsStageFree(stage_16bit);
            }
            free(table_16bit);
        }
    }
    
    // Test cmsStageAllocCLutFloatGranular
    cmsUInt32Number* clutPoints = (cmsUInt32Number*)malloc(inputChan * sizeof(cmsUInt32Number));
    if (clutPoints) {
        for (cmsUInt32Number i = 0; i < inputChan; i++) {
            clutPoints[i] = (parse_uint32(Data, Size, &offset) % 8) + 2;
        }
        
        size_t table_size_granular = 1;
        for (cmsUInt32Number i = 0; i < inputChan; i++) {
            table_size_granular *= clutPoints[i];
        }
        table_size_granular *= outputChan;
        
        if (table_size_granular < 1000000) {
            cmsFloat32Number* table_granular = (cmsFloat32Number*)calloc(table_size_granular, sizeof(cmsFloat32Number));
            if (table_granular) {
                fill_table_float(table_granular, table_size_granular, Data, Size, &offset);
                cmsStage* stage_granular = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, table_granular);
                if (stage_granular) {
                    cmsStageFree(stage_granular);
                }
                free(table_granular);
            }
        }
        
        // Test cmsStageAllocCLut16bitGranular
        if (table_size_granular < 1000000) {
            cmsUInt16Number* table_16bit_granular = (cmsUInt16Number*)calloc(table_size_granular, sizeof(cmsUInt16Number));
            if (table_16bit_granular) {
                fill_table_16bit(table_16bit_granular, table_size_granular, Data, Size, &offset);
                cmsStage* stage_16bit_granular = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table_16bit_granular);
                if (stage_16bit_granular) {
                    cmsStageFree(stage_16bit_granular);
                }
                free(table_16bit_granular);
            }
        }
        
        free(clutPoints);
    }
    
    free_test_context(ctx);
    return 0;
}