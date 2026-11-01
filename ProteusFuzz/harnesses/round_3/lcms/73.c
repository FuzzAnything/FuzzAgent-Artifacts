// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_stages(cmsStage* s1, cmsStage* s2, cmsStage* s3, cmsStage* s4, cmsStage* s5) {
    if (s1) cmsStageFree(s1);
    if (s2) cmsStageFree(s2);
    if (s3) cmsStageFree(s3);
    if (s4) cmsStageFree(s4);
    if (s5) cmsStageFree(s5);
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsStage *stage1 = NULL, *stage2 = NULL, *stage3 = NULL, *stage4 = NULL, *stage5 = NULL;
    cmsToneCurve *curve = NULL;
    cmsToneCurve *curves[4] = {NULL, NULL, NULL, NULL};

    // Parse inputs from Data
    uint32_t inputChan = Data[0] % 5; // 0 to 4
    uint32_t outputChan = Data[1] % 5; // 0 to 4
    uint32_t nChannels = (Data[0] + Data[1]) % 5;
    uint32_t nGridPoints = (Data[0] % 10) + 1; // 1 to 10

    // Ensure valid ranges for some functions
    if (inputChan > 4) inputChan = 4;
    if (outputChan > 4) outputChan = 4;
    if (nChannels > 4) nChannels = 4;

    // Prepare clutPoints for granular functions
    cmsUInt32Number clutPoints[4] = {nGridPoints, nGridPoints, nGridPoints, nGridPoints};
    if (inputChan < 4) {
        for (int i = inputChan; i < 4; i++) clutPoints[i] = 1;
    }

    // Calculate table sizes
    size_t floatTableSize = 1;
    for (int i = 0; i < inputChan; i++) floatTableSize *= clutPoints[i];
    floatTableSize *= outputChan;

    size_t uint16TableSize = floatTableSize; // Same structure for 16-bit

    // Allocate tables
    cmsFloat32Number *floatTable = (cmsFloat32Number *)malloc(floatTableSize * sizeof(cmsFloat32Number));
    cmsUInt16Number *uint16Table = (cmsUInt16Number *)malloc(uint16TableSize * sizeof(cmsUInt16Number));

    if (!floatTable || !uint16Table) {
        cmsDeleteContext(ctx);
        free(floatTable);
        free(uint16Table);
        return 0;
    }

    // Initialize tables with fuzzed data if available
    size_t dataOffset = 2;
    size_t availableData = Size - dataOffset;
    size_t bytesNeeded = floatTableSize * sizeof(cmsFloat32Number);
    
    // Fill float table
    for (size_t i = 0; i < floatTableSize; i++) {
        if (dataOffset + sizeof(cmsFloat32Number) <= Size) {
            memcpy(&floatTable[i], &Data[dataOffset], sizeof(cmsFloat32Number));
            dataOffset += sizeof(cmsFloat32Number);
        } else {
            floatTable[i] = (float)(i % 256) / 256.0f;
        }
    }

    // Fill uint16 table
    for (size_t i = 0; i < uint16TableSize; i++) {
        if (dataOffset + sizeof(cmsUInt16Number) <= Size) {
            memcpy(&uint16Table[i], &Data[dataOffset], sizeof(cmsUInt16Number));
            dataOffset += sizeof(cmsUInt16Number);
        } else {
            uint16Table[i] = (uint16_t)(i % 65536);
        }
    }

    // 1. cmsStageAllocCLutFloatGranular
    stage1 = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, floatTable);

    // 2. cmsStageAllocToneCurves
    // Create a dummy curve if needed using cmsBuildGamma which is standard in lcms2
    if (nChannels > 0) {
        curve = cmsBuildGamma(ctx, 1.0); // Linear curve (gamma 1.0)
        if (curve) {
            for (int i = 0; i < nChannels && i < 4; i++) {
                curves[i] = curve;
            }
            stage2 = cmsStageAllocToneCurves(ctx, nChannels, curves);
        }
    }

    // 3. cmsStageAllocIdentity
    stage3 = cmsStageAllocIdentity(ctx, nChannels);

    // 4. cmsStageAllocCLut16bitGranular
    stage4 = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, uint16Table);

    // 5. cmsStageAllocCLutFloat
    stage5 = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, floatTable);

    // 6. cmsStageOutputChannels
    // Test on all valid stages
    if (stage1) cmsStageOutputChannels(stage1);
    if (stage2) cmsStageOutputChannels(stage2);
    if (stage3) cmsStageOutputChannels(stage3);
    if (stage4) cmsStageOutputChannels(stage4);
    if (stage5) cmsStageOutputChannels(stage5);

    // Cleanup
    cleanup_stages(stage1, stage2, stage3, stage4, stage5);
    if (curve) cmsFreeToneCurve(curve);
    free(floatTable);
    free(uint16Table);
    cmsDeleteContext(ctx);

    return 0;
}