// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
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

static void fill_table_16bit(const uint8_t* data, size_t size, cmsUInt16Number* table, size_t table_size) {
    size_t i;
    for (i = 0; i < table_size && i * 2 + 1 < size; i++) {
        table[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
    for (; i < table_size; i++) {
        table[i] = 0;
    }
}

static void fill_table_float(const uint8_t* data, size_t size, cmsFloat32Number* table, size_t table_size) {
    size_t i;
    for (i = 0; i < table_size && i * 4 + 3 < size; i++) {
        uint32_t val = (data[i * 4] << 24) | (data[i * 4 + 1] << 16) | (data[i * 4 + 2] << 8) | data[i * 4 + 3];
        table[i] = *(cmsFloat32Number*)&val;
    }
    for (; i < table_size; i++) {
        table[i] = 0.0f;
    }
}

static cmsUInt32Number compute_table_size(const cmsUInt32Number* clutPoints, cmsUInt32Number inputChan, cmsUInt32Number outputChan) {
    cmsUInt32Number size = outputChan;
    cmsUInt32Number i;
    for (i = 0; i < inputChan; i++) {
        size *= clutPoints[i];
    }
    return size;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Extract parameters from fuzz input
    cmsUInt32Number inputChan = (Data[0] % 8) + 1;   // 1-8 channels
    cmsUInt32Number outputChan = (Data[1] % 8) + 1;  // 1-8 channels
    cmsUInt32Number nGridPoints = (Data[2] % 10) + 2; // 2-11 grid points
    cmsUInt32Number maxGridPoints = (Data[3] % 8) + 2; // 2-9 for granular
    
    const uint8_t* table_data = Data + 4;
    size_t table_data_size = Size - 4;

    // Test cmsStageAllocCLut16bit
    cmsUInt32Number table_size_16bit = 1;
    cmsUInt32Number i;
    for (i = 0; i < inputChan; i++) {
        table_size_16bit *= nGridPoints;
    }
    table_size_16bit *= outputChan;
    
    cmsUInt16Number* table_16bit = (cmsUInt16Number*)malloc(table_size_16bit * sizeof(cmsUInt16Number));
    if (table_16bit) {
        fill_table_16bit(table_data, table_data_size, table_16bit, table_size_16bit);
        
        cmsStage* stage16 = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table_16bit);
        if (stage16) {
            cmsUInt32Number in_ch = cmsStageInputChannels(stage16);
            cmsUInt32Number out_ch = cmsStageOutputChannels(stage16);
            (void)in_ch; (void)out_ch; // Use variables to avoid unused warnings
            cmsStageFree(stage16);
        }
        free(table_16bit);
    }

    // Test cmsStageAllocCLutFloat
    cmsFloat32Number* table_float = (cmsFloat32Number*)malloc(table_size_16bit * sizeof(cmsFloat32Number));
    if (table_float) {
        fill_table_float(table_data, table_data_size, table_float, table_size_16bit);
        
        cmsStage* stageFloat = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, table_float);
        if (stageFloat) {
            cmsUInt32Number in_ch = cmsStageInputChannels(stageFloat);
            cmsUInt32Number out_ch = cmsStageOutputChannels(stageFloat);
            (void)in_ch; (void)out_ch;
            cmsStageFree(stageFloat);
        }
        free(table_float);
    }

    // Test granular versions if we have enough data
    if (Size >= 4 + inputChan) {
        cmsUInt32Number* clutPoints = (cmsUInt32Number*)malloc(inputChan * sizeof(cmsUInt32Number));
        if (clutPoints) {
            for (i = 0; i < inputChan; i++) {
                clutPoints[i] = (Data[4 + i] % 8) + 2; // 2-9 grid points per channel
            }
            
            cmsUInt32Number table_size_granular = compute_table_size(clutPoints, inputChan, outputChan);
            
            // Test cmsStageAllocCLut16bitGranular
            cmsUInt16Number* table_16bit_granular = (cmsUInt16Number*)malloc(table_size_granular * sizeof(cmsUInt16Number));
            if (table_16bit_granular) {
                fill_table_16bit(table_data, table_data_size, table_16bit_granular, table_size_granular);
                
                cmsStage* stage16Granular = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table_16bit_granular);
                if (stage16Granular) {
                    cmsUInt32Number in_ch = cmsStageInputChannels(stage16Granular);
                    cmsUInt32Number out_ch = cmsStageOutputChannels(stage16Granular);
                    (void)in_ch; (void)out_ch;
                    cmsStageFree(stage16Granular);
                }
                free(table_16bit_granular);
            }
            
            // Test cmsStageAllocCLutFloatGranular
            cmsFloat32Number* table_float_granular = (cmsFloat32Number*)malloc(table_size_granular * sizeof(cmsFloat32Number));
            if (table_float_granular) {
                fill_table_float(table_data, table_data_size, table_float_granular, table_size_granular);
                
                cmsStage* stageFloatGranular = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, table_float_granular);
                if (stageFloatGranular) {
                    cmsUInt32Number in_ch = cmsStageInputChannels(stageFloatGranular);
                    cmsUInt32Number out_ch = cmsStageOutputChannels(stageFloatGranular);
                    (void)in_ch; (void)out_ch;
                    cmsStageFree(stageFloatGranular);
                }
                free(table_float_granular);
            }
            
            free(clutPoints);
        }
    }

    cmsDeleteContext(ctx);
    return 0;
}