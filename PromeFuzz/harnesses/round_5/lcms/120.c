// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsPipeline* create_test_pipeline(cmsContext ctx, cmsUInt32Number inputChannels, cmsUInt32Number outputChannels) {
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, inputChannels, outputChannels);
    if (!pipeline) return NULL;
    
    // Create a simple identity matrix stage for testing
    cmsStage* stage = cmsStageAllocIdentity(ctx, inputChannels);
    if (!stage) {
        cmsPipelineFree(pipeline);
        return NULL;
    }
    
    cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
    return pipeline;
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Use fuzzer data to determine pipeline parameters
    cmsUInt32Number inputChannels = (Data[0] % 8) + 1;  // 1-8 channels
    cmsUInt32Number outputChannels = (Data[1] % 8) + 1; // 1-8 channels
    
    // Create first pipeline
    cmsPipeline* pipeline1 = create_test_pipeline(ctx, inputChannels, outputChannels);
    if (!pipeline1) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Create second pipeline for concatenation
    cmsUInt32Number inputChannels2 = outputChannels; // Must match output of first
    cmsUInt32Number outputChannels2 = (Data[2] % 8) + 1;
    cmsPipeline* pipeline2 = create_test_pipeline(ctx, inputChannels2, outputChannels2);
    
    // Test cmsPipelineSetSaveAs8bitsFlag
    cmsBool flagValue = (Data[3] & 1) ? TRUE : FALSE;
    cmsPipelineSetSaveAs8bitsFlag(pipeline1, flagValue);
    
    // Test cmsPipelineEval16
    if (Size >= 4 + inputChannels * 2) {
        cmsUInt16Number* in16 = (cmsUInt16Number*)malloc(inputChannels * sizeof(cmsUInt16Number));
        cmsUInt16Number* out16 = (cmsUInt16Number*)malloc(outputChannels * sizeof(cmsUInt16Number));
        
        if (in16 && out16) {
            for (cmsUInt32Number i = 0; i < inputChannels; i++) {
                size_t offset = 4 + i * 2;
                if (offset + 1 < Size) {
                    in16[i] = (Data[offset] << 8) | Data[offset + 1];
                } else {
                    in16[i] = 0;
                }
            }
            
            cmsPipelineEval16(in16, out16, pipeline1);
        }
        
        free(in16);
        free(out16);
    }
    
    // Test cmsPipelineCat if we have second pipeline
    if (pipeline2) {
        cmsPipelineCat(pipeline1, pipeline2);
    }
    
    // Test cmsPipelineGetPtrToFirstStage
    cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline1);
    
    // Test cmsPipelineDup
    cmsPipeline* dupPipeline = cmsPipelineDup(pipeline1);
    if (dupPipeline) {
        // Test evaluation on duplicated pipeline
        if (Size >= 4 + inputChannels * 4) {
            cmsFloat32Number* inFloat = (cmsFloat32Number*)malloc(inputChannels * sizeof(cmsFloat32Number));
            cmsFloat32Number* outFloat = (cmsFloat32Number*)malloc(outputChannels2 * sizeof(cmsFloat32Number));
            
            if (inFloat && outFloat) {
                for (cmsUInt32Number i = 0; i < inputChannels; i++) {
                    size_t offset = 4 + i * 4;
                    if (offset + 3 < Size) {
                        // Interpret bytes as float (may produce invalid floats, but that's okay for fuzzing)
                        uint32_t temp = 0;
                        for (int j = 0; j < 4; j++) {
                            if (offset + j < Size) {
                                temp = (temp << 8) | Data[offset + j];
                            }
                        }
                        inFloat[i] = *(cmsFloat32Number*)&temp;
                    } else {
                        inFloat[i] = 0.0f;
                    }
                }
                
                cmsPipelineEvalFloat(inFloat, outFloat, dupPipeline);
            }
            
            free(inFloat);
            free(outFloat);
        }
        
        cmsPipelineFree(dupPipeline);
    }
    
    // Cleanup
    if (pipeline2) cmsPipelineFree(pipeline2);
    cmsPipelineFree(pipeline1);
    cmsDeleteContext(ctx);
    
    return 0;
}