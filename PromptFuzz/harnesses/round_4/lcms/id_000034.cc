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
//<ID> 240
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["cmsCreateContext","cmsD50_xyY","cmsDeleteContext","cmsCreateLab4ProfileTHR","cmsDeleteContext","cmsOpenProfileFromMemTHR","cmsCloseProfile","cmsDeleteContext","cmsGetHeaderManufacturer","cmsGetEncodedICCversion","cmsGBDAlloc","cmsGDBAddPoint","cmsCreateTransformTHR","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsStageData","cmsPipelineUnlinkStage","cmsStageFree","cmsBuildParametricToneCurve","cmsIsToneCurveMonotonic","cmsDoTransformLineStride","cmsFreeToneCurve","cmsPipelineFree","cmsDeleteTransform","cmsGBDFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsD50_xyY","cmsCreateLab4ProfileTHR","cmsOpenProfileFromMemTHR","cmsGetHeaderManufacturer","cmsGetEncodedICCversion","cmsGBDAlloc","cmsGDBAddPoint","cmsCreateTransformTHR","cmsPipelineAlloc","cmsStageAllocIdentity","cmsPipelineInsertStage","cmsStageData","cmsPipelineUnlinkStage","cmsStageFree","cmsBuildParametricToneCurve","cmsIsToneCurveMonotonic","cmsDoTransformLineStride","cmsFreeToneCurve","cmsPipelineFree","cmsDeleteTransform","cmsGBDFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<85) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_6, fuzz_double_array_size_6, fdp);
		FDPConsumeIntegralArray(uint8_t, fuzz_uint8_t_array_7, fuzz_uint8_t_array_size_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
	
		const cmsFloat64Number * fuzzer_var_cmsBuildParametricToneCurve_19_2 = fuzz_double_array_6;
		const void * fuzzer_var_cmsDoTransformLineStride_21_1 = fuzz_uint8_t_array_7;
		FDPGetRandomInteger(cmsUInt32Number, fuzzer_index_cmsDoTransformLineStride_21_1, fuzz_uint8_t_array_size_7)//fuzzer shim end}




    // Return early if there's not enough data for basic operations
    if (size < 100) {
	return 0;
	}
	

    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 2. Get D50 white point
    const cmsCIExyY* d50_white = cmsD50_xyY();
    if (!d50_white) {
        cmsDeleteContext(context);
        return 0;
    }

    // 3. Create Lab4 profile using D50
    cmsHPROFILE lab_profile = cmsCreateLab4ProfileTHR(context, d50_white);
    if (!lab_profile) {
        cmsDeleteContext(context);
        return 0;
    }

    // 4. Open profile from memory (treat input data as ICC profile)
    cmsHPROFILE input_profile = cmsOpenProfileFromMemTHR(context, data, static_cast<cmsUInt32Number>(size));
    if (!input_profile) {
        cmsCloseProfile(lab_profile);
        cmsDeleteContext(context);
        return 0;
    }

    // 5. Get header information from input profile
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(input_profile);
    cmsUInt32Number version = cmsGetEncodedICCversion(input_profile);

    // 6. Create GBD (Gamut Boundary Descriptor)
    cmsHANDLE gbd = cmsGBDAlloc(context);
    if (gbd) {
        // Add a sample Lab point to GBD
        cmsCIELab lab_point = {50.0, 0.0, 0.0};
        cmsGDBAddPoint(gbd, &lab_point);
    }

    // 7. Create transform from input to Lab profile
    // Use RGB_8 as input format (common for many profiles)
    cmsUInt32Number input_format = TYPE_RGB_8;
    cmsUInt32Number output_format = TYPE_Lab_16;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = cmsFLAGS_NOCACHE;
    
    cmsHTRANSFORM transform = cmsCreateTransformTHR(
        context,
        input_profile,
        input_format,
        lab_profile,
        output_format,
        fuzz_uint32_t_1,
        fuzz_uint32_t_2
    );

    // 8. Create and manipulate pipeline
    cmsPipeline* pipeline = nullptr;
    cmsStage* stage = nullptr;
    cmsToneCurve* tone_curve = nullptr;
    
    if (transform) {
        // Create a pipeline for demonstration
        pipeline = cmsPipelineAlloc(context, fuzz_uint32_t_3, fuzz_uint32_t_4);
        if (pipeline) {
            // Create identity stage
            stage = cmsStageAllocIdentity(context, fuzz_uint32_t_5);
            if (stage) {
                // Insert stage at beginning
                cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage);
                
                // Get stage data (should be identity matrix for identity stage)
                void* stage_data = cmsStageData(stage);
                
                // Unlink the stage
                cmsStage* unlinked_stage = nullptr;
                cmsPipelineUnlinkStage(pipeline, cmsAT_BEGIN, &unlinked_stage);
                
                // Free the unlinked stage
                if (unlinked_stage) {
                    cmsStageFree(unlinked_stage);
                }
            }
            
            // Create a parametric tone curve (gamma 2.2)
            cmsFloat64Number params[5] = {2.2, 0.0, 0.0, 0.0, 0.0};
            tone_curve = cmsBuildParametricToneCurve(context, 1, fuzzer_var_cmsBuildParametricToneCurve_19_2);
            if (tone_curve) {
                // Check if tone curve is monotonic
                cmsBool is_monotonic = cmsIsToneCurveMonotonic(tone_curve);
                (void)is_monotonic; // Use result to avoid unused variable warning
            }
        }
    }

    // 9. Perform color transformation if we have a valid transform
    if (transform) {
        // Prepare input and output buffers
        const size_t pixels_per_line = 1;
        const size_t line_count = 1;
        
        // RGB input (3 bytes per pixel)
        uint8_t input_buffer[3] = {128, 64, 192};
        
        // Lab output (3 channels * 2 bytes per channel = 6 bytes)
        uint8_t output_buffer[6];
        
        // Perform transformation with line stride
        cmsDoTransformLineStride(
            transform,
            fuzzer_var_cmsDoTransformLineStride_21_1,
            output_buffer,
            fuzzer_index_cmsDoTransformLineStride_21_1,
            static_cast<cmsUInt32Number>(line_count),
            fuzz_uint32_t_8,  // Bytes per line in (3 bytes for RGB)
            fuzz_uint32_t_9,  // Bytes per line out (6 bytes for Lab16)
            fuzz_uint32_t_10,  // Bytes per plane in (RGB is planar by default)
            fuzz_uint32_t_11   // Bytes per plane out (Lab16 has 2 bytes per channel)
        );
    }

    // 10. Cleanup in reverse order of creation
    if (tone_curve) {
        cmsFreeToneCurve(tone_curve);
    }
    
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (gbd) {
        cmsGBDFree(gbd);
    }
    
    cmsCloseProfile(input_profile);
    cmsCloseProfile(lab_profile);
    cmsDeleteContext(context);

    return 0;
}