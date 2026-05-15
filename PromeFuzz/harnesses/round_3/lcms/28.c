// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
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

static void cleanup_stage(cmsStage* stage) {
    if (stage) {
        cmsStageFree(stage);
    }
}

static cmsUInt16Number* create_test_table(cmsUInt32Number size, const uint8_t* data, size_t data_size) {
    if (size == 0) return NULL;
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(size * sizeof(cmsUInt16Number));
    if (!table) return NULL;
    
    for (cmsUInt32Number i = 0; i < size; i++) {
        if (i * 2 < data_size) {
            table[i] = (data[i * 2] << 8) | data[i * 2 + 1];
        } else {
            table[i] = (i * 257) & 0xFFFF; // Simple pattern if data runs out
        }
    }
    
    return table;
}

static cmsFloat64Number* create_test_matrix(cmsUInt32Number size, const uint8_t* data, size_t data_size) {
    if (size == 0) return NULL;
    
    cmsFloat64Number* matrix = (cmsFloat64Number*)malloc(size * sizeof(cmsFloat64Number));
    if (!matrix) return NULL;
    
    for (cmsUInt32Number i = 0; i < size; i++) {
        if (i < data_size) {
            matrix[i] = (cmsFloat64Number)data[i] / 255.0;
        } else {
            matrix[i] = (cmsFloat64Number)(i % 10) / 10.0;
        }
    }
    
    return matrix;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext context = create_test_context();
    if (!context) return 0;
    
    // Use first byte to determine which function to test
    uint8_t function_selector = Data[0] % 6;
    const uint8_t* func_data = Data + 1;
    size_t func_size = Size - 1;
    
    switch (function_selector) {
        case 0: { // cmsStageAllocCLut16bitGranular
            if (func_size < 8) break;
            
            cmsUInt32Number inputChan = (func_data[0] % 16) + 1;
            cmsUInt32Number outputChan = (func_data[1] % 16) + 1;
            
            cmsUInt32Number* clutPoints = (cmsUInt32Number*)malloc(inputChan * sizeof(cmsUInt32Number));
            if (!clutPoints) break;
            
            for (cmsUInt32Number i = 0; i < inputChan; i++) {
                clutPoints[i] = (func_data[2 + (i % (func_size - 2))] % 16) + 2;
            }
            
            cmsUInt32Number table_size = 1;
            for (cmsUInt32Number i = 0; i < inputChan; i++) {
                table_size *= clutPoints[i];
            }
            table_size *= outputChan;
            
            cmsUInt16Number* table = create_test_table(table_size, func_data, func_size);
            
            cmsStage* stage = cmsStageAllocCLut16bitGranular(context, clutPoints, inputChan, outputChan, table);
            
            if (stage) {
                // Test query functions on the created stage
                cmsUInt32Number in_channels = cmsStageInputChannels(stage);
                cmsUInt32Number out_channels = cmsStageOutputChannels(stage);
                (void)in_channels; // Use variables to avoid unused warnings
                (void)out_channels;
                
                cleanup_stage(stage);
            }
            
            free(clutPoints);
            free(table);
            break;
        }
        
        case 1: { // cmsStageAllocIdentity
            if (func_size < 1) break;
            
            cmsUInt32Number nChannels = (func_data[0] % 16) + 1;
            cmsStage* stage = cmsStageAllocIdentity(context, nChannels);
            
            if (stage) {
                cmsUInt32Number in_channels = cmsStageInputChannels(stage);
                cmsUInt32Number out_channels = cmsStageOutputChannels(stage);
                (void)in_channels;
                (void)out_channels;
                
                cleanup_stage(stage);
            }
            break;
        }
        
        case 2: { // cmsStageAllocMatrix
            if (func_size < 4) break;
            
            cmsUInt32Number Rows = (func_data[0] % 4) + 1;
            cmsUInt32Number Cols = (func_data[1] % 4) + 1;
            
            cmsUInt32Number matrix_size = Rows * Cols;
            cmsFloat64Number* matrix = create_test_matrix(matrix_size, func_data + 2, func_size - 2);
            
            cmsUInt32Number offset_size = Rows;
            cmsFloat64Number* offset = create_test_matrix(offset_size, func_data + 2 + (matrix_size % func_size), 
                                                         func_size - 2 - (matrix_size % func_size));
            
            cmsStage* stage = cmsStageAllocMatrix(context, Rows, Cols, matrix, offset);
            
            if (stage) {
                cmsUInt32Number in_channels = cmsStageInputChannels(stage);
                cmsUInt32Number out_channels = cmsStageOutputChannels(stage);
                (void)in_channels;
                (void)out_channels;
                
                cleanup_stage(stage);
            }
            
            free(matrix);
            free(offset);
            break;
        }
        
        case 3: { // cmsStageOutputChannels
            // First create a stage to test the query function
            if (func_size < 1) break;
            
            cmsUInt32Number nChannels = (func_data[0] % 16) + 1;
            cmsStage* stage = cmsStageAllocIdentity(context, nChannels);
            
            if (stage) {
                cmsUInt32Number out_channels = cmsStageOutputChannels(stage);
                (void)out_channels;
                
                cleanup_stage(stage);
            }
            break;
        }
        
        case 4: { // cmsStageAllocCLut16bit
            if (func_size < 4) break;
            
            cmsUInt32Number nGridPoints = (func_data[0] % 16) + 2;
            cmsUInt32Number inputChan = (func_data[1] % 16) + 1;
            cmsUInt32Number outputChan = (func_data[2] % 16) + 1;
            
            cmsUInt32Number table_size = 1;
            for (cmsUInt32Number i = 0; i < inputChan; i++) {
                table_size *= nGridPoints;
            }
            table_size *= outputChan;
            
            cmsUInt16Number* table = create_test_table(table_size, func_data + 3, func_size - 3);
            
            cmsStage* stage = cmsStageAllocCLut16bit(context, nGridPoints, inputChan, outputChan, table);
            
            if (stage) {
                cmsUInt32Number in_channels = cmsStageInputChannels(stage);
                cmsUInt32Number out_channels = cmsStageOutputChannels(stage);
                (void)in_channels;
                (void)out_channels;
                
                cleanup_stage(stage);
            }
            
            free(table);
            break;
        }
        
        case 5: { // cmsStageInputChannels
            // First create a stage to test the query function
            if (func_size < 1) break;
            
            cmsUInt32Number nChannels = (func_data[0] % 16) + 1;
            cmsStage* stage = cmsStageAllocIdentity(context, nChannels);
            
            if (stage) {
                cmsUInt32Number in_channels = cmsStageInputChannels(stage);
                (void)in_channels;
                
                cleanup_stage(stage);
            }
            break;
        }
    }
    
    cmsDeleteContext(context);
    return 0;
}