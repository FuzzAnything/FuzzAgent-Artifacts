// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void free_stage(cmsStage* stage) {
    if (stage) {
        cmsStageFree(stage);
    }
}

int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    // Extract parameters from fuzz data
    uint32_t nGridPoints = Data[0] + 1; // Avoid 0
    uint32_t inputChan = Data[1] % 5;   // 0-4
    uint32_t outputChan = Data[2] % 5;  // 0-4
    uint32_t rows = Data[3] % 5;
    uint32_t cols = (Size > 4) ? (Data[4] % 5) : 0;

    // Limit grid points to prevent huge allocations
    if (nGridPoints > 16) nGridPoints = 16;
    if (inputChan > 4) inputChan = 4;
    if (outputChan > 4) outputChan = 4;

    // Allocate buffers based on maximum dimensions (4) for CLUTs as per instructions
    size_t max_input_dim = 4;
    size_t table_size_16 = 1;
    for (int i = 0; i < max_input_dim; i++) table_size_16 *= nGridPoints;
    table_size_16 *= outputChan;
    
    size_t table_size_float = table_size_16; // Same count, different type size

    cmsUInt16Number* table_16 = NULL;
    cmsFloat32Number* table_float = NULL;
    cmsFloat64Number* matrix = NULL;
    cmsFloat64Number* offset = NULL;
    cmsUInt32Number clutPoints[4] = {1, 1, 1, 1};

    if (table_size_16 > 0) {
        table_16 = (cmsUInt16Number*)malloc(table_size_16 * sizeof(cmsUInt16Number));
        if (table_16) {
            memset(table_16, 0, table_size_16 * sizeof(cmsUInt16Number));
            if (Size > 10) {
                size_t copy_len = (Size - 10) < (table_size_16 * sizeof(cmsUInt16Number)) ? (Size - 10) : (table_size_16 * sizeof(cmsUInt16Number));
                memcpy(table_16, Data + 10, copy_len);
            }
        }
    }

    if (table_size_float > 0) {
        table_float = (cmsFloat32Number*)malloc(table_size_float * sizeof(cmsFloat32Number));
        if (table_float) {
            memset(table_float, 0, table_size_float * sizeof(cmsFloat32Number));
            if (Size > 10) {
                size_t copy_len = (Size - 10) < (table_size_float * sizeof(cmsFloat32Number)) ? (Size - 10) : (table_size_float * sizeof(cmsFloat32Number));
                memcpy(table_float, Data + 10, copy_len);
            }
        }
    }

    if (rows > 0 && cols > 0) {
        size_t mat_size = rows * cols;
        matrix = (cmsFloat64Number*)malloc(mat_size * sizeof(cmsFloat64Number));
        offset = (cmsFloat64Number*)malloc(rows * sizeof(cmsFloat64Number));
        if (matrix && offset) {
            memset(matrix, 0, mat_size * sizeof(cmsFloat64Number));
            memset(offset, 0, rows * sizeof(cmsFloat64Number));
            if (Size > 20) {
                size_t copy_len = (Size - 20) < (mat_size * sizeof(cmsFloat64Number)) ? (Size - 20) : (mat_size * sizeof(cmsFloat64Number));
                memcpy(matrix, Data + 20, copy_len);
            }
        }
    }

    // Set granular points
    for (int i = 0; i < 4; i++) {
        if (i < Size) clutPoints[i] = Data[i] % 16 + 1;
    }

    cmsStage* stage = NULL;

    // 1. cmsStageAllocCLut16bit
    stage = cmsStageAllocCLut16bit(ContextID, nGridPoints, inputChan, outputChan, table_16);
    free_stage(stage);

    // 2. cmsStageAllocMatrix
    stage = cmsStageAllocMatrix(ContextID, rows, cols, matrix, offset);
    free_stage(stage);

    // 3. cmsStageAllocCLut16bitGranular
    stage = cmsStageAllocCLut16bitGranular(ContextID, clutPoints, inputChan, outputChan, table_16);
    free_stage(stage);

    // 4. cmsStageAllocCLutFloatGranular
    stage = cmsStageAllocCLutFloatGranular(ContextID, clutPoints, inputChan, outputChan, table_float);
    free_stage(stage);

    // 5. cmsStageAllocCLutFloat
    stage = cmsStageAllocCLutFloat(ContextID, nGridPoints, inputChan, outputChan, table_float);
    free_stage(stage);

    // 6. cmsStageAllocIdentity
    stage = cmsStageAllocIdentity(ContextID, inputChan);
    free_stage(stage);

    // Cleanup
    free(table_16);
    free(table_float);
    free(matrix);
    free(offset);
    cmsDeleteContext(ContextID);

    return 0;
}