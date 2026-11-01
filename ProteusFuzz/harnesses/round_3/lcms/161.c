// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_stage(cmsStage* stage) {
    if (stage != NULL) {
        cmsStageFree(stage);
    }
}

int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Extract parameters from fuzz input with strict bounds checking
    uint32_t inputChan = 1;
    uint32_t outputChan = 1;
    uint32_t nGridPoints = 2;
    uint32_t nChannels = 1;
    
    if (Size >= 4) {
        inputChan = *(uint32_t*)(Data);
        if (inputChan > 4) inputChan = 4; // LCMS typically limits to 4 channels for CLUTs
        if (inputChan == 0) inputChan = 1;
    }
    if (Size >= 8) {
        outputChan = *(uint32_t*)(Data + 4);
        if (outputChan > 4) outputChan = 4;
        if (outputChan == 0) outputChan = 1;
    }
    if (Size >= 12) {
        nGridPoints = *(uint32_t*)(Data + 8);
        if (nGridPoints > 10) nGridPoints = 10; // Limit grid size to prevent OOM
        if (nGridPoints < 2) nGridPoints = 2;
    }
    if (Size >= 16) {
        nChannels = *(uint32_t*)(Data + 12);
        if (nChannels > 8) nChannels = 8;
        if (nChannels == 0) nChannels = 1;
    }

    // Test cmsStageAllocIdentity
    cmsStage* stageIdentity = cmsStageAllocIdentity(ctx, nChannels);
    if (stageIdentity != NULL) {
        cmsContext checkCtx = cmsGetStageContextID(stageIdentity);
        (void)checkCtx; // Suppress unused warning
        cleanup_stage(stageIdentity);
    }

    // Test cmsStageAllocToneCurves
    cmsToneCurve* curves[8] = {NULL};
    cmsStage* stageTone = cmsStageAllocToneCurves(ctx, nChannels, curves);
    cleanup_stage(stageTone);

    // Prepare data for CLUT tests
    // Calculate total points: grid_size^input_channels
    size_t totalPoints = 1;
    for (uint32_t i = 0; i < inputChan; i++) {
        if (nGridPoints > 100) { // Safety check for overflow
            totalPoints = 0; 
            break;
        }
        totalPoints *= nGridPoints;
    }

    // Safety cap for table size to avoid OOM
    if (totalPoints > 100000) totalPoints = 100000;

    cmsFloat32Number* floatTable = NULL;
    cmsUInt16Number* uint16Table = NULL;
    cmsUInt32Number* clutPoints = NULL;

    // Allocate tables if size is reasonable
    if (totalPoints > 0) {
        size_t floatSize = totalPoints * outputChan * sizeof(cmsFloat32Number);
        size_t uint16Size = totalPoints * outputChan * sizeof(cmsUInt16Number);
        
        if (floatSize < 10 * 1024 * 1024) { // 10MB limit
            floatTable = (cmsFloat32Number*)malloc(floatSize);
            if (floatTable) {
                // Initialize with deterministic values to avoid garbage data issues
                // Use fuzz data only if available and within bounds
                size_t dataAvailable = (Size > 20) ? (Size - 20) : 0;
                size_t copySize = (dataAvailable < floatSize) ? dataAvailable : floatSize;
                
                if (copySize > 0) {
                    memcpy(floatTable, Data + 20, copySize);
                }
                if (copySize < floatSize) {
                    memset(floatTable + copySize/sizeof(cmsFloat32Number), 0, floatSize - copySize);
                }
            }
        }
        
        if (uint16Size < 10 * 1024 * 1024) {
            uint16Table = (cmsUInt16Number*)malloc(uint16Size);
            if (uint16Table) {
                size_t dataAvailable = (Size > 20) ? (Size - 20) : 0;
                size_t copySize = (dataAvailable < uint16Size) ? dataAvailable : uint16Size;
                
                if (copySize > 0) {
                    memcpy(uint16Table, Data + 20, copySize);
                }
                if (copySize < uint16Size) {
                    memset(uint16Table + copySize/sizeof(cmsUInt16Number), 0, uint16Size - copySize);
                }
            }
        }

        // Allocate clutPoints array
        clutPoints = (cmsUInt32Number*)malloc(inputChan * sizeof(cmsUInt32Number));
        if (clutPoints) {
            for (uint32_t i = 0; i < inputChan; i++) {
                clutPoints[i] = nGridPoints;
            }
        }
    }

    // Test cmsStageAllocCLutFloat
    cmsStage* stageFloat = NULL;
    if (floatTable != NULL && inputChan <= 4 && outputChan <= 4) {
        stageFloat = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, floatTable);
    }
    cleanup_stage(stageFloat);

    // Test cmsStageAllocCLutFloatGranular
    cmsStage* stageFloatGran = NULL;
    if (floatTable != NULL && clutPoints != NULL && inputChan <= 4 && outputChan <= 4) {
        stageFloatGran = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, floatTable);
    }
    cleanup_stage(stageFloatGran);

    // Test cmsStageAllocCLut16bitGranular
    cmsStage* stage16bit = NULL;
    if (uint16Table != NULL && clutPoints != NULL && inputChan <= 4 && outputChan <= 4) {
        stage16bit = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, uint16Table);
    }
    cleanup_stage(stage16bit);

    // Cleanup allocated tables
    free(floatTable);
    free(uint16Table);
    free(clutPoints);

    // Cleanup context
    cmsDeleteContext(ctx);

    return 0;
}