// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_dummy_tone_curve(cmsContext ctx) {
    // Create a simple gamma curve
    cmsFloat64Number gamma = 2.2;
    return cmsBuildGamma(ctx, gamma);
}

static void free_dummy_tone_curve(cmsToneCurve* curve) {
    if (curve) {
        cmsFreeToneCurve(curve);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Create a context for all operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Use first 4 bytes to determine parameters
    uint32_t param_seed;
    memcpy(&param_seed, Data, 4);
    
    // Limit channel counts to reasonable values
    cmsUInt32Number nChannels = (param_seed % 16) + 1;  // 1-16 channels
    cmsUInt32Number inputChan = (param_seed % 8) + 1;   // 1-8 input channels
    cmsUInt32Number outputChan = ((param_seed >> 8) % 8) + 1;  // 1-8 output channels
    cmsUInt32Number nGridPoints = ((param_seed >> 16) % 32) + 2;  // 2-33 grid points
    
    // Test cmsStageAllocIdentity
    cmsStage* identity_stage = cmsStageAllocIdentity(ctx, nChannels);
    if (identity_stage) {
        // Test cmsStageInputChannels on identity stage
        cmsUInt32Number input_channels = cmsStageInputChannels(identity_stage);
        
        // Test cmsStageOutputChannels on identity stage
        cmsUInt32Number output_channels = cmsStageOutputChannels(identity_stage);
        
        cmsStageFree(identity_stage);
    }
    
    // Test cmsStageAllocToneCurves
    cmsToneCurve* dummy_curve = create_dummy_tone_curve(ctx);
    if (dummy_curve) {
        cmsToneCurve* curves[16];  // Maximum 16 channels
        for (cmsUInt32Number i = 0; i < nChannels && i < 16; i++) {
            curves[i] = dummy_curve;
        }
        
        cmsStage* tone_curve_stage = cmsStageAllocToneCurves(ctx, nChannels, curves);
        if (tone_curve_stage) {
            // Test query functions on tone curve stage
            cmsUInt32Number tone_input = cmsStageInputChannels(tone_curve_stage);
            cmsUInt32Number tone_output = cmsStageOutputChannels(tone_curve_stage);
            
            cmsStageFree(tone_curve_stage);
        }
        
        free_dummy_tone_curve(dummy_curve);
    }
    
    // Test cmsStageAllocCLut16bitGranular
    cmsUInt32Number clutPoints[8];  // Maximum 8 input channels
    for (cmsUInt32Number i = 0; i < inputChan && i < 8; i++) {
        clutPoints[i] = nGridPoints + (i % 3);  // Vary grid points slightly
    }
    
    // Calculate table size needed
    size_t table_size = outputChan;
    for (cmsUInt32Number i = 0; i < inputChan && i < 8; i++) {
        table_size *= clutPoints[i];
    }
    
    // Allocate and fill table with fuzzer data
    cmsUInt16Number* table = NULL;
    if (table_size > 0 && Size >= 4 + table_size * sizeof(cmsUInt16Number)) {
        table = (cmsUInt16Number*)malloc(table_size * sizeof(cmsUInt16Number));
        if (table) {
            // Use fuzzer data for table values, wrapping around if needed
            for (size_t i = 0; i < table_size; i++) {
                size_t data_idx = 4 + (i * 2) % (Size - 4);
                if (data_idx + 1 < Size) {
                    table[i] = (Data[data_idx] << 8) | Data[data_idx + 1];
                } else {
                    table[i] = i % 65535;
                }
            }
            
            cmsStage* clut_granular_stage = cmsStageAllocCLut16bitGranular(ctx, clutPoints, inputChan, outputChan, table);
            if (clut_granular_stage) {
                // Test query functions
                cmsUInt32Number granular_input = cmsStageInputChannels(clut_granular_stage);
                cmsUInt32Number granular_output = cmsStageOutputChannels(clut_granular_stage);
                
                cmsStageFree(clut_granular_stage);
            }
            
            free(table);
        }
    }
    
    // Test cmsStageAllocCLut16bit
    // Calculate table size for uniform grid
    size_t uniform_table_size = outputChan;
    for (cmsUInt32Number i = 0; i < inputChan; i++) {
        uniform_table_size *= nGridPoints;
    }
    
    // Allocate and fill uniform table with fuzzer data
    cmsUInt16Number* uniform_table = NULL;
    if (uniform_table_size > 0 && Size >= 4 + uniform_table_size * sizeof(cmsUInt16Number)) {
        uniform_table = (cmsUInt16Number*)malloc(uniform_table_size * sizeof(cmsUInt16Number));
        if (uniform_table) {
            // Use fuzzer data for table values, wrapping around if needed
            for (size_t i = 0; i < uniform_table_size; i++) {
                size_t data_idx = 4 + (i * 2) % (Size - 4);
                if (data_idx + 1 < Size) {
                    uniform_table[i] = (Data[data_idx] << 8) | Data[data_idx + 1];
                } else {
                    uniform_table[i] = i % 65535;
                }
            }
            
            cmsStage* clut_uniform_stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, uniform_table);
            if (clut_uniform_stage) {
                // Test query functions
                cmsUInt32Number uniform_input = cmsStageInputChannels(clut_uniform_stage);
                cmsUInt32Number uniform_output = cmsStageOutputChannels(clut_uniform_stage);
                
                cmsStageFree(clut_uniform_stage);
            }
            
            free(uniform_table);
        }
    }
    
    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}