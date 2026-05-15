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
//<ID> 226
//<Prompt> []
/*<Combination>: [
*/
//<score> 92, nr_unique_branch: 3
//<Quality> {"density":23,"unique_branches":{"cmsMLUsetUTF8":[[389,9,389,21,0,0,4,0]],"_cmsCallocDefaultFn":[[192,9,192,19,0,0,4,0]],"cmsCreateDeviceLinkFromCubeFileTHR":[[3286,9,3286,75,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateLab4Profile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext","cmsGetTagCount","cmsDictAlloc","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageInputChannels","cmsCreateProofingTransform","cmsDoTransform","cmsDeleteTransform","cmsDoTransform","cmsDeleteTransform","cmsGetHeaderCMM","cmsDictFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsCreateLab4Profile","cmsGetTagCount","cmsDictAlloc","cmsCreateTransformTHR","cmsGetTransformPipeline","cmsPipelineGetPtrToFirstStage","cmsStageInputChannels","cmsCreateProofingTransform","cmsDoTransform","cmsDeleteTransform","cmsDoTransform","cmsDeleteTransform","cmsGetHeaderCMM","cmsDictFree","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<51) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegralArray(uint16_t, fuzz_uint16_t_array_7, fuzz_uint16_t_array_size_7, fdp);
		FDPConsumeIntegralArray(uint16_t, fuzz_uint16_t_array_8, fuzz_uint16_t_array_size_8, fdp);
	
		const void * fuzzer_var_cmsDoTransform_14_1 = fuzz_uint16_t_array_7;
		const void * fuzzer_var_cmsDoTransform_16_1 = fuzz_uint16_t_array_8;//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 4) {
	return 0;
	}
	

    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	

    // Write input data to a temporary file for cube file reading
    FILE* cube_file = fopen("input_file", "wb");
    if (!cube_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&cube_file);
	return 0;
    }
    fwrite(data, 1, size, cube_file);
    assert_file_closed(&cube_file);;

    // Create device link profile from cube file
    cmsHPROFILE device_link = cmsCreateDeviceLinkFromCubeFileTHR(ctx, fuzz_str_1);
    
    // Create Lab4 profile with D50 white point
    cmsCIExyY d50_white = {0.3457, 0.3585, 1.0};
    cmsHPROFILE lab4_profile = cmsCreateLab4Profile(&d50_white);
    
    if (!device_link || !lab4_profile) {
        if (device_link) cmsCloseProfile(device_link);
        if (lab4_profile) cmsCloseProfile(lab4_profile);
        remove("input_file");
        cmsDeleteContext(ctx);
        assert_file_closed(&cube_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Get tag count from device link profile
    cmsInt32Number tag_count = cmsGetTagCount(device_link);

    // Allocate dictionary
    cmsHANDLE dict = cmsDictAlloc(ctx);
    
    // Create transform from device link to Lab4 profile
    cmsUInt32Number input_format = TYPE_Lab_16;
    cmsUInt32Number output_format = TYPE_Lab_16;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = cmsFLAGS_NOCACHE;
    
    cmsHTRANSFORM transform = cmsCreateTransformTHR(
        ctx, 
        device_link, 
        input_format, 
        lab4_profile, 
        output_format, 
        fuzz_uint32_t_2, 
        fuzz_uint32_t_3
    );
    
    if (transform) {
        // Use CIECAM02 forward model (requires a model handle, which we don't have)
        // We'll skip this as we don't have a valid model handle
        // cmsCIECAM02Forward requires a valid model handle
        
        // Get transform pipeline
        cmsPipeline* pipeline = cmsGetTransformPipeline(transform);
        
        if (pipeline) {
            // Get first stage of pipeline
            cmsStage* first_stage = cmsPipelineGetPtrToFirstStage(pipeline);
            if (first_stage) {
                // Get input channels of first stage
                cmsUInt32Number stage_input_channels = cmsStageInputChannels(first_stage);
                (void)stage_input_channels; // Use variable to avoid unused warning
            }
        }
        
        // Create proofing transform
        cmsHTRANSFORM proofing_transform = cmsCreateProofingTransform(
            device_link,
            input_format,
            lab4_profile,
            output_format,
            lab4_profile,  // Using Lab4 as proofing profile
            fuzz_uint32_t_4,
            fuzz_uint32_t_5,
            fuzz_uint32_t_6
        );
        
        if (proofing_transform) {
            // Perform transformation on sample data
            cmsUInt16Number input_buffer[3] = {0x8000, 0x8000, 0x8000}; // Mid gray in Lab
            cmsUInt16Number output_buffer[3];
            cmsDoTransform(proofing_transform, fuzzer_var_cmsDoTransform_14_1, output_buffer, 1);
            
            // Delete proofing transform
            cmsDeleteTransform(proofing_transform);
        }
        
        // Perform transformation on sample data with main transform
        cmsUInt16Number input_buffer2[3] = {0x8000, 0x8000, 0x8000};
        cmsUInt16Number output_buffer2[3];
        cmsDoTransform(transform, fuzzer_var_cmsDoTransform_16_1, output_buffer2, 1);
        
        // Delete main transform
        cmsDeleteTransform(transform);
    }
    
    // Get CMM from header of device link profile
    cmsUInt32Number cmm = cmsGetHeaderCMM(device_link);
    (void)cmm; // Use variable to avoid unused warning
    
    // Free dictionary
    if (dict) {
        cmsDictFree(dict);
    }
    
    // Close profiles
    cmsCloseProfile(device_link);
    cmsCloseProfile(lab4_profile);
    
    // Clean up temporary file
    remove("input_file");
    
    // Delete context
    cmsDeleteContext(ctx);
    
    assert_file_closed(&cube_file);
	assert_file_name_closed("input_file");
	return 0;
}