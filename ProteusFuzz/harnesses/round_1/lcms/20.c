// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_stage(cmsContext ctx, cmsStage* stage) {
    if (stage != NULL) {
        cmsStageFree(stage);
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    uint8_t mode = Data[0] % 6;
    uint32_t nChannels = (Data[1] % 5) + 1; // 1 to 5
    uint32_t nGridPoints = (Data[1] % 10) + 2; // 2 to 11
    uint32_t inputChan = (Data[1] % 4) + 1;
    uint32_t outputChan = (Data[1] % 4) + 1;

    cmsStage* stage = NULL;
    cmsUInt32Number inChans = 0;
    cmsUInt32Number outChans = 0;

    switch (mode) {
        case 0: // cmsStageAllocIdentity
            stage = cmsStageAllocIdentity(ctx, nChannels);
            if (stage != NULL) {
                inChans = cmsStageInputChannels(stage);
                outChans = cmsStageOutputChannels(stage);
            }
            break;

        case 1: // cmsStageAllocCLut16bitGranular
        {
            cmsUInt32Number points[4] = {0};
            for (int i = 0; i < 4; i++) {
                points[i] = (Size > 2 + i) ? (Data[2 + i] % 10) + 2 : 2;
            }
            // Limit to max 4 channels for CLUT
            uint32_t actualIn = (inputChan > 4) ? 4 : inputChan;
            uint32_t actualOut = (outputChan > 4) ? 4 : outputChan;
            
            // Calculate required table size for granular
            size_t reqSize = 1;
            for (int i = 0; i < actualIn; i++) reqSize *= points[i];
            reqSize *= actualOut;
            
            if (Size >= 2 + reqSize * sizeof(cmsUInt16Number)) {
                const cmsUInt16Number* table = (const cmsUInt16Number*)(Data + 2);
                stage = cmsStageAllocCLut16bitGranular(ctx, points, actualIn, actualOut, table);
            }
            break;
        }

        case 2: // cmsStageAllocCLut16bit
        {
            uint32_t actualIn = (inputChan > 4) ? 4 : inputChan;
            uint32_t actualOut = (outputChan > 4) ? 4 : outputChan;
            size_t reqSize = 1;
            for (int i = 0; i < actualIn; i++) reqSize *= nGridPoints;
            reqSize *= actualOut;

            if (Size >= 2 + reqSize * sizeof(cmsUInt16Number)) {
                const cmsUInt16Number* table = (const cmsUInt16Number*)(Data + 2);
                stage = cmsStageAllocCLut16bit(ctx, nGridPoints, actualIn, actualOut, table);
            }
            break;
        }

        case 3: // cmsStageAllocCLutFloat
        {
            uint32_t actualIn = (inputChan > 4) ? 4 : inputChan;
            uint32_t actualOut = (outputChan > 4) ? 4 : outputChan;
            size_t reqSize = 1;
            for (int i = 0; i < actualIn; i++) reqSize *= nGridPoints;
            reqSize *= actualOut;

            if (Size >= 2 + reqSize * sizeof(cmsFloat32Number)) {
                const cmsFloat32Number* table = (const cmsFloat32Number*)(Data + 2);
                stage = cmsStageAllocCLutFloat(ctx, nGridPoints, actualIn, actualOut, table);
            }
            break;
        }
        
        case 4: // Mixed test: Identity then query
            stage = cmsStageAllocIdentity(ctx, nChannels);
            if (stage != NULL) {
                inChans = cmsStageInputChannels(stage);
                outChans = cmsStageOutputChannels(stage);
            }
            break;
            
        case 5: // Edge case: Invalid channels for CLUT (should return NULL)
        {
            uint32_t actualIn = inputChan; 
            uint32_t actualOut = outputChan;
            // Force high channel count to trigger validation failure
            if (actualIn > 4 || actualOut > 4) {
                // Provide dummy table to avoid crash on NULL check inside function if it happens
                cmsUInt16Number dummy = 0;
                stage = cmsStageAllocCLut16bit(ctx, nGridPoints, actualIn, actualOut, &dummy);
            }
            break;
        }
    }

    // Additional queries if stage exists
    if (stage != NULL) {
        // Re-query to ensure stability
        inChans = cmsStageInputChannels(stage);
        outChans = cmsStageOutputChannels(stage);
        
        // Free the stage
        cmsStageFree(stage);
    }

    // Cleanup context
    cmsDeleteContext(ctx);

    return 0;
}