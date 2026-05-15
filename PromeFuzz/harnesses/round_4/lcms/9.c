// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

#ifdef __cplusplus
extern "C" {
#endif

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static cmsToneCurve* create_test_tone_curve(cmsContext ctx) {
    cmsFloat64Number gamma = 2.2;
    return cmsBuildGamma(ctx, gamma);
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    cmsContext ctx = create_test_context();
    if (!ctx) return 0;
    
    size_t offset = 0;
    
    cmsUInt32Number param1 = 0, param2 = 0, param3 = 0;
    if (Size >= 12) {
        memcpy(&param1, Data, sizeof(cmsUInt32Number));
        memcpy(&param2, Data + 4, sizeof(cmsUInt32Number));
        memcpy(&param3, Data + 8, sizeof(cmsUInt32Number));
        offset = 12;
    } else if (Size >= 4) {
        memcpy(&param1, Data, sizeof(cmsUInt32Number));
        offset = 4;
    }
    
    param1 = param1 % 256;
    param2 = param2 % 256;
    param3 = param3 % 256;
    
    if (param1 > 0 && param2 > 0 && param3 > 0) {
        cmsUInt32Number gridPoints = (param1 % 10) + 2;
        cmsUInt32Number inputChan = (param2 % 8) + 1;
        cmsUInt32Number outputChan = (param3 % 8) + 1;
        
        size_t tableSize = 1;
        for (cmsUInt32Number i = 0; i < inputChan; i++) {
            tableSize *= gridPoints;
        }
        tableSize *= outputChan;
        
        cmsUInt16Number* table = NULL;
        if (tableSize > 0 && tableSize < 1000000) {
            table = (cmsUInt16Number*)malloc(tableSize * sizeof(cmsUInt16Number));
            if (table) {
                size_t bytesToCopy = Size - offset;
                if (bytesToCopy > tableSize * sizeof(cmsUInt16Number)) {
                    bytesToCopy = tableSize * sizeof(cmsUInt16Number);
                }
                if (bytesToCopy > 0) {
                    memcpy(table, Data + offset, bytesToCopy);
                }
                
                cmsStage* stage = cmsStageAllocCLut16bit(ctx, gridPoints, inputChan, outputChan, table);
                if (stage) {
                    cmsContext stageCtx = cmsGetStageContextID(stage);
                    cmsUInt32Number outChannels = cmsStageOutputChannels(stage);
                    cmsStageFree(stage);
                }
                free(table);
            }
        }
    }
    
    if (param1 > 0) {
        cmsUInt32Number nChannels = (param1 % 16) + 1;
        cmsStage* stage = cmsStageAllocIdentity(ctx, nChannels);
        if (stage) {
            cmsContext stageCtx = cmsGetStageContextID(stage);
            cmsUInt32Number outChannels = cmsStageOutputChannels(stage);
            cmsStageFree(stage);
        }
    }
    
    if (param1 > 0) {
        cmsUInt32Number nChannels = (param1 % 8) + 1;
        
        cmsToneCurve** curves = (cmsToneCurve**)malloc(nChannels * sizeof(cmsToneCurve*));
        if (curves) {
            for (cmsUInt32Number i = 0; i < nChannels; i++) {
                curves[i] = create_test_tone_curve(ctx);
            }
            
            cmsStage* stage = cmsStageAllocToneCurves(ctx, nChannels, curves);
            if (stage) {
                cmsContext stageCtx = cmsGetStageContextID(stage);
                cmsUInt32Number outChannels = cmsStageOutputChannels(stage);
                cmsStageFree(stage);
            }
            
            for (cmsUInt32Number i = 0; i < nChannels; i++) {
                if (curves[i]) {
                    cmsFreeToneCurve(curves[i]);
                }
            }
            free(curves);
        }
    }
    
    if (param1 > 0 && param2 > 0 && param3 > 0) {
        cmsUInt32Number inputChan = (param1 % 8) + 1;
        cmsUInt32Number outputChan = (param2 % 8) + 1;
        
        cmsUInt32Number* clutPoints = (cmsUInt32Number*)malloc(inputChan * sizeof(cmsUInt32Number));
        if (clutPoints) {
            size_t totalPoints = 1;
            for (cmsUInt32Number i = 0; i < inputChan; i++) {
                clutPoints[i] = ((param3 + i) % 10) + 2;
                if (totalPoints > 0 && clutPoints[i] > 0) {
                    if (totalPoints > SIZE_MAX / clutPoints[i]) {
                        totalPoints = SIZE_MAX;
                    } else {
                        totalPoints *= clutPoints[i];
                    }
                }
            }
            
            size_t tableSize = totalPoints * outputChan;
            
            cmsUInt16Number* table = NULL;
            if (tableSize > 0 && tableSize < 1000000) {
                table = (cmsUInt16Number*)malloc(tableSize * sizeof(cmsUInt16Number));
                if (table) {
                    size_t bytesToCopy = Size - offset;
                    if (bytesToCopy > tableSize * sizeof(cmsUInt16Number)) {
                        bytesToCopy = tableSize * sizeof(cmsUInt16Number);
                    }
                    if (bytesToCopy > 0) {
                        memcpy(table, Data + offset, bytesToCopy);
                    }
                    
                    cmsStage* stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table);
                    if (stage) {
                        cmsContext stageCtx = cmsGetStageContextID(stage);
                        cmsUInt32Number outChannels = cmsStageOutputChannels(stage);
                        cmsStageFree(stage);
                    }
                    free(table);
                }
            }
            free(clutPoints);
        }
    }
    
    cmsDeleteContext(ctx);
    return 0;
}

#ifdef __cplusplus
}
#endif