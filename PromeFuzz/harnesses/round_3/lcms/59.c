// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineGetPtrToFirstStage at cmslut.c:1646:21 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsPipeline* create_test_pipeline(cmsContext ctx, const uint8_t* data, size_t size, 
                                         cmsUInt32Number* inputChannels, cmsUInt32Number* outputChannels) {
    if (size < 2) return NULL;
    
    *inputChannels = (data[0] % 15) + 1;
    *outputChannels = (data[1] % 15) + 1;
    
    return cmsPipelineAlloc(ctx, *inputChannels, *outputChannels);
}

static void add_dummy_stage(cmsPipeline* pipeline, cmsUInt32Number inputChannels) {
    if (!pipeline) return;
    
    cmsToneCurve* curve = cmsBuildGamma(NULL, 2.2);
    if (curve) {
        cmsStage* stage = cmsStageAllocToneCurves(NULL, inputChannels, &curve);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        }
        cmsFreeToneCurve(curve);
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsUInt32Number inputChannels1, outputChannels1;
    cmsUInt32Number inputChannels2, outputChannels2;
    
    cmsPipeline* pipeline1 = create_test_pipeline(ctx, Data, Size, &inputChannels1, &outputChannels1);
    if (!pipeline1) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    add_dummy_stage(pipeline1, inputChannels1);
    
    if (Size >= 4) {
        cmsPipeline* pipeline2 = create_test_pipeline(ctx, Data + 2, Size - 2, &inputChannels2, &outputChannels2);
        if (pipeline2) {
            add_dummy_stage(pipeline2, inputChannels2);
            
            cmsPipelineCat(pipeline1, pipeline2);
            
            cmsPipelineSetSaveAs8bitsFlag(pipeline1, (Data[0] & 1));
            
            cmsPipeline* pipeline3 = cmsPipelineDup(pipeline1);
            if (pipeline3) {
                cmsStage* firstStage = cmsPipelineGetPtrToFirstStage(pipeline3);
                (void)firstStage;
                
                size_t minEvalSize = inputChannels1 * 2;
                if (Size >= 4 + minEvalSize) {
                    const uint8_t* evalData = Data + 4;
                    size_t evalSize = Size - 4;
                    
                    cmsUInt16Number* in = (cmsUInt16Number*)malloc(inputChannels1 * sizeof(cmsUInt16Number));
                    cmsUInt16Number* out = (cmsUInt16Number*)malloc(outputChannels1 * sizeof(cmsUInt16Number));
                    
                    if (in && out) {
                        size_t maxPairs = evalSize / 2;
                        if (maxPairs > inputChannels1) {
                            maxPairs = inputChannels1;
                        }
                        
                        for (size_t i = 0; i < maxPairs; i++) {
                            in[i] = (evalData[i * 2] << 8) | evalData[i * 2 + 1];
                        }
                        
                        for (size_t i = maxPairs; i < inputChannels1; i++) {
                            in[i] = 0;
                        }
                        
                        cmsPipelineEval16(in, out, pipeline1);
                        cmsPipelineEval16(in, out, pipeline3);
                    }
                    
                    free(in);
                    free(out);
                }
                
                cmsPipelineFree(pipeline3);
            }
            
            cmsPipelineFree(pipeline2);
        }
    }
    
    cmsPipelineSetSaveAs8bitsFlag(pipeline1, (Data[1] & 1));
    
    cmsPipelineFree(pipeline1);
    cmsDeleteContext(ctx);
    
    return 0;
}