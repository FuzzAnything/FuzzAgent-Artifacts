// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fill_table_float(const uint8_t *data, size_t size, cmsFloat32Number *table, size_t table_size) {
    for (size_t i = 0; i < table_size; i++) {
        if (size >= sizeof(cmsFloat32Number)) {
            memcpy(&table[i], data, sizeof(cmsFloat32Number));
            data += sizeof(cmsFloat32Number);
            size -= sizeof(cmsFloat32Number);
        } else {
            table[i] = 0.0f;
        }
    }
}

static void fill_table_16bit(const uint8_t *data, size_t size, cmsUInt16Number *table, size_t table_size) {
    for (size_t i = 0; i < table_size; i++) {
        if (size >= sizeof(cmsUInt16Number)) {
            memcpy(&table[i], data, sizeof(cmsUInt16Number));
            data += sizeof(cmsUInt16Number);
            size -= sizeof(cmsUInt16Number);
        } else {
            table[i] = 0;
        }
    }
}

static size_t compute_table_size(const cmsUInt32Number *clutPoints, cmsUInt32Number inputChan, cmsUInt32Number outputChan) {
    size_t total = 1;
    for (cmsUInt32Number i = 0; i < inputChan; i++) {
        total *= clutPoints[i];
    }
    return total * outputChan;
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Extract parameters from fuzzer input
    const uint8_t *ptr = Data;
    size_t remaining = Size;

    // Read inputChan and outputChan (1-16 each)
    cmsUInt32Number inputChan = (ptr[0] % 16) + 1;
    cmsUInt32Number outputChan = (ptr[1] % 16) + 1;
    cmsUInt32Number nGridPoints = (ptr[2] % 64) + 2; // 2-65
    ptr += 3;
    remaining -= 3;

    // Test cmsStageAllocCLutFloat
    if (remaining >= sizeof(cmsFloat32Number)) {
        size_t table_size = 1;
        for (cmsUInt32Number i = 0; i < inputChan; i++) {
            table_size *= nGridPoints;
        }
        table_size *= outputChan;
        
        cmsFloat32Number *float_table = (cmsFloat32Number*)malloc(table_size * sizeof(cmsFloat32Number));
        if (float_table) {
            fill_table_float(ptr, remaining, float_table, table_size);
            cmsStage *stage = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, float_table);
            if (stage) {
                cmsUInt32Number in = cmsStageInputChannels(stage);
                cmsUInt32Number out = cmsStageOutputChannels(stage);
                (void)in; (void)out; // Use variables to avoid unused warnings
                cmsStageFree(stage);
            }
            free(float_table);
        }
    }

    // Reset pointer for next test
    ptr = Data + 3;
    remaining = Size - 3;

    // Test cmsStageAllocCLut16bit
    if (remaining >= sizeof(cmsUInt16Number)) {
        size_t table_size = 1;
        for (cmsUInt32Number i = 0; i < inputChan; i++) {
            table_size *= nGridPoints;
        }
        table_size *= outputChan;
        
        cmsUInt16Number *int16_table = (cmsUInt16Number*)malloc(table_size * sizeof(cmsUInt16Number));
        if (int16_table) {
            fill_table_16bit(ptr, remaining, int16_table, table_size);
            cmsStage *stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, int16_table);
            if (stage) {
                cmsUInt32Number in = cmsStageInputChannels(stage);
                cmsUInt32Number out = cmsStageOutputChannels(stage);
                (void)in; (void)out;
                cmsStageFree(stage);
            }
            free(int16_table);
        }
    }

    // Test cmsStageAllocCLutFloatGranular
    if (remaining >= inputChan * sizeof(cmsUInt32Number)) {
        cmsUInt32Number *clutPoints = (cmsUInt32Number*)malloc(inputChan * sizeof(cmsUInt32Number));
        if (clutPoints) {
            for (cmsUInt32Number i = 0; i < inputChan; i++) {
                clutPoints[i] = (ptr[i % remaining] % 64) + 2; // 2-65
            }
            
            size_t table_size = compute_table_size(clutPoints, inputChan, outputChan);
            cmsFloat32Number *float_table = (cmsFloat32Number*)malloc(table_size * sizeof(cmsFloat32Number));
            if (float_table) {
                fill_table_float(ptr + inputChan, remaining - inputChan, float_table, table_size);
                cmsStage *stage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, float_table);
                if (stage) {
                    cmsUInt32Number in = cmsStageInputChannels(stage);
                    cmsUInt32Number out = cmsStageOutputChannels(stage);
                    (void)in; (void)out;
                    cmsStageFree(stage);
                }
                free(float_table);
            }
            free(clutPoints);
        }
    }

    // Test cmsStageAllocCLut16bitGranular
    if (remaining >= inputChan * sizeof(cmsUInt32Number)) {
        cmsUInt32Number *clutPoints = (cmsUInt32Number*)malloc(inputChan * sizeof(cmsUInt32Number));
        if (clutPoints) {
            for (cmsUInt32Number i = 0; i < inputChan; i++) {
                clutPoints[i] = (ptr[i % remaining] % 64) + 2; // 2-65
            }
            
            size_t table_size = compute_table_size(clutPoints, inputChan, outputChan);
            cmsUInt16Number *int16_table = (cmsUInt16Number*)malloc(table_size * sizeof(cmsUInt16Number));
            if (int16_table) {
                fill_table_16bit(ptr + inputChan, remaining - inputChan, int16_table, table_size);
                cmsStage *stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, int16_table);
                if (stage) {
                    cmsUInt32Number in = cmsStageInputChannels(stage);
                    cmsUInt32Number out = cmsStageOutputChannels(stage);
                    (void)in; (void)out;
                    cmsStageFree(stage);
                }
                free(int16_table);
            }
            free(clutPoints);
        }
    }

    cmsDeleteContext(ctx);
    return 0;
}