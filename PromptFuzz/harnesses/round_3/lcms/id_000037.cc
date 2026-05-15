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
//<ID> 247
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 7
//<Quality> {"density":14,"unique_branches":{"DefaultEvalParametricFn":[[601,13,601,29,0,0,4,1],[605,17,605,22,0,0,4,1]],"cmsDetectRGBProfileGamma":[[622,35,622,59,0,0,4,0],[623,9,623,32,0,0,4,0],[623,36,623,63,0,0,4,0]],"cmsCreateInkLimitingDeviceLinkTHR":[[407,9,407,20,0,0,4,1],[407,24,407,35,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8FindDataFormat","cmsBuildSegmentedToneCurve","cmsCreateLinearizationDeviceLink","cmsCreateInkLimitingDeviceLinkTHR","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsStageSampleCLutFloat","cmsPipelineInputChannels","cmsCreateTransformTHR","cmsChangeBuffersFormat","cmsDeleteTransform","cmsStageFree","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8FindDataFormat","cmsBuildSegmentedToneCurve","cmsCreateLinearizationDeviceLink","cmsCreateInkLimitingDeviceLinkTHR","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsStageSampleCLutFloat","cmsPipelineInputChannels","cmsCreateTransformTHR","cmsChangeBuffersFormat","cmsDeleteTransform","cmsStageFree","cmsPipelineFree","cmsCloseProfile","cmsCloseProfile","cmsFreeToneCurve","cmsIT8Free","cmsDeleteContext"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified LCMS APIs in a coherent workflow:

*/


// Custom sampler function for cmsSliceSpaceFloat and cmsStageSampleCLutFloat
static int  id_000037_custom_sampler_float(const cmsFloat32Number in[], cmsFloat32Number out[], void* cargo) {
    // Simple pass-through sampler
    for (int i = 0; i < 3; i++) {  // Assuming 3 channels
        out[i] = in[i];
    }
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data

    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 2. Load IT8 from memory (consume input data)
    cmsHANDLE it8 = cmsIT8LoadFromMem(context, data, size);
    
    // 3. Find data format in IT8
    const char* sample_name = "Sample1";
    int data_format = -1;
    if (it8) {
        data_format = cmsIT8FindDataFormat(it8, sample_name);
    }

    // 4. Build segmented tone curve
    cmsToneCurve* tone_curve = nullptr;
    if (size >= 200) {  // Ensure enough data for segments
        cmsCurveSegment segments[1];
        segments[0].x0 = 0.0f;
        segments[0].x1 = 1.0f;
        segments[0].Type = 6;  // Simple gamma
        
        // Use data to seed gamma parameter
        segments[0].Params[0] = 2.2 + (data[0] / 255.0 * 0.8);  // Gamma between 2.2-3.0
        
        tone_curve = cmsBuildSegmentedToneCurve(context, 1, segments);
    }

    // 5. Create linearization device link
    cmsHPROFILE linearization_profile = nullptr;
    if (tone_curve) {
        cmsToneCurve* curves[3] = {tone_curve, tone_curve, tone_curve};
        linearization_profile = cmsCreateLinearizationDeviceLink(cmsSigRgbData, curves);
    }

    // 6. Create ink limiting device link
    cmsHPROFILE ink_limit_profile = nullptr;
    if (linearization_profile && size > 10) {
        // Use data to determine limit (200-400)
        cmsFloat64Number limit = 200.0 + (data[1] / 255.0 * 200.0);
        ink_limit_profile = cmsCreateInkLimitingDeviceLinkTHR(
            context, cmsSigCmykData, limit);
    }

    // 7. Create pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(context, 3, 3);
    
    // 8. Allocate tone curve stage
    cmsStage* tone_stage = nullptr;
    if (pipeline && tone_curve) {
        cmsToneCurve* curves[3] = {tone_curve, tone_curve, tone_curve};
        tone_stage = cmsStageAllocToneCurves(context, 3, curves);
        
        if (tone_stage) {
            // 9. Sample CLUT in stage
            cmsStageSampleCLutFloat(tone_stage,  id_000037_custom_sampler_float, nullptr, 0);
        }
    }

    // 10. Get pipeline input channels
    cmsUInt32Number input_channels = 0;
    if (pipeline) {
        input_channels = cmsPipelineInputChannels(pipeline);
    }

    // 11. Create a transform to use with cmsChangeBuffersFormat
    cmsHTRANSFORM transform = nullptr;
    if (linearization_profile && ink_limit_profile) {
        // Create transform using general API (from provided list)
        transform = cmsCreateTransformTHR(
            context,
            linearization_profile,
            TYPE_RGB_16,
            ink_limit_profile,
            TYPE_CMYK_16,
            INTENT_PERCEPTUAL,
            cmsFLAGS_NOCACHE);
        
        if (transform) {
            // 12. Change buffers format
            cmsChangeBuffersFormat(transform, TYPE_RGB_8, TYPE_CMYK_8);
        }
    }

    // 13. Create FILE* from input data for additional testing
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        // Use file descriptor if needed
        int fd = fuzz_fileno(in_file);
        (void)fd;  // Avoid unused variable warning
        
        // Read some data from file
        uint8_t buffer[10];
        fread(buffer, 1, sizeof(buffer), in_file);
        assert_file_closed(&in_file);;
    }

    // 14. Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        (void)out_fd;  // Avoid unused variable warning
        
        // Write some dummy data
        const char* dummy = "test";
        fwrite(dummy, 1, strlen(dummy), out_file);
        assert_file_closed(&out_file);;
    }

    // 15. Cleanup - release all resources
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (tone_stage) {
        cmsStageFree(tone_stage);
    }
    
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    if (ink_limit_profile) {
        cmsCloseProfile(ink_limit_profile);
    }
    
    if (linearization_profile) {
        cmsCloseProfile(linearization_profile);
    }
    
    if (tone_curve) {
        cmsFreeToneCurve(tone_curve);
    }
    
    if (it8) {
        cmsIT8Free(it8);
    }
    
    cmsDeleteContext(context);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}