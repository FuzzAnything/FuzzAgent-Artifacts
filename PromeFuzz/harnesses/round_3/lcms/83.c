// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsToneCurve* create_dummy_tone_curve(cmsContext ctx) {
    // Create a simple tone curve with 256 entries
    cmsUInt16Number table[256];
    for (int i = 0; i < 256; i++) {
        table[i] = (cmsUInt16Number)(i * 257); // Simple linear ramp
    }
    return cmsBuildTabulatedToneCurve16(ctx, 256, table);
}

static void dummy_eval_fn(const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage* mpe) {
    for (int i = 0; i < 3; i++) {
        Out[i] = In[i];
    }
}

static void* dummy_dup_fn(cmsStage* mpe) {
    return NULL;
}

static void dummy_free_fn(cmsStage* mpe) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    Data++;
    Size--;
    
    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    // For clutPoints array
    cmsUInt32Number clutPoints[16];
    int num_clut_points = (selector % 8) + 1;
    if (num_clut_points > 16) num_clut_points = 16;
    
    for (int i = 0; i < num_clut_points && offset + 4 <= Size; i++) {
        memcpy(&clutPoints[i], Data + offset, 4);
        offset += 4;
    }
    
    // Extract other parameters
    cmsUInt32Number inputChan = 3;
    cmsUInt32Number outputChan = 3;
    cmsUInt32Number nGridPoints = 2;
    cmsUInt32Number nChannels = 3;
    
    if (offset + 4 <= Size) {
        memcpy(&inputChan, Data + offset, 4);
        offset += 4;
    }
    if (offset + 4 <= Size) {
        memcpy(&outputChan, Data + offset, 4);
        offset += 4;
    }
    if (offset + 4 <= Size) {
        memcpy(&nGridPoints, Data + offset, 4);
        offset += 4;
    }
    if (offset + 4 <= Size) {
        memcpy(&nChannels, Data + offset, 4);
        offset += 4;
    }
    
    // Limit parameters to reasonable values
    if (inputChan > 16) inputChan = 16;
    if (outputChan > 16) outputChan = 16;
    if (nGridPoints > 256) nGridPoints = 256;
    if (nChannels > 16) nChannels = 16;
    
    // Calculate table size for CLUT functions
    size_t table_size = 1;
    for (int i = 0; i < num_clut_points; i++) {
        if (clutPoints[i] > 256) clutPoints[i] = 256;
        table_size *= clutPoints[i];
    }
    table_size *= outputChan;
    
    size_t uniform_table_size = nGridPoints;
    for (int i = 0; i < inputChan; i++) {
        uniform_table_size *= nGridPoints;
    }
    uniform_table_size *= outputChan;
    
    // Allocate and fill table data
    cmsUInt16Number* table = NULL;
    if (table_size > 0 && table_size < 1000000) {
        table = (cmsUInt16Number*)malloc(table_size * sizeof(cmsUInt16Number));
        if (table) {
            size_t bytes_to_copy = table_size * sizeof(cmsUInt16Number);
            if (bytes_to_copy > Size - offset) {
                bytes_to_copy = Size - offset;
            }
            if (bytes_to_copy > 0) {
                memcpy(table, Data + offset, bytes_to_copy);
            }
            offset += bytes_to_copy;
        }
    }
    
    // Test cmsStageAllocCLut16bitGranular
    if (selector & 0x01) {
        cmsStage* stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table);
        if (stage) {
            cmsContext stage_ctx = cmsGetStageContextID(stage);
            cmsStageFree(stage);
        }
    }
    
    // Test cmsStageAllocIdentity
    if (selector & 0x02) {
        cmsStage* stage = cmsStageAllocIdentity(ctx, nChannels);
        if (stage) {
            cmsContext stage_ctx = cmsGetStageContextID(stage);
            cmsStageFree(stage);
        }
    }
    
    // Test _cmsStageAllocPlaceholder
    if (selector & 0x04) {
        cmsStage* stage = _cmsStageAllocPlaceholder(ctx, (cmsStageSignature)selector, 
                                                   inputChan, outputChan, 
                                                   dummy_eval_fn, dummy_dup_fn, dummy_free_fn, NULL);
        if (stage) {
            cmsContext stage_ctx = cmsGetStageContextID(stage);
            cmsStageFree(stage);
        }
    }
    
    // Test cmsStageAllocToneCurves
    if (selector & 0x08) {
        cmsToneCurve* curves[16];
        for (int i = 0; i < nChannels && i < 16; i++) {
            curves[i] = create_dummy_tone_curve(ctx);
        }
        
        cmsStage* stage = cmsStageAllocToneCurves(ctx, nChannels, curves);
        if (stage) {
            cmsContext stage_ctx = cmsGetStageContextID(stage);
            cmsStageFree(stage);
        }
        
        for (int i = 0; i < nChannels && i < 16; i++) {
            if (curves[i]) {
                cmsFreeToneCurve(curves[i]);
            }
        }
    }
    
    // Test cmsStageAllocCLut16bit
    if (selector & 0x10) {
        cmsStage* stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table);
        if (stage) {
            cmsContext stage_ctx = cmsGetStageContextID(stage);
            cmsStageFree(stage);
        }
    }
    
    // Cleanup
    if (table) {
        free(table);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}