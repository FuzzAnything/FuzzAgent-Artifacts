// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
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

static void cleanup_test_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsStage* create_test_stage_float(cmsContext ctx, cmsUInt32Number nGridPoints, 
                                         cmsUInt32Number inputChan, cmsUInt32Number outputChan,
                                         const uint8_t* data, size_t size) {
    if (!ctx || nGridPoints == 0 || inputChan == 0 || outputChan == 0) {
        return NULL;
    }
    
    size_t table_size = nGridPoints * inputChan * outputChan;
    cmsFloat32Number* table = NULL;
    
    if (table_size > 0 && table_size <= 1024) {
        table = (cmsFloat32Number*)malloc(table_size * sizeof(cmsFloat32Number));
        if (!table) {
            return NULL;
        }
        
        for (size_t i = 0; i < table_size && i * sizeof(cmsFloat32Number) < size; i++) {
            cmsFloat32Number val = 0.0f;
            size_t bytes_to_copy = sizeof(cmsFloat32Number);
            if (i * sizeof(cmsFloat32Number) + bytes_to_copy > size) {
                bytes_to_copy = size - i * sizeof(cmsFloat32Number);
            }
            memcpy(&val, data + i * sizeof(cmsFloat32Number), bytes_to_copy);
            table[i] = val;
        }
    }
    
    cmsStage* stage = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, table);
    
    if (table) {
        free(table);
    }
    
    return stage;
}

static cmsStage* create_test_stage_16bit(cmsContext ctx, cmsUInt32Number nGridPoints,
                                         cmsUInt32Number inputChan, cmsUInt32Number outputChan,
                                         const uint8_t* data, size_t size) {
    if (!ctx || nGridPoints == 0 || inputChan == 0 || outputChan == 0) {
        return NULL;
    }
    
    size_t table_size = nGridPoints * inputChan * outputChan;
    cmsUInt16Number* table = NULL;
    
    if (table_size > 0 && table_size <= 1024) {
        table = (cmsUInt16Number*)malloc(table_size * sizeof(cmsUInt16Number));
        if (!table) {
            return NULL;
        }
        
        for (size_t i = 0; i < table_size && i * sizeof(cmsUInt16Number) < size; i++) {
            cmsUInt16Number val = 0;
            size_t bytes_to_copy = sizeof(cmsUInt16Number);
            if (i * sizeof(cmsUInt16Number) + bytes_to_copy > size) {
                bytes_to_copy = size - i * sizeof(cmsUInt16Number);
            }
            memcpy(&val, data + i * sizeof(cmsUInt16Number), bytes_to_copy);
            table[i] = val;
        }
    }
    
    cmsStage* stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table);
    
    if (table) {
        free(table);
    }
    
    return stage;
}

static cmsStage* create_test_stage_float_granular(cmsContext ctx, const cmsUInt32Number clutPoints[],
                                                  cmsUInt32Number inputChan, cmsUInt32Number outputChan,
                                                  const uint8_t* data, size_t size) {
    if (!ctx || !clutPoints || inputChan == 0 || outputChan == 0) {
        return NULL;
    }
    
    size_t table_size = 1;
    for (cmsUInt32Number i = 0; i < inputChan; i++) {
        if (clutPoints[i] == 0) {
            return NULL;
        }
        table_size *= clutPoints[i];
    }
    table_size *= outputChan;
    
    cmsFloat32Number* table = NULL;
    
    if (table_size > 0 && table_size <= 1024) {
        table = (cmsFloat32Number*)malloc(table_size * sizeof(cmsFloat32Number));
        if (!table) {
            return NULL;
        }
        
        for (size_t i = 0; i < table_size && i * sizeof(cmsFloat32Number) < size; i++) {
            cmsFloat32Number val = 0.0f;
            size_t bytes_to_copy = sizeof(cmsFloat32Number);
            if (i * sizeof(cmsFloat32Number) + bytes_to_copy > size) {
                bytes_to_copy = size - i * sizeof(cmsFloat32Number);
            }
            memcpy(&val, data + i * sizeof(cmsFloat32Number), bytes_to_copy);
            table[i] = val;
        }
    }
    
    cmsStage* stage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, table);
    
    if (table) {
        free(table);
    }
    
    return stage;
}

