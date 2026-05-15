// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Dummy evaluation function for placeholder stage
static void dummy_eval(const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage* mpe) {
    // We don't know the actual channel count, so just copy first 16 channels
    for (int i = 0; i < 16; i++) {
        Out[i] = In[i];
    }
}

// Dummy duplicate function for placeholder stage
static void* dummy_dup(cmsStage* mpe) {
    // Return NULL as we don't actually duplicate anything
    return NULL;
}

// Dummy free function for placeholder stage
static void dummy_free(cmsStage* mpe) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Use first byte to determine which functions to test
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    // Test cmsStageAllocIdentity
    if (selector & 0x01) {
        if (offset + 1 <= Size) {
            cmsUInt32Number nChannels = (Data[offset] % 16) + 1;
            offset += 1;
            
            cmsStage* identity_stage = cmsStageAllocIdentity(context, nChannels);
            if (identity_stage) {
                // Test cmsGetStageContextID
                cmsContext stage_context = cmsGetStageContextID(identity_stage);
                (void)stage_context; // Use to avoid unused variable warning
                
                cmsStageFree(identity_stage);
            }
        }
    }
    
    // Test cmsStageAllocToneCurves
    if (selector & 0x02) {
        if (offset + 2 <= Size) {
            cmsUInt32Number nChannels = (Data[offset] % 8) + 1;
            offset += 1;
            
            // Create dummy tone curves
            cmsToneCurve* curves[8];
            int valid_curves = 0;
            
            for (cmsUInt32Number i = 0; i < nChannels && offset + 2 <= Size; i++) {
                cmsUInt16Number table[2] = {0, 65535};
                curves[i] = cmsBuildTabulatedToneCurve16(context, 2, table);
                if (curves[i]) {
                    valid_curves++;
                }
                offset += 2;
            }
            
            if (valid_curves == nChannels) {
                cmsStage* tone_curve_stage = cmsStageAllocToneCurves(context, nChannels, curves);
                if (tone_curve_stage) {
                    cmsStageFree(tone_curve_stage);
                }
            }
            
            // Free allocated tone curves
            for (cmsUInt32Number i = 0; i < nChannels; i++) {
                if (curves[i]) {
                    cmsFreeToneCurve(curves[i]);
                }
            }
        }
    }
    
    // Test cmsStageAllocCLut16bitGranular
    if (selector & 0x04) {
        if (offset + 3 <= Size) {
            cmsUInt32Number inputChan = (Data[offset] % 8) + 1;
            offset += 1;
            cmsUInt32Number outputChan = (Data[offset] % 8) + 1;
            offset += 1;
            
            cmsUInt32Number clutPoints[8];
            size_t total_points = 1;
            
            for (cmsUInt32Number i = 0; i < inputChan && offset + 1 <= Size; i++) {
                clutPoints[i] = (Data[offset] % 8) + 2; // 2-9 points
                offset += 1;
                total_points *= clutPoints[i];
            }
            
            // Calculate table size needed
            size_t table_size = total_points * outputChan;
            size_t bytes_needed = table_size * sizeof(cmsUInt16Number);
            
            if (bytes_needed > 0 && offset + bytes_needed <= Size) {
                const cmsUInt16Number* table = (const cmsUInt16Number*)(Data + offset);
                offset += bytes_needed;
                
                cmsStage* clut_stage = cmsStageAllocCLut16bitGranular(context, clutPoints, 
                                                                      inputChan, outputChan, table);
                if (clut_stage) {
                    cmsStageFree(clut_stage);
                }
            }
        }
    }
    
    // Test cmsStageAllocCLut16bit
    if (selector & 0x08) {
        if (offset + 4 <= Size) {
            cmsUInt32Number nGridPoints = (Data[offset] % 8) + 2; // 2-9 points
            offset += 1;
            cmsUInt32Number inputChan = (Data[offset] % 8) + 1;
            offset += 1;
            cmsUInt32Number outputChan = (Data[offset] % 8) + 1;
            offset += 1;
            
            // Calculate table size needed
            size_t table_size = 1;
            for (cmsUInt32Number i = 0; i < inputChan; i++) {
                table_size *= nGridPoints;
            }
            table_size *= outputChan;
            size_t bytes_needed = table_size * sizeof(cmsUInt16Number);
            
            if (bytes_needed > 0 && offset + bytes_needed <= Size) {
                const cmsUInt16Number* table = (const cmsUInt16Number*)(Data + offset);
                offset += bytes_needed;
                
                cmsStage* clut_stage = cmsStageAllocCLut16bit(context, nGridPoints, 
                                                              inputChan, outputChan, table);
                if (clut_stage) {
                    cmsStageFree(clut_stage);
                }
            }
        }
    }
    
    // Test _cmsStageAllocPlaceholder
    if (selector & 0x10) {
        if (offset + 4 <= Size) {
            cmsStageSignature type = (cmsStageSignature)(Data[offset] % 256);
            offset += 1;
            cmsUInt32Number inputChannels = (Data[offset] % 8) + 1;
            offset += 1;
            cmsUInt32Number outputChannels = (Data[offset] % 8) + 1;
            offset += 1;
            
            // Use remaining data for custom data if available
            void* custom_data = NULL;
            if (offset < Size) {
                size_t data_size = Size - offset;
                if (data_size > 0) {
                    custom_data = malloc(data_size);
                    if (custom_data) {
                        memcpy(custom_data, Data + offset, data_size);
                    }
                }
            }
            
            cmsStage* placeholder_stage = _cmsStageAllocPlaceholder(context, type,
                                                                    inputChannels, outputChannels,
                                                                    dummy_eval, dummy_dup, dummy_free,
                                                                    custom_data);
            if (placeholder_stage) {
                cmsStageFree(placeholder_stage);
            } else {
                // If stage creation failed, free the custom data
                if (custom_data) {
                    free(custom_data);
                }
            }
        }
    }
    
    cmsDeleteContext(context);
    return 0;
}