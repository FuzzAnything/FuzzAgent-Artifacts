// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsPipelineDup at cmslut.c:1464:24 in lcms2.h
// cmsPipelineCat at cmslut.c:1612:20 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineEvalFloat at cmslut.c:1455:16 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsPipeline* create_test_pipeline(cmsContext ctx, cmsUInt32Number input_channels, cmsUInt32Number output_channels) {
    return cmsPipelineAlloc(ctx, input_channels, output_channels);
}

static void cleanup_pipeline(cmsPipeline* pipeline) {
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
}

static cmsPipeline* duplicate_pipeline(const cmsPipeline* orig) {
    return cmsPipelineDup(orig);
}

static cmsBool concatenate_pipelines(cmsPipeline* l1, const cmsPipeline* l2) {
    return cmsPipelineCat(l1, l2);
}

static cmsBool set_save_as_8bits_flag(cmsPipeline* lut, cmsBool on) {
    return cmsPipelineSetSaveAs8bitsFlag(lut, on);
}

static void evaluate_pipeline_float(const cmsPipeline* lut, const cmsFloat32Number* in, cmsFloat32Number* out) {
    if (lut && in && out) {
        cmsPipelineEvalFloat(in, out, lut);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsContext ctx = NULL;
    cmsPipeline* pipeline1 = NULL;
    cmsPipeline* pipeline2 = NULL;
    cmsPipeline* pipeline3 = NULL;

    // Use first byte to determine input channels (1-16)
    cmsUInt32Number input_channels1 = (Data[0] % 16) + 1;
    // Use second byte to determine output channels (1-16)
    cmsUInt32Number output_channels1 = (Data[1] % 16) + 1;
    // Use third byte to determine input channels for second pipeline (1-16)
    cmsUInt32Number input_channels2 = (Data[2] % 16) + 1;
    // Use fourth byte to determine output channels for second pipeline (1-16)
    cmsUInt32Number output_channels2 = (Data[3] % 16) + 1;
    // Use fifth byte (if available) for boolean flag
    cmsBool flag = (Size > 4) ? (Data[4] % 2) : 0;

    // Create first pipeline
    pipeline1 = create_test_pipeline(ctx, input_channels1, output_channels1);
    if (!pipeline1) {
        goto cleanup;
    }

    // Set save as 8 bits flag
    set_save_as_8bits_flag(pipeline1, flag);

    // Create second pipeline
    pipeline2 = create_test_pipeline(ctx, input_channels2, output_channels2);
    if (!pipeline2) {
        goto cleanup;
    }

    // Duplicate first pipeline
    pipeline3 = duplicate_pipeline(pipeline1);
    if (pipeline3) {
        // Set flag on duplicated pipeline with opposite value
        set_save_as_8bits_flag(pipeline3, !flag);
        
        // Clean up duplicated pipeline
        cleanup_pipeline(pipeline3);
        pipeline3 = NULL;
    }

    // Try to concatenate pipelines (even if channel counts don't match)
    concatenate_pipelines(pipeline1, pipeline2);

    // Evaluate pipeline with float inputs if we have enough data
    if (pipeline1) {
        // Allocate arrays with maximum size we might need (16 channels)
        cmsFloat32Number in[16] = {0};
        cmsFloat32Number out[16] = {0};
        
        // Fill with available data (up to 16 channels)
        size_t data_offset = 4;
        if (Size > 4) data_offset++; // Skip the flag byte if it exists
        
        size_t channels_to_fill = (input_channels1 < 16) ? input_channels1 : 16;
        for (size_t i = 0; i < channels_to_fill && (data_offset + i) < Size; i++) {
            in[i] = (cmsFloat32Number)Data[data_offset + i] / 255.0f;
        }
        
        evaluate_pipeline_float(pipeline1, in, out);
    }

cleanup:
    cleanup_pipeline(pipeline1);
    cleanup_pipeline(pipeline2);
    
    return 0;
}