// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void dummy_eval(const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage* mpe) {
    if (mpe) {
        cmsUInt32Number nChannels = cmsStageOutputChannels(mpe);
        for (cmsUInt32Number i = 0; i < nChannels; ++i) {
            Out[i] = In[i];
        }
    }
}

static void* dummy_dup(cmsStage* mpe) {
    (void)mpe;
    cmsUInt32Number* copy = (cmsUInt32Number*)malloc(sizeof(cmsUInt32Number));
    if (copy) {
        *copy = 1;
    }
    return copy;
}

static void dummy_free(cmsStage* mpe) {
    (void)mpe;
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsUInt32Number nGridPoints = 2;
    cmsUInt32Number inputChan = 1;
    cmsUInt32Number outputChan = 1;
    cmsUInt32Number nChannels = 1;
    
    if (Size >= 4) {
        nGridPoints = ((cmsUInt32Number*)Data)[0] % 10 + 2;
    }
    if (Size >= 8) {
        inputChan = ((cmsUInt32Number*)Data)[1] % 8 + 1;
    }
    if (Size >= 12) {
        outputChan = ((cmsUInt32Number*)Data)[2] % 8 + 1;
    }
    if (Size >= 16) {
        nChannels = ((cmsUInt32Number*)Data)[3] % 8 + 1;
    }
    
    cmsStage* stage1 = cmsStageAllocIdentity(ctx, nChannels);
    if (stage1) {
        cmsUInt32Number in1 = cmsStageInputChannels(stage1);
        cmsUInt32Number out1 = cmsStageOutputChannels(stage1);
        (void)in1; (void)out1;
        cmsStageFree(stage1);
    }
    
    size_t table_size = 1;
    for (cmsUInt32Number i = 0; i < inputChan; ++i) {
        table_size *= nGridPoints;
    }
    table_size *= outputChan;
    
    if (table_size > 0 && table_size <= 1000000) {
        cmsUInt16Number* table = (cmsUInt16Number*)calloc(table_size, sizeof(cmsUInt16Number));
        if (table) {
            cmsStage* stage2 = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table);
            if (stage2) {
                cmsUInt32Number in2 = cmsStageInputChannels(stage2);
                cmsUInt32Number out2 = cmsStageOutputChannels(stage2);
                (void)in2; (void)out2;
                cmsStageFree(stage2);
            }
            free(table);
        }
    }
    
    cmsUInt32Number clutPoints[8];
    for (int i = 0; i < 8; ++i) {
        clutPoints[i] = 2;
    }
    
    if (Size >= 4) {
        for (int i = 0; i < 8 && i < (int)(Size / 4); ++i) {
            clutPoints[i] = ((cmsUInt32Number*)Data)[i % (Size / 4)] % 5 + 2;
        }
    }
    
    size_t table_size2 = 1;
    for (cmsUInt32Number i = 0; i < inputChan && i < 8; ++i) {
        table_size2 *= clutPoints[i];
    }
    table_size2 *= outputChan;
    
    if (table_size2 > 0 && table_size2 <= 1000000) {
        cmsUInt16Number* table2 = (cmsUInt16Number*)calloc(table_size2, sizeof(cmsUInt16Number));
        if (table2) {
            cmsStage* stage3 = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table2);
            if (stage3) {
                cmsUInt32Number in3 = cmsStageInputChannels(stage3);
                cmsUInt32Number out3 = cmsStageOutputChannels(stage3);
                (void)in3; (void)out3;
                cmsStageFree(stage3);
            }
            free(table2);
        }
    }
    
    cmsUInt32Number* placeholder_data = (cmsUInt32Number*)malloc(sizeof(cmsUInt32Number));
    if (placeholder_data) {
        *placeholder_data = outputChan;
        cmsStage* stage4 = _cmsStageAllocPlaceholder(ctx, 0, inputChan, outputChan, dummy_eval, dummy_dup, dummy_free, placeholder_data);
        if (stage4) {
            cmsUInt32Number in4 = cmsStageInputChannels(stage4);
            cmsUInt32Number out4 = cmsStageOutputChannels(stage4);
            (void)in4; (void)out4;
            cmsStageFree(stage4);
        } else {
            free(placeholder_data);
        }
    }
    
    cmsDeleteContext(ctx);
    return 0;
}