// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

// Dummy evaluation function for placeholder stage with correct signature
static void dummy_eval(const float* In, float* Out, const cmsStage* mpe) {
    (void)In;
    (void)Out;
    (void)mpe;
}

// Dummy duplication function for placeholder stage with correct signature
static void* dummy_dup(cmsStage* mpe) {
    (void)mpe;
    return NULL;
}

// Dummy free function for placeholder stage with correct signature
static void dummy_free(cmsStage* mpe) {
    (void)mpe;
}

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Use first byte to determine which function to test
    uint8_t selector = Data[0] % 6;
    Data++;
    Size--;
    
    switch (selector) {
        case 0: { // cmsStageAllocToneCurves
            if (Size < 4) break;
            
            cmsUInt32Number nChannels = (Data[0] % 8) + 1; // 1-8 channels
            cmsToneCurve* curves[8];
            
            // Create dummy tone curves
            for (cmsUInt32Number i = 0; i < nChannels && i < 8; i++) {
                cmsUInt16Number table[256];
                for (int j = 0; j < 256; j++) {
                    table[j] = (cmsUInt16Number)(j * 257); // Simple linear curve
                }
                curves[i] = cmsBuildTabulatedToneCurve16(ctx, 256, table);
                if (!curves[i]) {
                    // Clean up previously created curves
                    for (cmsUInt32Number k = 0; k < i; k++) {
                        cmsFreeToneCurve(curves[k]);
                    }
                    cmsDeleteContext(ctx);
                    return 0;
                }
            }
            
            cmsStage* stage = cmsStageAllocToneCurves(ctx, nChannels, curves);
            if (stage) {
                cmsContext stageCtx = cmsGetStageContextID(stage);
                (void)stageCtx; // Use the context ID
                cmsStageFree(stage);
            }
            
            // Clean up tone curves
            for (cmsUInt32Number i = 0; i < nChannels && i < 8; i++) {
                cmsFreeToneCurve(curves[i]);
            }
            break;
        }
        
        case 1: { // _cmsStageAllocPlaceholder
            if (Size < 4) break;
            
            cmsUInt32Number inputChan = (Data[0] % 8) + 1;
            cmsUInt32Number outputChan = (Data[1] % 8) + 1;
            cmsStageSignature type = (cmsStageSignature)(Data[2] | (Data[3] << 8));
            
            void* userData = NULL;
            if (Size > 8) {
                userData = malloc(Size - 4);
                if (userData) {
                    memcpy(userData, Data + 4, Size - 4);
                }
            }
            
            cmsStage* stage = _cmsStageAllocPlaceholder(ctx, type, inputChan, outputChan,
                                                       dummy_eval, dummy_dup, dummy_free, userData);
            if (stage) {
                cmsContext stageCtx = cmsGetStageContextID(stage);
                (void)stageCtx;
                cmsStageFree(stage);
            } else {
                free(userData);
            }
            break;
        }
        
        case 2: { // cmsStageAllocCLut16bit
            if (Size < 8) break;
            
            cmsUInt32Number nGridPoints = (Data[0] % 16) + 2; // 2-17 grid points
            cmsUInt32Number inputChan = (Data[1] % 8) + 1;
            cmsUInt32Number outputChan = (Data[2] % 8) + 1;
            
            // Calculate table size
            size_t tableEntries = 1;
            for (cmsUInt32Number i = 0; i < inputChan; i++) {
                tableEntries *= nGridPoints;
            }
            tableEntries *= outputChan;
            
            if (tableEntries > 1000000) break; // Avoid excessive memory usage
            
            cmsUInt16Number* table = (cmsUInt16Number*)malloc(tableEntries * sizeof(cmsUInt16Number));
            if (!table) break;
            
            // Fill table with data from fuzzer input
            size_t tableSize = tableEntries * sizeof(cmsUInt16Number);
            size_t copySize = (Size - 3 < tableSize) ? Size - 3 : tableSize;
            if (copySize > 0) {
                memcpy(table, Data + 3, copySize);
                // Fill remaining with zeros if needed
                if (copySize < tableSize) {
                    memset((uint8_t*)table + copySize, 0, tableSize - copySize);
                }
            } else {
                memset(table, 0, tableSize);
            }
            
            cmsStage* stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table);
            if (stage) {
                cmsContext stageCtx = cmsGetStageContextID(stage);
                (void)stageCtx;
                cmsStageFree(stage);
            }
            
            free(table);
            break;
        }
        
        case 3: { // cmsStageAllocCLut16bitGranular
            if (Size < 8) break;
            
            cmsUInt32Number inputChan = (Data[0] % 8) + 1;
            cmsUInt32Number outputChan = (Data[1] % 8) + 1;
            
            cmsUInt32Number* clutPoints = (cmsUInt32Number*)malloc(inputChan * sizeof(cmsUInt32Number));
            if (!clutPoints) break;
            
            // Set grid points for each input channel
            for (cmsUInt32Number i = 0; i < inputChan; i++) {
                clutPoints[i] = (Data[2 + i] % 16) + 2; // 2-17 grid points
            }
            
            // Calculate table size
            size_t tableEntries = 1;
            for (cmsUInt32Number i = 0; i < inputChan; i++) {
                tableEntries *= clutPoints[i];
            }
            tableEntries *= outputChan;
            
            if (tableEntries > 1000000) { // Avoid excessive memory usage
                free(clutPoints);
                break;
            }
            
            cmsUInt16Number* table = (cmsUInt16Number*)malloc(tableEntries * sizeof(cmsUInt16Number));
            if (!table) {
                free(clutPoints);
                break;
            }
            
            // Fill table with data from fuzzer input
            size_t offset = 2 + inputChan;
            if (offset >= Size) offset = Size - 1;
            
            size_t tableSize = tableEntries * sizeof(cmsUInt16Number);
            size_t copySize = (Size - offset < tableSize) ? Size - offset : tableSize;
            if (copySize > 0) {
                memcpy(table, Data + offset, copySize);
                if (copySize < tableSize) {
                    memset((uint8_t*)table + copySize, 0, tableSize - copySize);
                }
            } else {
                memset(table, 0, tableSize);
            }
            
            cmsStage* stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table);
            if (stage) {
                cmsContext stageCtx = cmsGetStageContextID(stage);
                (void)stageCtx;
                cmsStageFree(stage);
            }
            
            free(table);
            free(clutPoints);
            break;
        }
        
        case 4: { // cmsGetStageContextID
            if (Size < 2) break;
            
            cmsUInt32Number nChannels = (Data[0] % 8) + 1;
            
            // Create a simple stage to get its context
            cmsStage* stage = cmsStageAllocIdentity(ctx, nChannels);
            if (stage) {
                cmsContext stageCtx = cmsGetStageContextID(stage);
                (void)stageCtx;
                cmsStageFree(stage);
            }
            break;
        }
        
        case 5: { // cmsStageAllocIdentity
            if (Size < 1) break;
            
            cmsUInt32Number nChannels = (Data[0] % 8) + 1;
            cmsStage* stage = cmsStageAllocIdentity(ctx, nChannels);
            if (stage) {
                cmsContext stageCtx = cmsGetStageContextID(stage);
                (void)stageCtx;
                cmsStageFree(stage);
            }
            break;
        }
    }
    
    cmsDeleteContext(ctx);
    return 0;
}