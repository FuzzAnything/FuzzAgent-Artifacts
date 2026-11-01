// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_stage(cmsStage* stage) {
    if (stage != NULL) {
        cmsStageFree(stage);
    }
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Extract parameters from fuzz input
    cmsUInt32Number nGridPoints = Data[0] + 1; // Ensure at least 1
    cmsUInt32Number inputChan = Data[1] + 1;   // Ensure at least 1
    cmsUInt32Number outputChan = Data[2] + 1;  // Ensure at least 1
    cmsUInt32Number identityChan = Data[3];    // Can be 0

    // Limit values to prevent allocation of huge tables
    if (nGridPoints > 10) nGridPoints = 10;
    if (inputChan > 10) inputChan = 10;
    if (outputChan > 10) outputChan = 10;
    if (identityChan > 10) identityChan = 10;

    cmsStage* stage = NULL;
    cmsUInt32Number inCh = 0;
    cmsUInt32Number outCh = 0;

    // Test 1: cmsStageAllocIdentity
    if (identityChan > 0) {
        stage = cmsStageAllocIdentity(ctx, identityChan);
        if (stage != NULL) {
            inCh = cmsStageInputChannels(stage);
            outCh = cmsStageOutputChannels(stage);
            cleanup_stage(stage);
            stage = NULL;
        }
    }

    // Test 2: cmsStageAllocCLut16bit
    // Required size: nGridPoints^inputChan * outputChan * 2 bytes
    // We need to be careful not to overflow size_t or allocate too much
    // For fuzzing, we limit the total table size to a reasonable amount
    size_t requiredSize16 = 1;
    for (int i = 0; i < inputChan; i++) {
        if (Size / requiredSize16 < nGridPoints) { // Check for overflow
            requiredSize16 = Size + 1; 
            break;
        }
        requiredSize16 *= nGridPoints;
    }
    if (Size / requiredSize16 < outputChan) {
        requiredSize16 = Size + 1;
    } else {
        requiredSize16 *= outputChan;
    }
    requiredSize16 *= 2;

    if (Size >= 4 + requiredSize16) {
        const cmsUInt16Number* table16 = (const cmsUInt16Number*)(Data + 4);
        stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table16);
        if (stage != NULL) {
            inCh = cmsStageInputChannels(stage);
            outCh = cmsStageOutputChannels(stage);
            cleanup_stage(stage);
            stage = NULL;
        }
    }

    // Test 3: cmsStageAllocCLutFloat
    size_t requiredSizeFloat = 1;
    for (int i = 0; i < inputChan; i++) {
        if (Size / requiredSizeFloat < nGridPoints) {
            requiredSizeFloat = Size + 1;
            break;
        }
        requiredSizeFloat *= nGridPoints;
    }
    if (Size / requiredSizeFloat < outputChan) {
        requiredSizeFloat = Size + 1;
    } else {
        requiredSizeFloat *= outputChan;
    }
    requiredSizeFloat *= 4; // float is 4 bytes

    if (Size >= 4 + requiredSizeFloat) {
        const cmsFloat32Number* tableFloat = (const cmsFloat32Number*)(Data + 4);
        stage = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, tableFloat);
        if (stage != NULL) {
            inCh = cmsStageInputChannels(stage);
            outCh = cmsStageOutputChannels(stage);
            cleanup_stage(stage);
            stage = NULL;
        }
    }

    // Test 4: cmsStageAllocCLutFloatGranular
    // Prepare clutPoints array (variable grid per dimension)
    cmsUInt32Number clutPoints[10];
    // Ensure we have enough data to read clutPoints
    size_t availableForPoints = Size - 4;
    size_t pointsToRead = (availableForPoints < inputChan) ? availableForPoints : inputChan;
    
    for (int i = 0; i < pointsToRead; i++) {
        clutPoints[i] = (Data[4 + i] % 10) + 1;
    }
    // Fill remaining with 1 if inputChan > available data
    for (int i = pointsToRead; i < inputChan && i < 10; i++) {
        clutPoints[i] = 1;
    }

    // Calculate required table size for granular
    size_t totalPoints = 1;
    int overflow = 0;
    for (int i = 0; i < inputChan; i++) {
        if (Size / totalPoints < clutPoints[i]) {
            overflow = 1;
            break;
        }
        totalPoints *= clutPoints[i];
    }
    
    if (!overflow) {
        size_t tableSizeBytes = totalPoints * outputChan * sizeof(cmsFloat32Number);
        if (Size >= 4 + tableSizeBytes) {
            const cmsFloat32Number* tableFloatGran = (const cmsFloat32Number*)(Data + 4);
            stage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, tableFloatGran);
            if (stage != NULL) {
                inCh = cmsStageInputChannels(stage);
                outCh = cmsStageOutputChannels(stage);
                cleanup_stage(stage);
                stage = NULL;
            }
        }
    }

    // Cleanup context
    cmsDeleteContext(ctx);
    
    return 0;
}