// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineEval16 at cmslut.c:1447:16 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"

#define MAX_CHANNELS 16

static cmsPipeline* create_test_pipeline(cmsContext ctx, uint8_t input_channels, uint8_t output_channels) {
    if (input_channels == 0) input_channels = 1;
    if (output_channels == 0) output_channels = 1;
    if (input_channels > MAX_CHANNELS) input_channels = MAX_CHANNELS;
    if (output_channels > MAX_CHANNELS) output_channels = MAX_CHANNELS;
    
    return cmsPipelineAlloc(ctx, input_channels, output_channels);
}

static void populate_input_array_float(const uint8_t* data, size_t size, cmsFloat32Number* array, size_t channels) {
    for (size_t i = 0; i < channels; i++) {
        if (i < size) {
            array[i] = ((float)data[i]) / 255.0f;
        } else {
            array[i] = 0.0f;
        }
    }
}

static void populate_input_array_16(const uint8_t* data, size_t size, cmsUInt16Number* array, size_t channels) {
    for (size_t i = 0; i < channels; i++) {
        if (i * 2 + 1 < size) {
            uint16_t val = (data[i * 2] << 8) | data[i * 2 + 1];
            array[i] = val;
        } else {
            array[i] = 0;
        }
    }
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = NULL;
    
    uint8_t input_channels1 = Data[0] % (MAX_CHANNELS + 1);
    uint8_t output_channels1 = Data[1] % (MAX_CHANNELS + 1);
    uint8_t input_channels2 = Data[2] % (MAX_CHANNELS + 1);
    uint8_t output_channels2 = Data[3] % (MAX_CHANNELS + 1);
    
    cmsPipeline* pipeline1 = create_test_pipeline(ctx, input_channels1, output_channels1);
    if (!pipeline1) return 0;
    
    cmsPipeline* pipeline2 = create_test_pipeline(ctx, input_channels2, output_channels2);
    
    if (pipeline2) {
        cmsBool cat_result = cmsPipelineCat(pipeline1, pipeline2);
        (void)cat_result;
    }
    
    cmsBool prev_flag = cmsPipelineSetSaveAs8bitsFlag(pipeline1, Data[0] & 1);
    (void)prev_flag;
    
    if (input_channels1 > 0 && output_channels1 > 0) {
        cmsFloat32Number in_float[MAX_CHANNELS];
        cmsFloat32Number out_float[MAX_CHANNELS];
        
        populate_input_array_float(Data, Size, in_float, input_channels1);
        cmsPipelineEvalFloat(in_float, out_float, pipeline1);
        
        cmsUInt16Number in_16[MAX_CHANNELS];
        cmsUInt16Number out_16[MAX_CHANNELS];
        
        populate_input_array_16(Data, Size, in_16, input_channels1);
        cmsPipelineEval16(in_16, out_16, pipeline1);
    }
    
    cmsPipeline* dup_pipeline = cmsPipelineDup(pipeline1);
    if (dup_pipeline) {
        cmsPipelineFree(dup_pipeline);
    }
    
    if (pipeline2) {
        cmsPipelineFree(pipeline2);
    }
    
    cmsPipelineFree(pipeline1);
    
    return 0;
}