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
#include <png.h>
}
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 69, nr_unique_branch: 68
//<Quality> {"density":23,"unique_branches":{"png_fp_sub":[[1071,8,1071,19,0,0,4,0],[1073,11,1073,43,0,0,4,0]],"png_set_compression_method":[[1267,8,1267,23,0,0,4,1],[1273,8,1273,19,0,0,4,0],[1273,8,1273,19,0,0,4,1]],"png_XYZ_from_xy":[[1195,8,1195,22,0,0,4,1],[1195,26,1195,44,0,0,4,1],[1196,8,1196,22,0,0,4,1],[1196,26,1196,53,0,0,4,1],[1197,8,1197,22,0,0,4,1],[1197,26,1197,46,0,0,4,1],[1198,8,1198,22,0,0,4,1],[1198,26,1198,57,0,0,4,1],[1199,8,1199,22,0,0,4,1],[1199,26,1199,45,0,0,4,1],[1200,8,1200,22,0,0,4,1],[1200,26,1200,55,0,0,4,1],[1201,8,1201,22,0,0,4,1],[1201,26,1201,46,0,0,4,1],[1202,8,1202,22,0,0,4,1],[1202,26,1202,57,0,0,4,1],[1394,11,1394,80,0,0,4,1],[1396,11,1397,14,0,0,4,1],[1400,11,1400,16,0,0,4,1],[1403,11,1403,80,0,0,4,1],[1405,11,1406,14,0,0,4,1],[1414,11,1415,59,0,0,4,1],[1415,63,1415,68,0,0,4,1],[1416,11,1416,36,0,0,4,1],[1420,11,1420,78,0,0,4,1],[1422,11,1422,79,0,0,4,1],[1424,11,1425,59,0,0,4,1],[1425,63,1425,68,0,0,4,1],[1426,11,1426,38,0,0,4,1],[1435,11,1435,16,0,0,4,1],[1435,20,1435,35,0,0,4,1],[1443,8,1443,69,0,0,4,1],[1445,8,1445,69,0,0,4,1],[1447,8,1448,25,0,0,4,1],[1451,8,1451,75,0,0,4,1],[1453,8,1453,75,0,0,4,1],[1455,8,1456,27,0,0,4,1],[1459,8,1459,70,0,0,4,1],[1461,8,1461,70,0,0,4,1],[1463,8,1464,22,0,0,4,1]],"png_get_cHRM_XYZ_fixed":[[608,8,608,23,0,0,4,0],[608,27,608,43,0,0,4,0],[609,8,609,47,0,0,4,0],[609,8,609,47,0,0,4,1],[610,8,610,51,0,0,4,0],[612,11,612,28,0,0,4,0],[613,11,613,28,0,0,4,0],[614,11,614,28,0,0,4,0],[615,11,615,30,0,0,4,0],[616,11,616,30,0,0,4,0],[617,11,617,30,0,0,4,0],[618,11,618,29,0,0,4,0],[619,11,619,29,0,0,4,0],[620,11,620,29,0,0,4,0]],"png_get_pixel_aspect_ratio_fixed":[[219,8,219,23,0,0,4,0],[219,27,219,43,0,0,4,0],[220,8,220,46,0,0,4,0],[220,8,220,46,0,0,4,1],[221,8,221,39,0,0,4,0],[221,43,221,74,0,0,4,0],[222,8,222,54,0,0,4,0],[223,8,223,54,0,0,4,0],[230,11,231,56,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_read_info","png_get_pixel_aspect_ratio_fixed","png_get_cHRM_XYZ_fixed","png_chunk_warning","png_init_io","png_get_IHDR","png_set_IHDR","png_set_compression_method","png_set_cICP","png_write_info","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_read_info","png_get_pixel_aspect_ratio_fixed","png_get_cHRM_XYZ_fixed","png_chunk_warning","png_init_io","png_get_IHDR","png_set_IHDR","png_set_compression_method","png_set_cICP","png_write_info","png_destroy_write_struct","png_destroy_read_struct"],"visited":1}
/**/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<62) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for PNG operations
    
    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for writing output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize read structure
    png_struct *png_read_ptr = png_create_read_struct(
        fuzz_str_1, NULL, NULL, NULL);
    if (!png_read_ptr) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize info structure for reading
    png_info *read_info_ptr = png_create_info_struct(png_read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, NULL, NULL);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize write structure
    png_struct *png_write_ptr = png_create_write_struct(
        fuzz_str_2, NULL, NULL, NULL);
    if (!png_write_ptr) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, NULL);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize info structure for writing
    png_info *write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&png_write_ptr, NULL);
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, NULL);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_read_ptr)) || setjmp(png_jmpbuf(png_write_ptr))) {
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, NULL);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up read from file
    png_init_io(png_read_ptr, in_file);
    
    // Read PNG info
    png_read_info(png_read_ptr, read_info_ptr);
    
    // Get pixel aspect ratio (API #4)
    png_fixed_point aspect_ratio = png_get_pixel_aspect_ratio_fixed(
        png_read_ptr, read_info_ptr);
    
    // Declare variables for cHRM XYZ data
    png_fixed_point red_X, red_Y, red_Z;
    png_fixed_point green_X, green_Y, green_Z;
    png_fixed_point blue_X, blue_Y, blue_Z;
    
    // Get cHRM XYZ data (API #1)
    png_uint_32 has_chrm = png_get_cHRM_XYZ_fixed(
        png_read_ptr, read_info_ptr,
        &red_X, &red_Y, &red_Z,
        &green_X, &green_Y, &green_Z,
        &blue_X, &blue_Y, &blue_Z);
    
    // Issue warning if no cHRM data found (API #3)
    if (!has_chrm) {
        png_chunk_warning(png_read_ptr, 
            fuzz_str_3);
    }
    
    // Set up write to file
    png_init_io(png_write_ptr, out_file);
    
    // Set basic image info for writing
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    png_get_IHDR(png_read_ptr, read_info_ptr, &width, &height,
                 &bit_depth, &color_type, &interlace_type,
                 &compression_type, &filter_type);
    
    png_set_IHDR(png_write_ptr, write_info_ptr, width, height,
                 bit_depth, color_type, interlace_type,
                 compression_type, filter_type);
    
    // Set compression method (API #2)
    // Use 0-9 range (valid compression methods)
    int compression_method = size % 10;
    png_set_compression_method(png_write_ptr, compression_method);
    
    // Set CICP values (API #5)
    // Extract values from input data for fuzzing
    png_byte colour_primaries = data[0] % 16; // Valid range 0-15
    png_byte transfer_function = data[1] % 16; // Valid range 0-15
    png_byte matrix_coefficients = data[2] % 16; // Valid range 0-15
    png_byte video_full_range_flag = data[3] % 2; // 0 or 1
    
    png_set_cICP(png_write_ptr, write_info_ptr,
                 colour_primaries, transfer_function,
                 matrix_coefficients, video_full_range_flag);
    
    // Write PNG info
    png_write_info(png_write_ptr, write_info_ptr);
    
    // Clean up
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
    png_destroy_read_struct(&png_read_ptr, &read_info_ptr, NULL);
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}