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
//<ID> 621
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 10
//<Quality> {"density":18,"unique_branches":{"DefaultEvalParametricFn":[[601,13,601,29,0,0,4,1],[605,17,605,22,0,0,4,0],[605,17,605,22,0,0,4,1]],"SaveOneChromaticity":[[457,9,457,79,0,0,4,1],[458,9,458,79,0,0,4,1]],"Type_Chromaticity_Write":[[468,9,468,38,0,0,4,1],[469,9,469,38,0,0,4,1],[471,9,471,65,0,0,4,1],[472,9,472,67,0,0,4,1],[473,9,473,66,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsDeleteContext","cmsCreate_sRGBProfileTHR","cmsDeleteContext","cmsD50_xyY","cmsCreateLab4Profile","cmsCloseProfile","cmsDeleteContext","cmsBuildSegmentedToneCurve","cmsIT8LoadFromFile","cmsIT8LoadFromFile","cmsIT8SetPropertyUncooked","cmsSaveProfileToFile","cmsGetProfileIOhandler","cmsCreateMultiprofileTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsFreeToneCurve","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsCreate_sRGBProfileTHR","cmsD50_xyY","cmsCreateLab4Profile","cmsBuildSegmentedToneCurve","cmsIT8LoadFromFile","cmsIT8LoadFromFile","cmsIT8SetPropertyUncooked","cmsSaveProfileToFile","cmsGetProfileIOhandler","cmsCreateMultiprofileTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsFreeToneCurve","cmsIT8Free","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Looking at the provided APIs and the requirement to use the specified APIs together, I can create a fuzzer that:

1. Creates profiles and tone curves
2. Uses IT8 properties
3. Saves profiles to streams
4. Consumes the input data

*/


extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Create sRGB profile using specified API
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfileTHR(ctx);
    if (!srgb_profile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Create Lab4 profile using specified API
    // Use D50 white point
    const cmsCIExyY *white_point = cmsD50_xyY();
    cmsHPROFILE lab_profile = cmsCreateLab4Profile(white_point);
    if (!lab_profile) {
        cmsCloseProfile(srgb_profile);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create segmented tone curve using specified API
    // Use a single segment from the input data
    cmsCurveSegment segment;
    segment.x0 = 0.0f;
    segment.x1 = 1.0f;
    segment.Type = 6;  // Type 6: s-shaped curve
    
    // Use first 80 bytes of data for curve parameters
    size_t params_to_copy = (size < 80) ? size : 80;
    for (size_t i = 0; i < 10 && i * 8 < params_to_copy; i++) {
        // Copy double values from input data
        if (i * 8 + 8 <= params_to_copy) {
            memcpy(&segment.Params[i], data + i * 8, 8);
        } else {
            segment.Params[i] = 0.0;
        }
    }
    
    segment.nGridPoints = 0;
    segment.SampledPoints = nullptr;
    
    cmsToneCurve *curve = cmsBuildSegmentedToneCurve(ctx, 1, &segment);
    
    // 4. Create IT8 handle and set property using specified API
    // First, try to load IT8 from the input file
    fseek(in_file, 0, SEEK_SET);
    cmsHANDLE it8_handle = cmsIT8LoadFromFile(ctx, "input_file");
    
    // Create a simple IT8 file if loading fails
    if (!it8_handle) {
        // Write some IT8-like data to file
        FILE *temp_it8 = fopen("temp_it8.txt", "wb");
        if (temp_it8) {
            // Write minimal IT8 content
            fprintf(temp_it8, "CGATS.17\n");
            fprintf(temp_it8, "COLOR_REP RGB\n");
            fprintf(temp_it8, "NUMBER_OF_SETS 1\n");
            fprintf(temp_it8, "BEGIN_DATA\n");
            fprintf(temp_it8, "255 255 255\n");
            fprintf(temp_it8, "END_DATA\n");
            assert_file_closed(&temp_it8);;
            
            // Try to load again
            it8_handle = cmsIT8LoadFromFile(ctx, "temp_it8.txt");
        }
    }
    
    if (it8_handle) {
        // Use part of input data as property value
        char prop_buffer[256];
        size_t prop_size = (size < 255) ? size : 255;
        memcpy(prop_buffer, data, prop_size);
        prop_buffer[prop_size] = '\0';
        
        // Set property using specified API
        cmsIT8SetPropertyUncooked(it8_handle, "DESCRIPTOR", prop_buffer);
    }
    
    // 5. Save profile to stream using specified API
    // Note: cmsSaveProfileToStream is not in the provided API list,
    // but we can use cmsSaveProfileToFile or other available APIs
    cmsSaveProfileToFile(srgb_profile, "output_profile.icc");
    
    // Alternatively, we can write to the output file stream
    // using cmsGetProfileIOhandler and write directly
    cmsIOHANDLER *io_handler = cmsGetProfileIOhandler(srgb_profile);
    if (io_handler && out_file) {
        // Create a simple pipeline to demonstrate transform usage
        cmsHPROFILE profiles[] = {srgb_profile, lab_profile};
        cmsUInt32Number intent = INTENT_PERCEPTUAL;
        cmsUInt32Number flags = cmsFLAGS_NOCACHE;
        
        cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
            ctx, profiles, 2, TYPE_RGB_8, TYPE_Lab_8, intent, flags);
        
        if (transform) {
            // Use some input data for transform
            unsigned char rgb_input[3];
            unsigned short lab_output[3];
            
            for (size_t i = 0; i < 3 && i < size; i++) {
                rgb_input[i] = data[i];
            }
            
            cmsDoTransform(transform, rgb_input, lab_output, 1);
            cmsDeleteTransform(transform);
        }
    }
    
    // Cleanup
    if (curve) cmsFreeToneCurve(curve);
    if (it8_handle) cmsIT8Free(it8_handle);
    
    cmsCloseProfile(srgb_profile);
    cmsCloseProfile(lab_profile);
    
    // Remove temporary files
    remove("temp_it8.txt");
    remove("output_profile.icc");
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    remove("output_file");
    
    cmsDeleteContext(ctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}