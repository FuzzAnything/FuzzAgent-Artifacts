// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_stage(cmsStage* stage, cmsContext ctx) {
    if (stage != NULL) {
        cmsStageFree(stage);
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static cmsToneCurve* create_simple_curve(cmsContext ctx) {
    // Create a linear curve (identity) for testing using 16-bit table
    cmsToneCurve* curve = cmsBuildGamma(ctx, 1.0);
    return curve;
}

static void free_curve(cmsToneCurve* curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

int LLVMFuzzerTestOneInput_170(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Extract parameters from input data
    cmsUInt32Number nChannels = (Data[0] % 5) + 1; // 1 to 5 channels
    cmsUInt32Number gridPoints = (Data[1] % 10) + 2; // 2 to 11 grid points
    cmsUInt32Number inputChan = (Data[0] % 4) + 1;
    cmsUInt32Number outputChan = (Data[1] % 4) + 1;

    cmsStage* stage = NULL;
    cmsContext retrievedCtx = NULL;

    // Fuzz cmsStageAllocIdentity
    stage = cmsStageAllocIdentity(ctx, nChannels);
    if (stage != NULL) {
        retrievedCtx = cmsGetStageContextID(stage);
        // Context check is implicit, we just ensure no crash
        cmsStageFree(stage);
        stage = NULL;
    }

    // Fuzz cmsStageAllocCLut16bit
    if (Size >= 10) {
        // Calculate required table size for uniform grid
        // Total points = gridPoints^inputChan * outputChan
        // We need to ensure we don't overflow or read out of bounds
        size_t totalPoints = 1;
        int overflow = 0;
        for (int i = 0; i < inputChan; i++) {
            if (gridPoints > 0 && totalPoints > (SIZE_MAX / gridPoints)) {
                overflow = 1;
                break;
            }
            totalPoints *= gridPoints;
        }
        if (!overflow) {
            totalPoints *= outputChan;
        }
        
        if (!overflow && totalPoints > 0 && (totalPoints * sizeof(cmsUInt16Number)) <= (Size - 2)) {
            cmsUInt16Number* table16 = (cmsUInt16Number*)(Data + 2);
            stage = cmsStageAllocCLut16bit(ctx, gridPoints, inputChan, outputChan, table16);
            if (stage != NULL) {
                retrievedCtx = cmsGetStageContextID(stage);
                cmsStageFree(stage);
                stage = NULL;
            }
        }
    }

    // Fuzz cmsStageAllocCLut16bitGranular
    if (Size >= 10) {
        cmsUInt32Number clutPoints[4] = {gridPoints, gridPoints, gridPoints, gridPoints};
        size_t totalPoints = 1;
        int overflow = 0;
        for (int i = 0; i < inputChan && i < 4; i++) {
            if (clutPoints[i] > 0 && totalPoints > (SIZE_MAX / clutPoints[i])) {
                overflow = 1;
                break;
            }
            totalPoints *= clutPoints[i];
        }
        if (!overflow) {
            totalPoints *= outputChan;
        }

        if (!overflow && totalPoints > 0 && (totalPoints * sizeof(cmsUInt16Number)) <= (Size - 2)) {
            cmsUInt16Number* table16 = (cmsUInt16Number*)(Data + 2);
            stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table16);
            if (stage != NULL) {
                retrievedCtx = cmsGetStageContextID(stage);
                cmsStageFree(stage);
                stage = NULL;
            }
        }
    }

    // Fuzz cmsStageAllocCLutFloat
    if (Size >= 10) {
        size_t totalPoints = 1;
        int overflow = 0;
        for (int i = 0; i < inputChan; i++) {
            if (gridPoints > 0 && totalPoints > (SIZE_MAX / gridPoints)) {
                overflow = 1;
                break;
            }
            totalPoints *= gridPoints;
        }
        if (!overflow) {
            totalPoints *= outputChan;
        }

        if (!overflow && totalPoints > 0 && (totalPoints * sizeof(cmsFloat32Number)) <= (Size - 2)) {
            cmsFloat32Number* tableFloat = (cmsFloat32Number*)(Data + 2);
            stage = cmsStageAllocCLutFloat(ctx, gridPoints, inputChan, outputChan, tableFloat);
            if (stage != NULL) {
                retrievedCtx = cmsGetStageContextID(stage);
                cmsStageFree(stage);
                stage = NULL;
            }
        }
    }

    // Fuzz cmsStageAllocToneCurves
    if (Size >= 2) {
        cmsToneCurve* curves[5] = {NULL};
        int numCurves = 0;
        int maxCurves = (nChannels < 5) ? nChannels : 5;
        
        for (int i = 0; i < maxCurves; i++) {
            curves[i] = create_simple_curve(ctx);
            if (curves[i] != NULL) {
                numCurves++;
            }
        }
        
        if (numCurves > 0) {
            stage = cmsStageAllocToneCurves(ctx, numCurves, curves);
            if (stage != NULL) {
                retrievedCtx = cmsGetStageContextID(stage);
                cmsStageFree(stage);
                stage = NULL;
            }
        }
        
        for (int i = 0; i < maxCurves; i++) {
            free_curve(curves[i]);
        }
    }

    cleanup_stage(stage, ctx);
    return 0;
}