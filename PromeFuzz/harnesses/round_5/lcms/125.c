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
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
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

// Dummy evaluation function for placeholder stage
static void dummy_eval(const cmsFloat32Number* In, cmsFloat32Number* Out, const cmsStage* mpe) {
    (void)mpe;
    if (In && Out) {
        for (int i = 0; i < 4; i++) {
            Out[i] = In[i];
        }
    }
}

// Dummy duplication function for placeholder stage
static void* dummy_dup(cmsStage* mpe) {
    (void)mpe;
    return NULL;
}

// Dummy free function for placeholder stage
static void dummy_free(cmsStage* mpe) {
    (void)mpe;
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Use first byte to select different code paths
    uint8_t selector = Data[0];
    
    // Test cmsStageAllocToneCurves
    if ((selector & 0x1) && Size >= 8) {
        cmsUInt32Number nChannels = (Data[1] % 16) + 1; // 1-16 channels
        cmsToneCurve* curves[16];
        
        // Create dummy tone curves
        for (cmsUInt32Number i = 0; i < nChannels && i < 16; i++) {
            cmsUInt16Number table[256];
            for (int j = 0; j < 256; j++) {
                table[j] = (j * 257) % 65535;
            }
            curves[i] = cmsBuildTabulatedToneCurve16(context, 256, table);
        }
        
        cmsStage* stage = cmsStageAllocToneCurves(context, nChannels, curves);
        if (stage) {
            cmsContext stageContext = cmsGetStageContextID(stage);
            (void)stageContext;
            cmsStageFree(stage);
        }
        
        // Clean up tone curves
        for (cmsUInt32Number i = 0; i < nChannels && i < 16; i++) {
            if (curves[i]) cmsFreeToneCurve(curves[i]);
        }
    }
    
    // Test _cmsStageAllocPlaceholder
    if ((selector & 0x2) && Size >= 12) {
        cmsStageSignature type = (cmsStageSignature)(Data[2] | (Data[3] << 8));
        cmsUInt32Number inputChan = (Data[4] % 16) + 1;
        cmsUInt32Number outputChan = (Data[5] % 16) + 1;
        
        cmsStage* stage = _cmsStageAllocPlaceholder(context, type, inputChan, outputChan,
                                                   dummy_eval, dummy_dup, dummy_free, NULL);
        if (stage) {
            cmsContext stageContext = cmsGetStageContextID(stage);
            (void)stageContext;
            cmsStageFree(stage);
        }
    }
    
    // Test cmsStageAllocCLut16bit
    if ((selector & 0x4) && Size >= 16) {
        cmsUInt32Number nGridPoints = (Data[6] % 32) + 2; // 2-33 grid points
        cmsUInt32Number inputChan = (Data[7] % 8) + 1;
        cmsUInt32Number outputChan = (Data[8] % 8) + 1;
        
        // Calculate table size
        size_t tableSize = 1;
        for (cmsUInt32Number i = 0; i < inputChan; i++) {
            tableSize *= nGridPoints;
        }
        tableSize *= outputChan;
        
        if (tableSize > 0 && tableSize <= 1024 && Size >= 16 + tableSize * 2) {
            cmsUInt16Number* table = (cmsUInt16Number*)malloc(tableSize * sizeof(cmsUInt16Number));
            if (table) {
                // Fill table with data from fuzzer input
                for (size_t i = 0; i < tableSize && 16 + i * 2 + 1 < Size; i++) {
                    table[i] = (Data[16 + i * 2] << 8) | Data[16 + i * 2 + 1];
                }
                
                cmsStage* stage = cmsStageAllocCLut16bit(context, nGridPoints, inputChan, outputChan, table);
                if (stage) {
                    cmsContext stageContext = cmsGetStageContextID(stage);
                    (void)stageContext;
                    cmsStageFree(stage);
                }
                free(table);
            }
        }
    }
    
    // Test cmsStageAllocCLut16bitGranular
    if ((selector & 0x8) && Size >= 32) {
        cmsUInt32Number inputChan = (Data[9] % 8) + 1;
        cmsUInt32Number outputChan = (Data[10] % 8) + 1;
        cmsUInt32Number clutPoints[8];
        
        // Initialize clut points
        size_t tableSize = 1;
        for (cmsUInt32Number i = 0; i < inputChan; i++) {
            clutPoints[i] = (Data[11 + i] % 16) + 2; // 2-17 grid points per channel
            tableSize *= clutPoints[i];
        }
        tableSize *= outputChan;
        
        if (tableSize > 0 && tableSize <= 1024 && Size >= 32 + tableSize * 2) {
            cmsUInt16Number* table = (cmsUInt16Number*)malloc(tableSize * sizeof(cmsUInt16Number));
            if (table) {
                // Fill table with data from fuzzer input
                for (size_t i = 0; i < tableSize && 32 + i * 2 + 1 < Size; i++) {
                    table[i] = (Data[32 + i * 2] << 8) | Data[32 + i * 2 + 1];
                }
                
                cmsStage* stage = cmsStageAllocCLut16bitGranular(context, clutPoints, inputChan, outputChan, table);
                if (stage) {
                    cmsContext stageContext = cmsGetStageContextID(stage);
                    (void)stageContext;
                    cmsStageFree(stage);
                }
                free(table);
            }
        }
    }
    
    // Test cmsStageAllocIdentity
    if ((selector & 0x10)) {
        cmsUInt32Number nChannels = (Data[11] % 16) + 1;
        cmsStage* stage = cmsStageAllocIdentity(context, nChannels);
        if (stage) {
            cmsContext stageContext = cmsGetStageContextID(stage);
            (void)stageContext;
            cmsStageFree(stage);
        }
    }
    
    cmsDeleteContext(context);
    return 0;
}