// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsSliceSpace16 at cmslut.c:879:19 in lcms2.h
// cmsSliceSpaceFloat at cmslut.c:910:26 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageSampleCLutFloat at cmslut.c:813:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static int g_counter = 0;

static cmsBool CALLBACK_Sampler16(const cmsUInt16Number *In, cmsUInt16Number *Out, void *Cargo) {
    cmsUInt32Number n = *(cmsUInt32Number *)Cargo;
    (void)n;
    if (Out && In) {
        Out[0] = In[0];
    }
    return TRUE;
}

static cmsBool CALLBACK_SamplerFloat(const cmsFloat32Number *In, cmsFloat32Number *Out, void *Cargo) {
    cmsUInt32Number n = *(cmsUInt32Number *)Cargo;
    (void)n;
    if (Out && In) {
        Out[0] = In[0];
    }
    return TRUE;
}

static void cleanup_stage(cmsStage *stage) {
    if (stage) {
        cmsStageFree(stage);
    }
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        ctx = cmsCreateContext(NULL, NULL);
        if (!ctx) return 0;
    }

    cmsUInt32Number nInputs = Data[0] % 10;
    cmsUInt32Number nGridPoints = (Data[1] % 10) + 1;
    cmsUInt32Number inputChan = (Data[0] % 5) + 1;
    cmsUInt32Number outputChan = (Data[1] % 5) + 1;
    cmsUInt32Number dwFlags = 0;

    cmsUInt32Number clutPoints[10];
    for (int i = 0; i < nInputs && i < 10; i++) {
        clutPoints[i] = (Size > i + 2) ? (Data[i + 2] % 10) + 1 : nGridPoints;
    }

    size_t tableSize16 = 1;
    for (int i = 0; i < nInputs && i < 10; i++) {
        tableSize16 *= clutPoints[i];
    }
    if (tableSize16 > 1000000) tableSize16 = 1000000;

    cmsUInt16Number *table16 = NULL;
    cmsFloat32Number *tableFloat = NULL;

    if (tableSize16 > 0) {
        table16 = (cmsUInt16Number *)malloc(tableSize16 * outputChan * sizeof(cmsUInt16Number));
        tableFloat = (cmsFloat32Number *)malloc(tableSize16 * outputChan * sizeof(cmsFloat32Number));
        if (table16) {
            memset(table16, 0, tableSize16 * outputChan * sizeof(cmsUInt16Number));
        }
        if (tableFloat) {
            memset(tableFloat, 0, tableSize16 * outputChan * sizeof(cmsFloat32Number));
        }
    }

    cmsStage *stage16 = NULL;
    cmsStage *stageFloat = NULL;
    cmsBool ret = FALSE;

    if (table16) {
        stage16 = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table16);
        if (stage16) {
            cmsUInt32Number cargo16 = g_counter;
            ret = cmsStageSampleCLut16bit(stage16, CALLBACK_Sampler16, &cargo16, dwFlags);
            (void)ret;
            cleanup_stage(stage16);
            stage16 = NULL;
        }
    }

    if (tableFloat) {
        stageFloat = cmsStageAllocCLutFloatGranular(ctx, clutPoints, inputChan, outputChan, tableFloat);
        if (stageFloat) {
            cmsUInt32Number cargoFloat = g_counter;
            ret = cmsStageSampleCLutFloat(stageFloat, CALLBACK_SamplerFloat, &cargoFloat, dwFlags);
            (void)ret;
            cleanup_stage(stageFloat);
            stageFloat = NULL;
        }
    }

    cmsUInt32Number cargoSlice16 = g_counter;
    ret = cmsSliceSpace16(nInputs, clutPoints, CALLBACK_Sampler16, &cargoSlice16);
    (void)ret;

    cmsUInt32Number cargoSliceFloat = g_counter;
    ret = cmsSliceSpaceFloat(nInputs, clutPoints, CALLBACK_SamplerFloat, &cargoSliceFloat);
    (void)ret;

    free(table16);
    free(tableFloat);

    cmsDeleteContext(ctx);

    g_counter++;
    return 0;
}