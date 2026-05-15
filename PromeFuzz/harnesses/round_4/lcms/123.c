// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_test_tone_curve(cmsContext context) {
    // Create a simple gamma curve
    cmsFloat64Number params[10] = {2.4, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    return cmsBuildParametricToneCurve(context, 1, params);
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Use input data to generate parameters
    cmsUInt32Number nGridPoints = 2 + (Data[0] % 10);  // 2-11
    cmsUInt32Number inputChan = 1 + (Data[1] % 8);     // 1-8
    cmsUInt32Number outputChan = 1 + (Data[2] % 8);    // 1-8
    cmsUInt32Number nChannels = 1 + (Data[3] % 8);     // 1-8
    
    // Test cmsStageAllocIdentity
    cmsStage* identity_stage = cmsStageAllocIdentity(context, nChannels);
    if (identity_stage) {
        cmsUInt32Number in_ch = cmsStageInputChannels(identity_stage);
        cmsUInt32Number out_ch = cmsStageOutputChannels(identity_stage);
        cmsStageFree(identity_stage);
    }
    
    // Test cmsStageAllocCLut16bit
    // Calculate table size correctly: nGridPoints^inputChan * outputChan
    size_t table_entries = 1;
    for (cmsUInt32Number i = 0; i < inputChan; i++) {
        if (table_entries > 1000000 / nGridPoints) {
            table_entries = 0;
            break;
        }
        table_entries *= nGridPoints;
    }
    
    if (table_entries > 0 && outputChan > 0) {
        if (table_entries > 1000000 / outputChan) {
            table_entries = 0;
        } else {
            table_entries *= outputChan;
        }
    }
    
    if (table_entries > 0 && table_entries < 1000000) {
        cmsUInt16Number* table = (cmsUInt16Number*)malloc(table_entries * sizeof(cmsUInt16Number));
        if (table) {
            // Initialize with data from fuzzer input
            size_t copy_size = Size < (table_entries * 2) ? Size : (table_entries * 2);
            for (size_t i = 0; i < copy_size/2 && i < table_entries; i++) {
                table[i] = (Data[(i*2) % Size] << 8) | Data[((i*2)+1) % Size];
            }
            // Fill remaining with zeros
            for (size_t i = copy_size/2; i < table_entries; i++) {
                table[i] = 0;
            }
            
            cmsStage* clut_stage = cmsStageAllocCLut16bit(context, nGridPoints, inputChan, outputChan, table);
            if (clut_stage) {
                cmsUInt32Number in_ch = cmsStageInputChannels(clut_stage);
                cmsUInt32Number out_ch = cmsStageOutputChannels(clut_stage);
                cmsStageFree(clut_stage);
            }
            free(table);
        }
    }
    
    // Test cmsStageAllocToneCurves
    cmsToneCurve* curve = create_test_tone_curve(context);
    if (curve) {
        cmsToneCurve* curves[8];
        for (cmsUInt32Number i = 0; i < nChannels && i < 8; i++) {
            curves[i] = curve;
        }
        
        cmsStage* tone_stage = cmsStageAllocToneCurves(context, nChannels, curves);
        if (tone_stage) {
            cmsUInt32Number in_ch = cmsStageInputChannels(tone_stage);
            cmsUInt32Number out_ch = cmsStageOutputChannels(tone_stage);
            cmsStageFree(tone_stage);
        }
        cmsFreeToneCurve(curve);
    }
    
    // Test cmsStageAllocCLut16bitGranular
    if (inputChan > 0 && inputChan <= 8) {
        cmsUInt32Number clutPoints[8];
        size_t total_grid_points = 1;
        int overflow = 0;
        
        for (cmsUInt32Number i = 0; i < inputChan; i++) {
            clutPoints[i] = 2 + ((Data[i] + i) % 6);  // 2-7
            if (total_grid_points > 1000000 / clutPoints[i]) {
                overflow = 1;
                break;
            }
            total_grid_points *= clutPoints[i];
        }
        
        if (!overflow && outputChan > 0) {
            if (total_grid_points > 1000000 / outputChan) {
                overflow = 1;
            } else {
                total_grid_points *= outputChan;
            }
        }
        
        if (!overflow && total_grid_points > 0 && total_grid_points < 1000000) {
            cmsUInt16Number* granular_table = (cmsUInt16Number*)malloc(total_grid_points * sizeof(cmsUInt16Number));
            if (granular_table) {
                size_t copy_size = Size < (total_grid_points * 2) ? Size : (total_grid_points * 2);
                for (size_t i = 0; i < copy_size/2 && i < total_grid_points; i++) {
                    granular_table[i] = (Data[(i*2) % Size] << 8) | Data[((i*2)+1) % Size];
                }
                // Fill remaining with zeros
                for (size_t i = copy_size/2; i < total_grid_points; i++) {
                    granular_table[i] = 0;
                }
                
                cmsStage* granular_stage = cmsStageAllocCLut16bitGranular(context, clutPoints, inputChan, outputChan, granular_table);
                if (granular_stage) {
                    cmsUInt32Number in_ch = cmsStageInputChannels(granular_stage);
                    cmsUInt32Number out_ch = cmsStageOutputChannels(granular_stage);
                    cmsStageFree(granular_stage);
                }
                free(granular_table);
            }
        }
    }
    
    cmsDeleteContext(context);
    return 0;
}