// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void free_tone_curves(cmsToneCurve **curves, int nChannels) {
    if (curves == NULL) return;
    for (int i = 0; i < nChannels; i++) {
        if (curves[i] != NULL) {
            cmsFreeToneCurve(curves[i]);
            curves[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // cmsCreateContext requires two arguments: Plugin and UserData
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    uint16_t val = ((uint16_t)Data[0] << 8) | Data[1];
    int nChannels = (val % 5) + 1; // 1 to 5 channels
    int gridPoints = (val % 10) + 2; // 2 to 11 grid points
    int inputChan = (val % 5) + 1;
    int outputChan = (val % 5) + 1;

    // Limit channels to reasonable max for safety
    if (nChannels > 8) nChannels = 8;
    if (inputChan > 8) inputChan = 8;
    if (outputChan > 8) outputChan = 8;
    if (gridPoints > 32) gridPoints = 32;

    cmsStage *stage = NULL;

    // Fuzz cmsStageAllocToneCurves
    if (Size > 4) {
        cmsToneCurve **curves = (cmsToneCurve **)malloc(nChannels * sizeof(cmsToneCurve *));
        if (curves != NULL) {
            memset(curves, 0, nChannels * sizeof(cmsToneCurve *));
            for (int i = 0; i < nChannels; i++) {
                // Use cmsBuildGamma which is available in lcms2.h
                cmsToneCurve *curve = cmsBuildGamma(ContextID, (cmsFloat32Number)Data[2] / 255.0);
                if (curve == NULL) {
                    curve = cmsBuildGamma(ContextID, 1.0f);
                }
                curves[i] = curve;
            }
            stage = cmsStageAllocToneCurves(ContextID, nChannels, curves);
            free_tone_curves(curves, nChannels);
            free(curves);
        }
    }

    // Fuzz cmsStageAllocCLut16bit
    if (Size > 6) {
        size_t tableSize = (size_t)inputChan * outputChan;
        for (int i = 0; i < gridPoints; i++) tableSize *= gridPoints;
        if (tableSize > 100000) tableSize = 100000; // Cap memory

        cmsUInt16Number *table16 = (cmsUInt16Number *)malloc(tableSize * sizeof(cmsUInt16Number));
        if (table16 != NULL) {
            memset(table16, 0, tableSize * sizeof(cmsUInt16Number));
            for (size_t i = 0; i < tableSize && i < Size; i++) {
                table16[i] = (cmsUInt16Number)Data[i + 2];
            }
            stage = cmsStageAllocCLut16bit(ContextID, gridPoints, inputChan, outputChan, table16);
            free(table16);
        }
    }

    // Fuzz cmsStageAllocCLutFloat
    if (Size > 6) {
        size_t tableSize = (size_t)inputChan * outputChan;
        for (int i = 0; i < gridPoints; i++) tableSize *= gridPoints;
        if (tableSize > 100000) tableSize = 100000;

        cmsFloat32Number *tableFloat = (cmsFloat32Number *)malloc(tableSize * sizeof(cmsFloat32Number));
        if (tableFloat != NULL) {
            memset(tableFloat, 0, tableSize * sizeof(cmsFloat32Number));
            for (size_t i = 0; i < tableSize && i < Size; i++) {
                tableFloat[i] = (cmsFloat32Number)Data[i + 2] / 255.0f;
            }
            stage = cmsStageAllocCLutFloat(ContextID, gridPoints, inputChan, outputChan, tableFloat);
            free(tableFloat);
        }
    }

    // Fuzz cmsStageAllocIdentity
    stage = cmsStageAllocIdentity(ContextID, nChannels);

    // Fuzz cmsStageAllocCLut16bitGranular
    if (Size > 8) {
        cmsUInt32Number *clutPoints = (cmsUInt32Number *)malloc(inputChan * sizeof(cmsUInt32Number));
        if (clutPoints != NULL) {
            for (int i = 0; i < inputChan; i++) {
                clutPoints[i] = (Data[2 + i] % 10) + 2;
            }
            size_t tableSize = (size_t)outputChan;
            for (int i = 0; i < inputChan; i++) tableSize *= clutPoints[i];
            if (tableSize > 100000) tableSize = 100000;

            cmsUInt16Number *table16 = (cmsUInt16Number *)malloc(tableSize * sizeof(cmsUInt16Number));
            if (table16 != NULL) {
                memset(table16, 0, tableSize * sizeof(cmsUInt16Number));
                for (size_t i = 0; i < tableSize && i < Size; i++) {
                    table16[i] = (cmsUInt16Number)Data[i + 2];
                }
                stage = cmsStageAllocCLut16bitGranular(ContextID, clutPoints, inputChan, outputChan, table16);
                free(table16);
            }
            free(clutPoints);
        }
    }

    // Fuzz cmsStageAllocCLutFloatGranular
    if (Size > 8) {
        cmsUInt32Number *clutPoints = (cmsUInt32Number *)malloc(inputChan * sizeof(cmsUInt32Number));
        if (clutPoints != NULL) {
            for (int i = 0; i < inputChan; i++) {
                clutPoints[i] = (Data[2 + i] % 10) + 2;
            }
            size_t tableSize = (size_t)outputChan;
            for (int i = 0; i < inputChan; i++) tableSize *= clutPoints[i];
            if (tableSize > 100000) tableSize = 100000;

            cmsFloat32Number *tableFloat = (cmsFloat32Number *)malloc(tableSize * sizeof(cmsFloat32Number));
            if (tableFloat != NULL) {
                memset(tableFloat, 0, tableSize * sizeof(cmsFloat32Number));
                for (size_t i = 0; i < tableSize && i < Size; i++) {
                    tableFloat[i] = (cmsFloat32Number)Data[i + 2] / 255.0f;
                }
                stage = cmsStageAllocCLutFloatGranular(ContextID, clutPoints, inputChan, outputChan, tableFloat);
                free(tableFloat);
            }
            free(clutPoints);
        }
    }

    // Cleanup
    if (stage != NULL) {
        cmsStageFree(stage);
    }
    cmsDeleteContext(ContextID);

    return 0;
}