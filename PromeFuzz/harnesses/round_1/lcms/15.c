// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineEvalReverseFloat at cmslut.c:1753:19 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineCheckAndRetreiveStages at cmslut.c:110:20 in lcms2.h
// cmsPipelineUnlinkStage at cmslut.c:1555:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsPipelineInputChannels at cmslut.c:1413:27 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
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

static cmsPipeline* create_test_pipeline(cmsContext ctx, cmsUInt32Number input_channels, cmsUInt32Number output_channels) {
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, input_channels, output_channels);
    if (!pipeline) return NULL;
    
    // Create and insert some simple stages
    cmsStage* stage1 = cmsStageAllocIdentity(ctx, input_channels);
    if (stage1) {
        cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage1);
    }
    
    cmsStage* stage2 = cmsStageAllocIdentity(ctx, output_channels);
    if (stage2) {
        cmsPipelineInsertStage(pipeline, cmsAT_END, stage2);
    }
    
    return pipeline;
}

static cmsPipeline* parse_pipeline_from_data(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 2) return NULL;
    
    cmsUInt32Number input_channels = (data[0] % 8) + 1;
    cmsUInt32Number output_channels = (data[1] % 8) + 1;
    
    return create_test_pipeline(ctx, input_channels, output_channels);
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Parse pipeline from fuzz data
    cmsPipeline* pipeline = parse_pipeline_from_data(ctx, Data, Size);
    if (!pipeline) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Test cmsPipelineCheckAndRetreiveStages
    if (Size >= 3) {
        cmsStage* stage1 = NULL;
        cmsStage* stage2 = NULL;
        
        // Try to retrieve identity stages
        cmsStageSignature sig1 = cmsSigIdentityElemType;
        cmsStageSignature sig2 = cmsSigIdentityElemType;
        
        cmsPipelineCheckAndRetreiveStages(pipeline, 2, sig1, &stage1, sig2, &stage2);
    }
    
    // Test cmsPipelineUnlinkStage
    if (Size >= 4) {
        cmsStage* unlinked_stage = NULL;
        cmsStageLoc loc = (Data[3] % 2) ? cmsAT_BEGIN : cmsAT_END;
        cmsPipelineUnlinkStage(pipeline, loc, &unlinked_stage);
        
        // If we got a stage back, free it
        if (unlinked_stage) {
            cmsStageFree(unlinked_stage);
        }
    }
    
    // Test cmsPipelineInsertStage
    if (Size >= 5) {
        // Get input channels using API function
        cmsUInt32Number input_channels = cmsPipelineInputChannels(pipeline);
        cmsStage* new_stage = cmsStageAllocIdentity(ctx, input_channels);
        if (new_stage) {
            cmsStageLoc loc = (Data[4] % 2) ? cmsAT_BEGIN : cmsAT_END;
            cmsPipelineInsertStage(pipeline, loc, new_stage);
        }
    }
    
    // Test cmsPipelineEvalFloat and cmsPipelineEvalReverseFloat
    // Get input and output channels first
    cmsUInt32Number input_channels = cmsPipelineInputChannels(pipeline);
    cmsUInt32Number output_channels = cmsPipelineOutputChannels(pipeline);
    
    // Allocate arrays with appropriate sizes
    cmsFloat32Number* in = (cmsFloat32Number*)malloc(input_channels * sizeof(cmsFloat32Number));
    cmsFloat32Number* out = (cmsFloat32Number*)malloc(output_channels * sizeof(cmsFloat32Number));
    
    if (in && out && Size >= (input_channels + output_channels) * sizeof(cmsFloat32Number)) {
        // Initialize input array from fuzz data
        for (cmsUInt32Number i = 0; i < input_channels; i++) {
            if ((i * sizeof(cmsFloat32Number) + sizeof(cmsFloat32Number)) <= Size) {
                memcpy(&in[i], Data + (i * sizeof(cmsFloat32Number)), sizeof(cmsFloat32Number));
            } else {
                in[i] = 0.0f;
            }
        }
        
        // Test forward evaluation
        cmsPipelineEvalFloat(in, out, pipeline);
        
        // Test reverse evaluation (only for 3→3 or 4→3 LUTs)
        if (output_channels == 3 && (input_channels == 3 || input_channels == 4)) {
            cmsFloat32Number* target = (cmsFloat32Number*)malloc(output_channels * sizeof(cmsFloat32Number));
            cmsFloat32Number* result = (cmsFloat32Number*)malloc(input_channels * sizeof(cmsFloat32Number));
            cmsFloat32Number* hint = (cmsFloat32Number*)malloc(input_channels * sizeof(cmsFloat32Number));
            
            if (target && result && hint) {
                // Initialize target and hint arrays from fuzz data
                for (cmsUInt32Number i = 0; i < output_channels; i++) {
                    if ((i * sizeof(cmsFloat32Number) + sizeof(cmsFloat32Number)) <= Size) {
                        memcpy(&target[i], Data + (i * sizeof(cmsFloat32Number)), sizeof(cmsFloat32Number));
                    } else {
                        target[i] = 0.0f;
                    }
                }
                
                for (cmsUInt32Number i = 0; i < input_channels; i++) {
                    if ((i * sizeof(cmsFloat32Number) + sizeof(cmsFloat32Number)) <= Size) {
                        memcpy(&hint[i], Data + (i * sizeof(cmsFloat32Number)), sizeof(cmsFloat32Number));
                    } else {
                        hint[i] = 0.0f;
                    }
                }
                
                cmsPipelineEvalReverseFloat(target, result, hint, pipeline);
                
                free(target);
                free(result);
                free(hint);
            }
        }
    }
    
    // Clean up allocated arrays
    if (in) free(in);
    if (out) free(out);
    
    // Test cmsPipelineFree
    cmsPipelineFree(pipeline);
    
    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}