static cmsStage* create_test_stage_16bit_granular(cmsContext ctx, const cmsUInt32Number clutPoints[],
                                                  cmsUInt32Number inputChan, cmsUInt32Number outputChan,
                                                  const uint8_t* data, size_t size) {
    if (!ctx || !clutPoints || inputChan == 0 || outputChan == 0) {
        return NULL;
    }
    
    size_t table_size = 1;
    for (cmsUInt32Number i = 0; i < inputChan; i++) {
        if (clutPoints[i] == 0) {
            return NULL;
        }
        table_size *= clutPoints[i];
    }
    table_size *= outputChan;
    
    cmsUInt16Number* table = NULL;
    
    if (table_size > 0 && table_size <= 1024) {
        table = (cmsUInt16Number*)malloc(table_size * sizeof(cmsUInt16Number));
        if (!table) {
            return NULL;
        }
        
        for (size_t i = 0; i < table_size && i * sizeof(cmsUInt16Number) < size; i++) {
            cmsUInt16Number val = 0;
            size_t bytes_to_copy = sizeof(cmsUInt16Number);
            if (i * sizeof(cmsUInt16Number) + bytes_to_copy > size) {
                bytes_to_copy = size - i * sizeof(cmsUInt16Number);
            }
            memcpy(&val, data + i * sizeof(cmsUInt16Number), bytes_to_copy);
            table[i] = val;
        }
    }
    
    cmsStage* stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table);
    
    if (table) {
        free(table);
    }
    
    return stage;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    cmsContext ctx = create_test_context();
    if (!ctx) {
        return 0;
    }
    
    cmsUInt32Number test_case = Data[0] % 4;
    size_t data_offset = 1;
    
    switch (test_case) {
        case 0: {
            cmsUInt32Number nGridPoints = (Data[data_offset] % 16) + 2;
            data_offset = (data_offset + 1) % Size;
            cmsUInt32Number inputChan = (Data[data_offset] % 8) + 1;
            data_offset = (data_offset + 1) % Size;
            cmsUInt32Number outputChan = (Data[data_offset] % 8) + 1;
            data_offset = (data_offset + 1) % Size;
            
            cmsStage* stage = create_test_stage_float(ctx, nGridPoints, inputChan, outputChan, 
                                                     Data + data_offset, Size - data_offset);
            if (stage) {
                cmsStageInputChannels(stage);
                cmsStageOutputChannels(stage);
                cmsStageFree(stage);
            }
            break;
        }
        
        case 1: {
            cmsUInt32Number nGridPoints = (Data[data_offset] % 16) + 2;
            data_offset = (data_offset + 1) % Size;
            cmsUInt32Number inputChan = (Data[data_offset] % 8) + 1;
            data_offset = (data_offset + 1) % Size;
            cmsUInt32Number outputChan = (Data[data_offset] % 8) + 1;
            data_offset = (data_offset + 1) % Size;
            
            cmsStage* stage = create_test_stage_16bit(ctx, nGridPoints, inputChan, outputChan,
                                                     Data + data_offset, Size - data_offset);
            if (stage) {
                cmsStageInputChannels(stage);
                cmsStageOutputChannels(stage);
                cmsStageFree(stage);
            }
            break;
        }
        
        case 2: {
            cmsUInt32Number inputChan = (Data[data_offset] % 4) + 1;
            data_offset = (data_offset + 1) % Size;
            cmsUInt32Number outputChan = (Data[data_offset] % 4) + 1;
            data_offset = (data_offset + 1) % Size;
            
            cmsUInt32Number clutPoints[4];
            for (cmsUInt32Number i = 0; i < inputChan; i++) {
                clutPoints[i] = (Data[data_offset] % 8) + 2;
                data_offset = (data_offset + 1) % Size;
            }
            
            cmsStage* stage = create_test_stage_float_granular(ctx, clutPoints, inputChan, outputChan,
                                                              Data + data_offset, Size - data_offset);
            if (stage) {
                cmsStageInputChannels(stage);
                cmsStageOutputChannels(stage);
                cmsStageFree(stage);
            }
            break;
        }
        
        case 3: {
            cmsUInt32Number inputChan = (Data[data_offset] % 4) + 1;
            data_offset = (data_offset + 1) % Size;
            cmsUInt32Number outputChan = (Data[data_offset] % 4) + 1;
            data_offset = (data_offset + 1) % Size;
            
            cmsUInt32Number clutPoints[4];
            for (cmsUInt32Number i = 0; i < inputChan; i++) {
                clutPoints[i] = (Data[data_offset] % 8) + 2;
                data_offset = (data_offset + 1) % Size;
            }
            
            cmsStage* stage = create_test_stage_16bit_granular(ctx, clutPoints, inputChan, outputChan,
                                                              Data + data_offset, Size - data_offset);
            if (stage) {
                cmsStageInputChannels(stage);
                cmsStageOutputChannels(stage);
                cmsStageFree(stage);
            }
            break;
        }
    }
    
    cleanup_test_context(ctx);
    return 0;
}