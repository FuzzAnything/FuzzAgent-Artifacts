// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsContext createContext(void) {
    return cmsCreateContext(NULL, NULL);
}

static void cleanupContext(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static void cleanupStage(cmsStage *stage) {
    if (stage) {
        cmsStageFree(stage);
    }
}

static cmsFloat64Number* extractFloat64(const uint8_t *Data, size_t Size, size_t *outLen) {
    if (Size < sizeof(cmsFloat64Number)) {
        *outLen = 0;
        return NULL;
    }
    size_t count = Size / sizeof(cmsFloat64Number);
    if (count == 0) {
        *outLen = 0;
        return NULL;
    }
    cmsFloat64Number *arr = (cmsFloat64Number*)malloc(count * sizeof(cmsFloat64Number));
    if (!arr) {
        *outLen = 0;
        return NULL;
    }
    memcpy(arr, Data, count * sizeof(cmsFloat64Number));
    *outLen = count;
    return arr;
}

static cmsFloat32Number* extractFloat32(const uint8_t *Data, size_t Size, size_t *outLen) {
    if (Size < sizeof(cmsFloat32Number)) {
        *outLen = 0;
        return NULL;
    }
    size_t count = Size / sizeof(cmsFloat32Number);
    if (count == 0) {
        *outLen = 0;
        return NULL;
    }
    cmsFloat32Number *arr = (cmsFloat32Number*)malloc(count * sizeof(cmsFloat32Number));
    if (!arr) {
        *outLen = 0;
        return NULL;
    }
    memcpy(arr, Data, count * sizeof(cmsFloat32Number));
    *outLen = count;
    return arr;
}

static cmsUInt16Number* extractUInt16(const uint8_t *Data, size_t Size, size_t *outLen) {
    if (Size < sizeof(cmsUInt16Number)) {
        *outLen = 0;
        return NULL;
    }
    size_t count = Size / sizeof(cmsUInt16Number);
    if (count == 0) {
        *outLen = 0;
        return NULL;
    }
    cmsUInt16Number *arr = (cmsUInt16Number*)malloc(count * sizeof(cmsUInt16Number));
    if (!arr) {
        *outLen = 0;
        return NULL;
    }
    memcpy(arr, Data, count * sizeof(cmsUInt16Number));
    *outLen = count;
    return arr;
}

static cmsUInt32Number extractUInt32(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsUInt32Number)) {
        return 0;
    }
    cmsUInt32Number val;
    memcpy(&val, Data, sizeof(cmsUInt32Number));
    return val;
}

