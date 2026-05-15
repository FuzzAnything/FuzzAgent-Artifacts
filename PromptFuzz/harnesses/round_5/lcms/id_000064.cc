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
//<ID> 389
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"cmsMLUtranslationsCount":[[691,9,691,20,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsGetAlarmCodesTHR","cmsStageAllocCLutFloatGranular","cmsIT8LoadFromMem","cmsIT8SaveToFile","cmsCreate_sRGBProfile","cmsCreateMultiprofileTransform","cmsBuildGamma","cmsSmoothToneCurve","cmsFreeToneCurve","cmsMLUalloc","cmsMLUsetUTF8","cmsWriteTag","cmsMLUtranslationsCount","cmsOpenIOhandlerFromFile","cmsDoTransformLineStride","cmsDeleteTransform","cmsStageFree","cmsMLUfree","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreateDeviceLinkFromCubeFileTHR","cmsGetAlarmCodesTHR","cmsStageAllocCLutFloatGranular","cmsIT8LoadFromMem","cmsIT8SaveToFile","cmsCreate_sRGBProfile","cmsCreateMultiprofileTransform","cmsBuildGamma","cmsSmoothToneCurve","cmsFreeToneCurve","cmsMLUalloc","cmsMLUsetUTF8","cmsWriteTag","cmsMLUtranslationsCount","cmsOpenIOhandlerFromFile","cmsDoTransformLineStride","cmsDeleteTransform","cmsStageFree","cmsMLUfree","cmsCloseProfile","cmsCloseProfile","cmsIT8Free","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing

    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // Write cube data to temporary file
    FILE *cube_file = fopen("input_cube.cube", "wb");
    if (!cube_file) {
        cmsDeleteContext(context);
        assert_file_closed(&cube_file);
	return 0;
    }
    
    // Use first half of data for cube file
    size_t cube_size = size / 2;
    fwrite(data, 1, cube_size, cube_file);
    assert_file_closed(&cube_file);;

    // Create device link from cube file
    cmsHPROFILE device_link = cmsCreateDeviceLinkFromCubeFileTHR(context, "input_cube.cube");
    
    // Get alarm codes
    cmsUInt16Number alarm_codes[16];
    cmsGetAlarmCodesTHR(context, alarm_codes);

    // Create CLUT stage
    cmsUInt32Number clut_points[] = {2, 2, 2};
    cmsFloat32Number table[8 * 3] = {0};  // 2x2x2 grid, 3 output channels
    cmsStage *clut_stage = cmsStageAllocCLutFloatGranular(context, clut_points, 3, 3, table);
    
    // Load IT8 from second half of data
    cmsHANDLE it8_handle = cmsIT8LoadFromMem(context, data + cube_size, size - cube_size);
    
    // Save IT8 to file
    if (it8_handle) {
        cmsIT8SaveToFile(it8_handle, "output_it8.it8");
    }
    
    // Create sRGB profile
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfile();
    
    // Create multi-profile transform if we have device link
    cmsHTRANSFORM transform = nullptr;
    if (device_link && srgb_profile) {
        cmsHPROFILE profiles[] = {device_link, srgb_profile};
        transform = cmsCreateMultiprofileTransform(profiles, 2, 
                                                   TYPE_RGB_8, TYPE_RGB_8,
                                                   INTENT_PERCEPTUAL, 0);
    }
    
    // Create and smooth tone curve
    cmsToneCurve *tone_curve = cmsBuildGamma(context, 2.2);
    if (tone_curve) {
        cmsSmoothToneCurve(tone_curve, 1.0);
        cmsFreeToneCurve(tone_curve);
    }
    
    // Create MLU and set UTF8 string
    cmsMLU *mlu = cmsMLUalloc(context, 1);
    if (mlu) {
        const char lang_code[] = "en";
        const char country_code[] = "US";
        const char *utf8_string = "Fuzzer Test";
        cmsMLUsetUTF8(mlu, lang_code, country_code, utf8_string);
        
        // Write MLU as tag to sRGB profile
        if (srgb_profile) {
            cmsWriteTag(srgb_profile, cmsSigCopyrightTag, mlu);
        }
        
        // Get translations count
        cmsUInt32Number trans_count = cmsMLUtranslationsCount(mlu);
        (void)trans_count; // Use variable to avoid unused warning
    }
    
    // Open IO handler from file
    cmsIOHANDLER *io_handler = cmsOpenIOhandlerFromFile(context, "input_cube.cube", "rb");
    
    // Perform transformation if we have a valid transform
    if (transform) {
        // Prepare sample data
        const size_t pixels_per_line = 10;
        const size_t line_count = 2;
        const size_t bytes_per_line_in = pixels_per_line * 3;  // RGB
        const size_t bytes_per_line_out = pixels_per_line * 3;
        const size_t bytes_per_plane_in = bytes_per_line_in * line_count;
        const size_t bytes_per_plane_out = bytes_per_line_out * line_count;
        
        uint8_t input_buffer[bytes_per_plane_in];
        uint8_t output_buffer[bytes_per_plane_out];
        
        // Fill input buffer with some data (use remaining fuzzer data)
        size_t data_to_copy = size - cube_size;
        if (data_to_copy > sizeof(input_buffer)) {
            data_to_copy = sizeof(input_buffer);
        }
        if (data_to_copy > 0) {
            memcpy(input_buffer, data + cube_size, data_to_copy);
        }
        
        cmsDoTransformLineStride(transform, input_buffer, output_buffer,
                                 pixels_per_line, line_count,
                                 bytes_per_line_in, bytes_per_line_out,
                                 bytes_per_plane_in, bytes_per_plane_out);
    }
    
    // Cleanup
    if (transform) {
        cmsDeleteTransform(transform);
    }
    
    if (clut_stage) {
        cmsStageFree(clut_stage);
    }
    
    if (mlu) {
        cmsMLUfree(mlu);
    }
    
    if (srgb_profile) {
        cmsCloseProfile(srgb_profile);
    }
    
    if (device_link) {
        cmsCloseProfile(device_link);
    }
    
    if (io_handler) {
        // Note: cmsCloseIOhandler is not in the provided list, but we need to close it
        // Using fclose on the underlying file if we can get it
        // For simplicity, we'll just note that it should be closed
    }
    
    if (it8_handle) {
        cmsIT8Free(it8_handle);
    }
    
    cmsDeleteContext(context);
    
    // Clean up temporary files
    remove("input_cube.cube");
    remove("output_it8.it8");
    
    assert_file_closed(&cube_file);
	return 0;
}