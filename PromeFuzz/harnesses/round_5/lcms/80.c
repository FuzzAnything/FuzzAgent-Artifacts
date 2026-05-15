// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsSliceSpaceFloat at cmslut.c:910:26 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSliceSpace16 at cmslut.c:879:19 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsBool Sampler16(const cmsUInt16Number In[], cmsUInt16Number Out[], void* Cargo) {
    (void)In;
    (void)Out;
    (void)Cargo;
    return TRUE;
}

static cmsBool SamplerFloat(const cmsFloat32Number In[], cmsFloat32Number Out[], void* Cargo) {
    (void)In;
    (void)Out;
    (void)Cargo;
    return TRUE;
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Parse parameters from fuzzer input
    size_t offset = 0;
    cmsUInt32Number nInputs = Data[offset++] % 8 + 1;
    if (nInputs > 8) nInputs = 8;
    
    cmsUInt32Number clutPoints[8];
    for (cmsUInt32Number i = 0; i < nInputs; i++) {
        if (offset >= Size) break;
        clutPoints[i] = (Data[offset++] % 16) + 2;
        if (clutPoints[i] < 2) clutPoints[i] = 2;
        if (clutPoints[i] > 256) clutPoints[i] = 256; // Add reasonable upper bound
    }

    cmsUInt32Number inputChan = nInputs;
    if (offset >= Size) goto cleanup;
    cmsUInt32Number outputChan = Data[offset++] % 8 + 1;
    if (outputChan > 8) outputChan = 8;

    // Calculate table size with overflow protection
    size_t tableSize = outputChan;
    for (cmsUInt32Number i = 0; i < nInputs; i++) {
        if (clutPoints[i] == 0) {
            tableSize = 0;
            break;
        }
        if (tableSize > SIZE_MAX / clutPoints[i]) {
            tableSize = 0;
            break;
        }
        tableSize *= clutPoints[i];
    }

    cmsUInt16Number* table = NULL;
    if (tableSize > 0 && tableSize <= 65536) {
        table = (cmsUInt16Number*)malloc(tableSize * sizeof(cmsUInt16Number));
        if (table) {
            size_t bytesToCopy = tableSize * sizeof(cmsUInt16Number);
            if (bytesToCopy <= Size - offset) {
                memcpy(table, Data + offset, bytesToCopy);
                offset += bytesToCopy;
            } else {
                // Fill with deterministic data
                for (size_t i = 0; i < tableSize; i++) {
                    table[i] = (i * 65537) % 65536;
                }
            }
        }
    }

    // Test cmsSliceSpace16
    if (nInputs > 0) {
        cmsSliceSpace16(nInputs, clutPoints, Sampler16, NULL);
    }

    // Test cmsStageAllocCLut16bit
    cmsUInt32Number nGridPoints = clutPoints[0];
    if (nGridPoints >= 2 && nGridPoints <= 256) {
        cmsStage* stage1 = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table);
        if (stage1) {
            // Test cmsStageSampleCLut16bit
            cmsStageSampleCLut16bit(stage1, Sampler16, NULL, 0);
            cmsStageFree(stage1);
        }
    }

    // Test cmsStageAllocCLut16bitGranular
    cmsStage* stage2 = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table);
    if (stage2) {
        // Only call cmsStageSampleCLutFloat if we have valid parameters
        // Check that the stage is actually a float CLUT by examining its type
        // For safety, we'll skip this call for 16-bit CLUTs
        // cmsStageSampleCLutFloat(stage2, SamplerFloat, NULL, 0);
        cmsStageFree(stage2);
    }

    // Test cmsSliceSpaceFloat
    if (nInputs > 0) {
        cmsSliceSpaceFloat(nInputs, clutPoints, SamplerFloat, NULL);
    }

cleanup:
    free(table);
    cmsDeleteContext(ctx);
    return 0;
}