int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = createContext();
    if (!ctx) return 0;

    cmsStage *stage = NULL;
    cmsUInt32Number rows, cols, nChannels, inputChan, outputChan, nGridPoints;
    cmsUInt32Number clutPoints[4] = {1, 1, 1, 1};
    cmsFloat64Number *matrix = NULL, *offset = NULL;
    cmsFloat32Number *floatTable = NULL;
    cmsUInt16Number *uint16Table = NULL;
    size_t matrixLen, offsetLen, floatTableLen, uint16TableLen;

    // Parse basic parameters from the beginning of the input
    rows = extractUInt32(Data, Size);
    cols = extractUInt32(Data + sizeof(cmsUInt32Number), Size > sizeof(cmsUInt32Number) ? Size - sizeof(cmsUInt32Number) : 0);
    nChannels = extractUInt32(Data + 2 * sizeof(cmsUInt32Number), Size > 2 * sizeof(cmsUInt32Number) ? Size - 2 * sizeof(cmsUInt32Number) : 0);
    inputChan = extractUInt32(Data + 3 * sizeof(cmsUInt32Number), Size > 3 * sizeof(cmsUInt32Number) ? Size - 3 * sizeof(cmsUInt32Number) : 0);
    outputChan = extractUInt32(Data + 4 * sizeof(cmsUInt32Number), Size > 4 * sizeof(cmsUInt32Number) ? Size - 4 * sizeof(cmsUInt32Number) : 0);
    nGridPoints = extractUInt32(Data + 5 * sizeof(cmsUInt32Number), Size > 5 * sizeof(cmsUInt32Number) ? Size - 5 * sizeof(cmsUInt32Number) : 0);

    // Limit values to reasonable ranges to avoid immediate crashes or huge allocations
    if (rows > 10) rows = 10;
    if (cols > 10) cols = 10;
    if (nChannels > 4) nChannels = 4;
    if (inputChan > 4) inputChan = 4;
    if (outputChan > 4) outputChan = 4;
    if (nGridPoints > 10) nGridPoints = 10;

    // Extract arrays if enough data exists
    size_t offsetPos = 6 * sizeof(cmsUInt32Number);
    if (Size > offsetPos) {
        matrix = extractFloat64(Data + offsetPos, Size - offsetPos, &matrixLen);
        offsetPos += matrixLen * sizeof(cmsFloat64Number);
        if (Size > offsetPos) {
            offset = extractFloat64(Data + offsetPos, Size - offsetPos, &offsetLen);
            offsetPos += offsetLen * sizeof(cmsFloat64Number);
            if (Size > offsetPos) {
                floatTable = extractFloat32(Data + offsetPos, Size - offsetPos, &floatTableLen);
                offsetPos += floatTableLen * sizeof(cmsFloat32Number);
                if (Size > offsetPos) {
                    uint16Table = extractUInt16(Data + offsetPos, Size - offsetPos, &uint16TableLen);
                }
            }
        }
    }

    // Fuzz cmsStageAllocMatrix
    if (matrix && matrixLen >= rows * cols) {
        cmsFloat64Number *mat = matrix;
        cmsFloat64Number *off = offset ? offset : NULL;
        size_t offLen = offset ? offsetLen : 0;
        if (off && offLen < cols) off = NULL;
        stage = cmsStageAllocMatrix(ctx, rows, cols, mat, off);
        cleanupStage(stage);
        stage = NULL;
    }

    // Fuzz cmsStageAllocIdentity
    stage = cmsStageAllocIdentity(ctx, nChannels);
    cleanupStage(stage);
    stage = NULL;

    // Fuzz cmsStageAllocCLut16bitGranular
    if (uint16Table && uint16TableLen > 0) {
        // Construct clutPoints array from input if possible
        if (Size > offsetPos + 4 * sizeof(cmsUInt32Number)) {
            memcpy(clutPoints, Data + offsetPos, 4 * sizeof(cmsUInt32Number));
            // Limit grid points to avoid huge allocations
            for (int i = 0; i < 4; i++) {
                if (clutPoints[i] > 10) clutPoints[i] = 10;
            }
        }
        // Calculate expected table size
        cmsUInt32Number expectedSize = 1;
        for (int i = 0; i < inputChan; i++) expectedSize *= clutPoints[i];
        expectedSize *= outputChan;
        if (uint16TableLen >= expectedSize) {
            stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, uint16Table);
            cleanupStage(stage);
            stage = NULL;
        }
    }

    // Fuzz cmsStageAllocCLutFloat
    if (floatTable && floatTableLen > 0) {
        cmsUInt32Number expectedSize = 1;
        for (int i = 0; i < inputChan; i++) expectedSize *= nGridPoints;
        expectedSize *= outputChan;
        if (floatTableLen >= expectedSize) {
            stage = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, floatTable);
            cleanupStage(stage);
            stage = NULL;
        }
    }

    // Fuzz cmsStageAllocCLut16bit
    if (uint16Table && uint16TableLen > 0) {
        cmsUInt32Number expectedSize = 1;
        for (int i = 0; i < inputChan; i++) expectedSize *= nGridPoints;
        expectedSize *= outputChan;
        if (uint16TableLen >= expectedSize) {
            stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, uint16Table);
            cleanupStage(stage);
            stage = NULL;
        }
    }

    // Fuzz cmsStageAllocCLutFloatGranular
    if (floatTable && floatTableLen > 0) {
        // Use clutPoints if we parsed them, otherwise default to nGridPoints
        if (Size <= offsetPos + 4 * sizeof(cmsUInt32Number)) {
            for (int i = 0; i < 4; i++) clutPoints[i] = nGridPoints;
        }
        cmsUInt32Number expectedSize = 1;
        for (int i = 0; i < inputChan; i++) expectedSize *= clutPoints[i];
        expectedSize *= outputChan;
        if (floatTableLen >= expectedSize) {
            stage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, floatTable);
            cleanupStage(stage);
            stage = NULL;
        }
    }

    // Cleanup allocated arrays
    free(matrix);
    free(offset);
    free(floatTable);
    free(uint16Table);

    cleanupContext(ctx);
    return 0;
}