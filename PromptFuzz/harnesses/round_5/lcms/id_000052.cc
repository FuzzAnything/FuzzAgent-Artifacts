#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <lcms2.h>
}
//<ID> 295
//<Prompt> []
/*<Combination>: [
*/
//<score> 87, nr_unique_branch: 2
//<Quality> {"density":29,"unique_branches":{"GetXFormColorSpaces":[[1023,9,1023,29,0,0,4,0]],"cmsCreateExtendedTransform":[[1168,9,1168,86,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenIOhandlerFromMem","cmsDeleteContext","cmsDictAlloc","cmsDictDup","cmsDictFree","cmsDictFree","cmsGetAlarmCodesTHR","cmsBuildGamma","cmsFreeToneCurve","cmsCloseIOhandler","cmsDeleteContext","cmsStageAllocToneCurves","cmsFreeToneCurve","cmsCloseIOhandler","cmsDeleteContext","cmsPipelineAlloc","cmsStageFree","cmsCloseIOhandler","cmsDeleteContext","cmsPipelineInsertStage","cmsCreateTransformTHR","cmsPipelineFree","cmsCloseIOhandler","cmsDeleteContext","cmsDoTransformStride","cmsDeleteTransform","cmsPipelineFree","cmsCloseIOhandler","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenIOhandlerFromMem","cmsDictAlloc","cmsDictDup","cmsDictFree","cmsDictFree","cmsGetAlarmCodesTHR","cmsBuildGamma","cmsStageAllocToneCurves","cmsFreeToneCurve","cmsPipelineAlloc","cmsPipelineInsertStage","cmsCreateTransformTHR","cmsDoTransformStride","cmsDeleteTransform","cmsPipelineFree","cmsCloseIOhandler","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Minimal size check

    // Create a context for lcms operations
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 1. Use cmsOpenIOhandlerFromMem to create an I/O handler from input data
    cmsIOHANDLER* io_handler = cmsOpenIOhandlerFromMem(context, (void*)data, size, "r");
    if (!io_handler) {
        cmsDeleteContext(context);
        return 0;
    }

    // 2. Use cmsDictDup to duplicate a dictionary (create a dummy dict first)
    cmsHANDLE dict = cmsDictAlloc(context);
    cmsHANDLE dup_dict = cmsDictDup(dict);
    if (dict) cmsDictFree(dict);
    if (dup_dict) cmsDictFree(dup_dict);

    // 3. Use cmsGetAlarmCodesTHR to get alarm codes
    cmsUInt16Number alarm_codes[16];
    cmsGetAlarmCodesTHR(context, alarm_codes);

    // 4. Create tone curves and use cmsStageAllocToneCurves
    cmsToneCurve* curves[3];
    for (int i = 0; i < 3; ++i) {
        // Create a simple gamma curve
        cmsFloat64Number gamma_val = 2.2;
        curves[i] = cmsBuildGamma(context, gamma_val);
        if (!curves[i]) {
            for (int j = 0; j < i; ++j) cmsFreeToneCurve(curves[j]);
            cmsCloseIOhandler(io_handler);
            cmsDeleteContext(context);
            return 0;
        }
    }
    
    cmsStage* tone_curve_stage = cmsStageAllocToneCurves(context, 3, curves);
    for (int i = 0; i < 3; ++i) {
        cmsFreeToneCurve(curves[i]);
    }
    if (!tone_curve_stage) {
        cmsCloseIOhandler(io_handler);
        cmsDeleteContext(context);
        return 0;
    }

    // 5. Create a simple transform pipeline using the tone curve stage
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    if (!pipeline) {
        cmsStageFree(tone_curve_stage);
        cmsCloseIOhandler(io_handler);
        cmsDeleteContext(context);
        return 0;
    }

    cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, tone_curve_stage);
    
    // Create a transform from the pipeline
    cmsHTRANSFORM transform = cmsCreateTransformTHR(context, 
        nullptr, TYPE_RGB_8, 
        nullptr, TYPE_RGB_8, 
        INTENT_PERCEPTUAL, 0);
    if (!transform) {
        cmsPipelineFree(pipeline);
        cmsCloseIOhandler(io_handler);
        cmsDeleteContext(context);
        return 0;
    }

    // 6. Use cmsDoTransformStride with the transform
    // Prepare input/output buffers
    const size_t num_pixels = 10;
    const size_t stride_in = 3;  // RGB
    const size_t stride_out = 3; // RGB
    const size_t buffer_size = num_pixels * 3;
    
    // Use part of input data as pixel data
    uint8_t input_buffer[buffer_size];
    uint8_t output_buffer[buffer_size];
    
    // Copy data from input, ensuring we don't overflow
    size_t copy_size = (size > buffer_size) ? buffer_size : size;
    memcpy(input_buffer, data, copy_size);
    
    // Fill any remaining space with default values
    for (size_t i = copy_size; i < buffer_size; ++i) {
        input_buffer[i] = 128;  // Mid-gray
    }

    // Perform the transform with stride
    cmsDoTransformStride(transform, input_buffer, output_buffer, 
                        num_pixels, stride_in);

    // Cleanup
    cmsDeleteTransform(transform);
    cmsPipelineFree(pipeline);
    cmsCloseIOhandler(io_handler);
    cmsDeleteContext(context);

    return 0;
}