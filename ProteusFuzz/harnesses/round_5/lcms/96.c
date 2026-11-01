// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_stage(cmsStage *stage) {
    if (stage != NULL) {
        cmsStageFree(stage);
    }
}

static cmsContext create_context(void) {
    return cmsCreateContext(NULL, NULL);
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = create_context();
    if (ctx == NULL) return 0;

    // Extract parameters from input data safely
    cmsUInt32Number inputChan = 1;
    cmsUInt32Number outputChan = 1;
    cmsUInt32Number nGridPoints = 2;
    cmsUInt32Number clutPoints[4] = {2, 2, 2, 2};

    if (Size >= 4) {
        inputChan = Data[0] % 5; // 0 to 4
        outputChan = Data[1] % 5; // 0 to 4
        nGridPoints = (Data[2] % 10) + 1; // 1 to 10
        clutPoints[0] = (Data[3] % 10) + 1;
        if (Size > 4) clutPoints[1] = (Data[4] % 10) + 1;
        if (Size > 5) clutPoints[2] = (Data[5] % 10) + 1;
        if (Size > 6) clutPoints[3] = (Data[6] % 10) + 1;
    }

    // Ensure at least 1 channel for valid operations
    if (inputChan == 0) inputChan = 1;
    if (outputChan == 0) outputChan = 1;
    if (nGridPoints == 0) nGridPoints = 2;

    // Calculate table size for 16-bit CLUT
    // Ensure we don't overflow or allocate too much
    cmsUInt32Number tableSize16 = 1;
    int overflow = 0;
    for (int i = 0; i < inputChan; i++) {
        if (clutPoints[i] == 0) {
            overflow = 1;
            break;
        }
        if (__builtin_mul_overflow(tableSize16, clutPoints[i], &tableSize16)) {
            overflow = 1;
            break;
        }
    }
    if (!overflow && __builtin_mul_overflow(tableSize16, outputChan, &tableSize16)) {
        overflow = 1;
    }

    cmsUInt16Number *table16 = NULL;
    if (!overflow && tableSize16 > 0 && tableSize16 <= 100000) {
        table16 = (cmsUInt16Number *)malloc(tableSize16 * sizeof(cmsUInt16Number));
        if (table16) {
            memset(table16, 0, tableSize16 * sizeof(cmsUInt16Number));
            // Fill with some data from input if available, ensuring bounds
            size_t fillSize = (Size > tableSize16 * 2) ? tableSize16 * 2 : Size;
            for (size_t i = 0; i < fillSize && i < tableSize16 * 2; i++) {
                table16[i / 2] |= (Data[i] << ((i % 2) * 8));
            }
        }
    }

    // Calculate table size for float CLUT (same logic)
    cmsUInt32Number tableSizeFloat = 1;
    overflow = 0;
    for (int i = 0; i < inputChan; i++) {
        if (clutPoints[i] == 0) {
            overflow = 1;
            break;
        }
        if (__builtin_mul_overflow(tableSizeFloat, clutPoints[i], &tableSizeFloat)) {
            overflow = 1;
            break;
        }
    }
    if (!overflow && __builtin_mul_overflow(tableSizeFloat, outputChan, &tableSizeFloat)) {
        overflow = 1;
    }

    cmsFloat32Number *tableFloat = NULL;
    if (!overflow && tableSizeFloat > 0 && tableSizeFloat <= 100000) {
        tableFloat = (cmsFloat32Number *)malloc(tableSizeFloat * sizeof(cmsFloat32Number));
        if (tableFloat) {
            memset(tableFloat, 0, tableSizeFloat * sizeof(cmsFloat32Number));
            for (size_t i = 0; i < tableSizeFloat && i < Size; i++) {
                tableFloat[i] = (float)(Data[i] / 255.0);
            }
        }
    }

    cmsStage *stage = NULL;

    // Test cmsStageAllocCLut16bit
    // Note: cmsStageAllocCLut16bit uses nGridPoints for all dimensions.
    // We must ensure the table size matches nGridPoints^inputChan * outputChan
    if (table16 != NULL) {
        cmsUInt32Number expectedSize16 = 1;
        int ok = 1;
        for (int i = 0; i < inputChan; i++) {
            if (__builtin_mul_overflow(expectedSize16, nGridPoints, &expectedSize16)) {
                ok = 0; break;
            }
        }
        if (ok && __builtin_mul_overflow(expectedSize16, outputChan, &expectedSize16)) {
            ok = 0;
        }
        
        if (ok && expectedSize16 == tableSize16) {
            stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table16);
            if (stage != NULL) {
                cmsUInt32Number out = cmsStageOutputChannels(stage);
                (void)out;
                cleanup_stage(stage);
                stage = NULL;
            }
        }
    }

    // Test cmsStageAllocCLut16bitGranular
    if (table16 != NULL) {
        // Verify table size matches clutPoints product
        cmsUInt32Number expectedSize16 = 1;
        int ok = 1;
        for (int i = 0; i < inputChan; i++) {
            if (__builtin_mul_overflow(expectedSize16, clutPoints[i], &expectedSize16)) {
                ok = 0; break;
            }
        }
        if (ok && __builtin_mul_overflow(expectedSize16, outputChan, &expectedSize16)) {
            ok = 0;
        }

        if (ok && expectedSize16 == tableSize16) {
            stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table16);
            if (stage != NULL) {
                cmsUInt32Number out = cmsStageOutputChannels(stage);
                (void)out;
                cleanup_stage(stage);
                stage = NULL;
            }
        }
    }

    // Test cmsStageAllocCLutFloat
    if (tableFloat != NULL) {
        cmsUInt32Number expectedSizeFloat = 1;
        int ok = 1;
        for (int i = 0; i < inputChan; i++) {
            if (__builtin_mul_overflow(expectedSizeFloat, nGridPoints, &expectedSizeFloat)) {
                ok = 0; break;
            }
        }
        if (ok && __builtin_mul_overflow(expectedSizeFloat, outputChan, &expectedSizeFloat)) {
            ok = 0;
        }

        if (ok && expectedSizeFloat == tableSizeFloat) {
            stage = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, tableFloat);
            if (stage != NULL) {
                cmsUInt32Number out = cmsStageOutputChannels(stage);
                (void)out;
                cleanup_stage(stage);
                stage = NULL;
            }
        }
    }

    // Test cmsStageAllocCLutFloatGranular
    if (tableFloat != NULL) {
        cmsUInt32Number expectedSizeFloat = 1;
        int ok = 1;
        for (int i = 0; i < inputChan; i++) {
            if (__builtin_mul_overflow(expectedSizeFloat, clutPoints[i], &expectedSizeFloat)) {
                ok = 0; break;
            }
        }
        if (ok && __builtin_mul_overflow(expectedSizeFloat, outputChan, &expectedSizeFloat)) {
            ok = 0;
        }

        if (ok && expectedSizeFloat == tableSizeFloat) {
            stage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, tableFloat);
            if (stage != NULL) {
                cmsUInt32Number out = cmsStageOutputChannels(stage);
                (void)out;
                cleanup_stage(stage);
                stage = NULL;
            }
        }
    }

    // Test cmsStageAllocIdentity
    stage = cmsStageAllocIdentity(ctx, inputChan);
    if (stage != NULL) {
        cmsUInt32Number out = cmsStageOutputChannels(stage);
        (void)out;
        cleanup_stage(stage);
        stage = NULL;
    }

    // Cleanup
    free(table16);
    free(tableFloat);
    cmsDeleteContext(ctx);

    return 0;
}