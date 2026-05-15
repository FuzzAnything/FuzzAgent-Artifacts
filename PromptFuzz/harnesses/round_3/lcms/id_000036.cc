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
//<ID> 246
//<Prompt> []
/*<Combination>: [
*/
//<score> 87, nr_unique_branch: 6
//<Quality> {"density":29,"unique_branches":{"cmsChangeBuffersFormat":[[1470,9,1470,67,0,0,4,0]],"cmsStageSampleCLutFloat":[[823,9,823,20,0,0,4,1],[827,9,827,21,0,0,4,1],[833,9,833,21,0,0,4,1],[834,9,834,22,0,0,4,1],[835,9,835,40,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsDeleteContext","cmsIT8FindDataFormat","cmsBuildSegmentedToneCurve","cmsIT8Free","cmsDeleteContext","cmsCreateLinearizationDeviceLink","cmsCreateInkLimitingDeviceLinkTHR","cmsPipelineAlloc","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext","cmsStageAllocToneCurves","cmsPipelineInputChannels","cmsStageSampleCLutFloat","cmsCreate_sRGBProfileTHR","cmsCreateLab4ProfileTHR","cmsCreateTransformTHR","cmsChangeBuffersFormat","cmsDeleteTransform","cmsStageFree","cmsPipelineFree","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsIT8LoadFromMem","cmsIT8FindDataFormat","cmsBuildSegmentedToneCurve","cmsCreateLinearizationDeviceLink","cmsCreateInkLimitingDeviceLinkTHR","cmsPipelineAlloc","cmsStageAllocToneCurves","cmsPipelineInputChannels","cmsStageSampleCLutFloat","cmsCreate_sRGBProfileTHR","cmsCreateLab4ProfileTHR","cmsCreateTransformTHR","cmsChangeBuffersFormat","cmsDeleteTransform","cmsStageFree","cmsPipelineFree","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // 1. Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // 2. Load IT8 data from memory
    cmsHANDLE it8 = cmsIT8LoadFromMem(ctx, data, (cmsUInt32Number)size);
    if (!it8) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // 3. Find data format in IT8
    const char* sample_name = "Sample";
    int format = cmsIT8FindDataFormat(it8, sample_name);

    // 4. Build segmented tone curve
    cmsCurveSegment segments[2] = {0};
    segments[0].Type = 1;  // Linear segment
    segments[0].Params[0] = 1.0;  // Gamma
    segments[0].x0 = 0.0;
    segments[0].x1 = 0.5;
    segments[0].nGridPoints = 256;
    
    segments[1].Type = 1;  // Linear segment
    segments[1].Params[0] = 2.2;  // Gamma
    segments[1].x0 = 0.5;
    segments[1].x1 = 1.0;
    segments[1].nGridPoints = 256;

    cmsToneCurve* tone_curve = cmsBuildSegmentedToneCurve(ctx, 2, segments);
    if (!tone_curve) {
        cmsIT8Free(it8);
        cmsDeleteContext(ctx);
        return 0;
    }

    // 5. Create tone curves array for device link
    cmsToneCurve* curves[3] = {tone_curve, tone_curve, tone_curve};
    cmsHPROFILE linear_link = cmsCreateLinearizationDeviceLink(cmsSigRgbData, curves);
    
    // 6. Create ink limiting device link
    cmsHPROFILE ink_link = cmsCreateInkLimitingDeviceLinkTHR(ctx, cmsSigCmykData, 0.9);
    
    // 7. Create pipeline
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (!pipeline) {
        cmsFreeToneCurve(tone_curve);
        cmsCloseProfile(linear_link);
        if (ink_link) cmsCloseProfile(ink_link);
        cmsIT8Free(it8);
        cmsDeleteContext(ctx);
        return 0;
    }

    // 8. Create and add tone curve stage to pipeline
    cmsStage* curve_stage = cmsStageAllocToneCurves(ctx, 3, curves);
    if (curve_stage) {
        // Note: cmsPipelineInsertStage is needed to actually add the stage
        // For fuzzing, we'll just create the stage
    }

    // 9. Prepare for CLUT sampling
    cmsUInt32Number clut_points[] = {17, 17, 17};
    
    // Define a simple sampler function
    auto sampler = [](const cmsFloat32Number in[], cmsFloat32Number out[], void* cargo) -> int {
        // Simple identity transform for fuzzing
        for (int i = 0; i < 3; i++) {
            out[i] = in[i];
        }
        return 1;
    };

    // 10. Get pipeline input channels
    cmsUInt32Number input_channels = cmsPipelineInputChannels(pipeline);

    // 11. Sample CLUT if we had a proper stage
    if (curve_stage) {
        // 14. Sample CLUT (using dummy stage)
        cmsStageSampleCLutFloat(curve_stage, sampler, nullptr, 0);
    }

    // 12. Try to create a simple transform to use cmsChangeBuffersFormat
    cmsHPROFILE src_profile = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE dst_profile = cmsCreateLab4ProfileTHR(ctx, nullptr);
    
    if (src_profile && dst_profile) {
        cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, src_profile, 
                                                       TYPE_RGB_8, 
                                                       dst_profile, 
                                                       TYPE_Lab_8, 
                                                       INTENT_PERCEPTUAL, 
                                                       0);
        if (transform) {
            // Try to change buffer format
            cmsChangeBuffersFormat(transform, TYPE_RGB_16, TYPE_Lab_16);
            cmsDeleteTransform(transform);
        }
    }

    // Cleanup
    if (curve_stage) cmsStageFree(curve_stage);
    cmsPipelineFree(pipeline);
    cmsFreeToneCurve(tone_curve);
    
    if (linear_link) cmsCloseProfile(linear_link);
    if (ink_link) cmsCloseProfile(ink_link);
    if (src_profile) cmsCloseProfile(src_profile);
    if (dst_profile) cmsCloseProfile(dst_profile);
    
    cmsIT8Free(it8);
    cmsDeleteContext(ctx);

    return 0;
}