// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsPipelineOutputChannels at cmslut.c:1419:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsPipeline* create_test_pipeline(cmsContext ctx, int input_channels, int output_channels) {
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, input_channels, output_channels);
    if (!pipeline) return NULL;
    
    // Add an identity stage to make the pipeline valid
    cmsStage* identity = cmsStageAllocIdentity(ctx, input_channels);
    if (!identity) {
        cmsPipelineFree(pipeline);
        return NULL;
    }
    cmsPipelineInsertStage(pipeline, cmsAT_END, identity);
    
    return pipeline;
}

static void init_test_data_16(cmsUInt16Number* in, const uint8_t* data, size_t size, int channels) {
    for (int i = 0; i < channels; i++) {
        if (size >= 2) {
            in[i] = (data[0] << 8) | data[1];
            data += 2;
            size -= 2;
        } else {
            in[i] = 0;
        }
    }
}

static void init_test_data_float(cmsFloat32Number* in, const uint8_t* data, size_t size, int channels) {
    for (int i = 0; i < channels; i++) {
        if (size >= 4) {
            uint32_t val;
            memcpy(&val, data, 4);
            in[i] = *(cmsFloat32Number*)&val;
            data += 4;
            size -= 4;
        } else {
            in[i] = 0.0f;
        }
    }
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create test pipelines with varying channel counts based on input data
    int input_channels = (Data[0] % 8) + 1;
    int output_channels = ((Data[0] >> 3) % 8) + 1;
    
    cmsPipeline* pipeline1 = create_test_pipeline(ctx, input_channels, output_channels);
    cmsPipeline* pipeline2 = create_test_pipeline(ctx, output_channels, input_channels);
    
    if (!pipeline1 || !pipeline2) {
        if (pipeline1) cmsPipelineFree(pipeline1);
        if (pipeline2) cmsPipelineFree(pipeline2);
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Use fuzzer data to determine which operations to perform
    size_t data_offset = 1;
    size_t remaining = Size - 1;
    
    while (remaining > 0) {
        uint8_t op = Data[data_offset] % 6;
        data_offset++;
        remaining--;
        
        switch (op) {
            case 0: {
                // cmsPipelineSetSaveAs8bitsFlag
                cmsBool flag = (remaining > 0) ? (Data[data_offset] & 1) : 0;
                cmsPipelineSetSaveAs8bitsFlag(pipeline1, flag);
                if (remaining > 0) {
                    data_offset++;
                    remaining--;
                }
                break;
            }
            
            case 1: {
                // cmsPipelineEval16
                // Get current channel counts after potential concatenations
                int current_input_channels = cmsPipelineInputChannels(pipeline1);
                int current_output_channels = cmsPipelineOutputChannels(pipeline1);
                
                cmsUInt16Number* in = (cmsUInt16Number*)malloc(current_input_channels * sizeof(cmsUInt16Number));
                cmsUInt16Number* out = (cmsUInt16Number*)malloc(current_output_channels * sizeof(cmsUInt16Number));
                if (in && out) {
                    init_test_data_16(in, Data + data_offset, remaining, current_input_channels);
                    cmsPipelineEval16(in, out, pipeline1);
                    free(in);
                    free(out);
                } else {
                    if (in) free(in);
                    if (out) free(out);
                }
                break;
            }
            
            case 2: {
                // cmsPipelineCat
                // Check if concatenation is valid (input channels of l2 must match output channels of l1)
                int pipeline1_output = cmsPipelineOutputChannels(pipeline1);
                int pipeline2_input = cmsPipelineInputChannels(pipeline2);
                
                if (pipeline1_output == pipeline2_input) {
                    cmsPipelineCat(pipeline1, pipeline2);
                }
                break;
            }
            
            case 3: {
                // cmsPipelineGetPtrToFirstStage
                cmsStage* first_stage = cmsPipelineGetPtrToFirstStage(pipeline1);
                (void)first_stage; // Use variable to avoid unused warning
                break;
            }
            
            case 4: {
                // cmsPipelineDup
                cmsPipeline* dup = cmsPipelineDup(pipeline1);
                if (dup) {
                    // Properly free the duplicated pipeline
                    cmsPipelineFree(dup);
                }
                break;
            }
            
            case 5: {
                // cmsPipelineEvalFloat
                // Get current channel counts after potential concatenations
                int current_input_channels = cmsPipelineInputChannels(pipeline1);
                int current_output_channels = cmsPipelineOutputChannels(pipeline1);
                
                cmsFloat32Number* in = (cmsFloat32Number*)malloc(current_input_channels * sizeof(cmsFloat32Number));
                cmsFloat32Number* out = (cmsFloat32Number*)malloc(current_output_channels * sizeof(cmsFloat32Number));
                if (in && out) {
                    init_test_data_float(in, Data + data_offset, remaining, current_input_channels);
                    cmsPipelineEvalFloat(in, out, pipeline1);
                    free(in);
                    free(out);
                } else {
                    if (in) free(in);
                    if (out) free(out);
                }
                break;
            }
        }
        
        // Advance data pointer based on operation
        if (op == 1 || op == 5) {
            // Get current channel counts for data consumption
            int current_input_channels = cmsPipelineInputChannels(pipeline1);
            size_t needed = (op == 1) ? (current_input_channels * 2) : (current_input_channels * 4);
            if (remaining >= needed) {
                data_offset += needed;
                remaining -= needed;
            } else {
                remaining = 0;
            }
        }
    }
    
    // Cleanup - ensure all pipelines are properly freed
    cmsPipelineFree(pipeline1);
    cmsPipelineFree(pipeline2);
    cmsDeleteContext(ctx);
    
    return 0;
}