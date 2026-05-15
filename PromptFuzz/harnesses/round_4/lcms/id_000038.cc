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
//<ID> 259
//<Prompt> []
/*<Combination>: [
*/
//<score> 76, nr_unique_branch: 3
//<Quality> {"density":19,"unique_branches":{"FixWhiteMisalignment":[[575,9,576,38,0,0,4,0]],"_cmsEndPointsBySpace":[[752,8,752,15,0,0,4,0]],"cmsGetTransformGamutCheckPipeline":[[1456,9,1456,22,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsAllocProfileSequenceDescription","cmsOpenProfileFromMemTHR","cmsCreateXYZProfile","cmsCreateLab2Profile","cmsCreate_sRGBProfileTHR","cmsSetHeaderModel","cmsSetHeaderRenderingIntent","cmsCreateProofingTransformTHR","cmsGetTransformGamutCheckPipeline","cmsGetPipelineContextID","cmsGetDeviceClass","cmsIT8LoadFromMem","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetDataRowColDbl","cmsSetAlarmCodesTHR","cmsStageAllocCLut16bit","cmsStageFree","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsFreeProfileSequenceDescription","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsAllocProfileSequenceDescription","cmsOpenProfileFromMemTHR","cmsCreateXYZProfile","cmsCreateLab2Profile","cmsCreate_sRGBProfileTHR","cmsSetHeaderModel","cmsSetHeaderRenderingIntent","cmsCreateProofingTransformTHR","cmsGetTransformGamutCheckPipeline","cmsGetPipelineContextID","cmsGetDeviceClass","cmsIT8LoadFromMem","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetDataRowColDbl","cmsSetAlarmCodesTHR","cmsStageAllocCLut16bit","cmsStageFree","cmsIT8Free","cmsDeleteTransform","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsFreeProfileSequenceDescription","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<83) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_11, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_12, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_13, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_14, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_15, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_16, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_17, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_18, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_19, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_20, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_21, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_22, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_23, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_24, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_25, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_26, fdp);
	
		cmsUInt16Number fuzzer_var_cmsSetAlarmCodesTHR_16_1[16] = {fuzz_uint16_t_11,fuzz_uint16_t_12,fuzz_uint16_t_13,fuzz_uint16_t_14,fuzz_uint16_t_15,fuzz_uint16_t_16,fuzz_uint16_t_17,fuzz_uint16_t_18,fuzz_uint16_t_19,fuzz_uint16_t_20,fuzz_uint16_t_21,fuzz_uint16_t_22,fuzz_uint16_t_23,fuzz_uint16_t_24,fuzz_uint16_t_25,fuzz_uint16_t_26};//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // Allocate profile sequence description
    cmsSEQ* seq = cmsAllocProfileSequenceDescription(context, fuzz_uint32_t_1);
    
    // Open profile from memory (using first half of data)
    size_t profile_size = size / 2;
    cmsHPROFILE input_profile = cmsOpenProfileFromMemTHR(context, data, profile_size);
    
    // Create XYZ profile
    cmsHPROFILE xyz_profile = cmsCreateXYZProfile();
    
    // Create Lab2 profile with default white point
    cmsCIExyY white_point = {0.3457, 0.3585, 1.0}; // D65
    cmsHPROFILE lab_profile = cmsCreateLab2Profile(&white_point);
    
    // Create sRGB profile
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfileTHR(context);
    
    // Set header information if profiles were created
    if (input_profile) {
        cmsSetHeaderModel(input_profile, fuzz_uint32_t_2); // Set model to 0
        cmsSetHeaderRenderingIntent(input_profile, fuzz_uint32_t_3); // Perceptual intent
    }
    
    // Create a proofing transform if we have all required profiles
    cmsHTRANSFORM transform = nullptr;
    if (input_profile && xyz_profile && lab_profile) {
        transform = cmsCreateProofingTransformTHR(
            context,
            input_profile,
            TYPE_RGB_8,
            xyz_profile,
            TYPE_XYZ_16,
            lab_profile,
            fuzz_uint32_t_4,
            fuzz_uint32_t_5,
            fuzz_uint32_t_6
        );
    }
    
    // Get gamut check pipeline if transform exists
    cmsPipeline* pipeline = nullptr;
    if (transform) {
        pipeline = cmsGetTransformGamutCheckPipeline(transform);
    }
    
    // Get pipeline context ID if pipeline exists
    cmsContext pipeline_context = nullptr;
    if (pipeline) {
        pipeline_context = cmsGetPipelineContextID(pipeline);
    }
    
    // Get device class if input profile exists
    cmsProfileClassSignature device_class = cmsSigInputClass;
    if (input_profile) {
        device_class = cmsGetDeviceClass(input_profile);
    }
    
    // Load IT8 data from second half of memory
    size_t it8_size = size - profile_size;
    const uint8_t* it8_data = data + profile_size;
    cmsHANDLE it8_handle = cmsIT8LoadFromMem(context, it8_data, it8_size);
    
    // If IT8 loading failed, allocate a new IT8 handle
    if (!it8_handle) {
        it8_handle = cmsIT8Alloc(context);
    }
    
    // Set IT8 properties if handle exists
    if (it8_handle) {
        cmsIT8SetPropertyStr(it8_handle, fuzz_str_7, fuzz_str_8);
        cmsIT8SetDataRowColDbl(it8_handle, fuzz_int32_t_9, fuzz_int32_t_10, 1.0);
    }
    
    // Set alarm codes
    cmsUInt16Number alarm_codes[16] = {0};
    cmsSetAlarmCodesTHR(context, fuzzer_var_cmsSetAlarmCodesTHR_16_1);
    
    // Create CLUT stage if we have enough data for a small table
    cmsStage* clut_stage = nullptr;
    if (size >= 200) {
        // Create a simple 2x2x2 CLUT (8 entries) for RGB to RGB
        cmsUInt16Number table[8 * 3] = {0};
        size_t table_size = sizeof(table);
        if (size - 100 >= table_size) {
            // Use some data for the table
            memcpy(table, data + 100, table_size);
            clut_stage = cmsStageAllocCLut16bit(
                context,
                2, // 2 grid points per dimension
                3, // 3 input channels (RGB)
                3, // 3 output channels (RGB)
                table
            );
        }
    }
    
    // Clean up resources in reverse order
    if (clut_stage) {
        cmsStageFree(clut_stage);
    }
    
    if (it8_handle) {
        cmsIT8Free(it8_handle);
    }
    
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (srgb_profile) {
        cmsCloseProfile(srgb_profile);
    }
    
    if (lab_profile) {
        cmsCloseProfile(lab_profile);
    }
    
    if (xyz_profile) {
        cmsCloseProfile(xyz_profile);
    }
    
    if (input_profile) {
        cmsCloseProfile(input_profile);
    }
    
    if (seq) {
        cmsFreeProfileSequenceDescription(seq);
    }
    
    cmsDeleteContext(context);
    
    return 0;
}