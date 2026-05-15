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
//<ID> 649
//<Prompt> []
/*<Combination>: [
*/
//<score> 224, nr_unique_branch: 24
//<Quality> {"density":32,"unique_branches":{"png_xy_from_XYZ":[[1118,8,1118,48,0,0,4,1],[1121,8,1121,62,0,0,4,1],[1123,8,1123,62,0,0,4,1],[1127,8,1127,52,0,0,4,1],[1130,8,1130,68,0,0,4,1],[1132,8,1132,68,0,0,4,1],[1136,8,1136,50,0,0,4,1],[1139,8,1139,65,0,0,4,1],[1141,8,1141,65,0,0,4,1],[1149,8,1149,38,0,0,4,1],[1157,8,1157,51,0,0,4,1],[1162,8,1162,51,0,0,4,1],[1166,8,1166,62,0,0,4,1],[1168,8,1168,62,0,0,4,1]],"png_fp_add":[[1050,8,1050,19,0,0,4,0],[1050,8,1050,19,0,0,4,1],[1052,11,1052,42,0,0,4,0],[1055,13,1055,24,0,0,4,1]],"png_chunk_benign_error":[[456,8,456,59,0,0,4,1]],"png_safe_add":[[1099,8,1099,14,0,0,4,0]],"png_set_cHRM_XYZ_fixed":[[74,8,74,23,0,0,4,1],[74,27,74,43,0,0,4,1],[87,8,87,39,0,0,4,0]],"png_set_compression_method":[[1273,8,1273,19,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_y_offset_inches","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_get_IHDR","png_set_IHDR","png_set_compression_method","png_set_gamma_fixed","png_set_cHRM_XYZ_fixed","png_set_iCCP","png_chunk_benign_error","png_convert_from_time_t","png_write_info","png_malloc","png_get_rowbytes","png_get_rowbytes","png_write_row","png_write_end","png_data_freer","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_y_offset_inches","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_IHDR","png_set_IHDR","png_set_compression_method","png_set_gamma_fixed","png_set_cHRM_XYZ_fixed","png_set_iCCP","png_chunk_benign_error","png_convert_from_time_t","png_write_info","png_malloc","png_get_rowbytes","png_get_rowbytes","png_write_row","png_write_end","png_data_freer","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful processing
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 1: Read PNG from memory
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                       nullptr, nullptr, nullptr);
    if (!png_ptr_read) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr_read))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(png_ptr_read, in_file);
    png_read_info(png_ptr_read, info_ptr_read);
    
    // Step 2: Get y offset in inches
    float y_offset_inches = png_get_y_offset_inches(png_ptr_read, info_ptr_read);
	FUZZ_FILENAME_PUSH("output_file")
    
    // Step 3: Create write structure for output
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                        nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(png_ptr_write))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(png_ptr_write, out_file);
    
    // Step 4: Copy basic info from read to write
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    
    png_get_IHDR(png_ptr_read, info_ptr_read, &width, &height, &bit_depth,
                 &color_type, &interlace_type, &compression_type, &filter_type);
    
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height, bit_depth,
                 color_type, interlace_type, compression_type, filter_type);
    
    // Step 5: Set compression method (using first 4 bytes for method value)
    int method = data[0] % 10; // Ensure valid range
    png_set_compression_method(png_ptr_write, method);
    
    // Step 6: Set gamma values using data bytes
    png_fixed_point screen_gamma = 45000; // 0.45 in fixed-point
    png_fixed_point override_file_gamma = 55000; // 0.55 in fixed-point
    
    // Use data bytes to influence gamma values
    if (size >= 8) {
        screen_gamma = (png_fixed_point)(data[4] * 1000);
        override_file_gamma = (png_fixed_point)(data[5] * 1000);
    }
    png_set_gamma_fixed(png_ptr_write, screen_gamma, override_file_gamma);
    
    // Step 7: Get uint32 from buffer
    if (size >= 12) {
        png_uint_32 extracted_value = png_get_uint_32((const png_byte*)&data[8]);
        (void)extracted_value; // Use value to avoid unused warning
    }
    
    // Step 8: Set cHRM XYZ values
    png_fixed_point red_X = 31270, red_Y = 32900, red_Z = 35830;
    png_fixed_point green_X = 63990, green_Y = 33000, green_Z = 3000;
    png_fixed_point blue_X = 16500, blue_Y = 16000, blue_Z = 74920;
    
    // Use data to influence cHRM values if enough bytes available
    if (size >= 44) {
        red_X = (png_fixed_point)(data[12] * 1000);
        red_Y = (png_fixed_point)(data[13] * 1000);
        red_Z = (png_fixed_point)(data[14] * 1000);
        green_X = (png_fixed_point)(data[15] * 1000);
        green_Y = (png_fixed_point)(data[16] * 1000);
        green_Z = (png_fixed_point)(data[17] * 1000);
        blue_X = (png_fixed_point)(data[18] * 1000);
        blue_Y = (png_fixed_point)(data[19] * 1000);
        blue_Z = (png_fixed_point)(data[20] * 1000);
    }
    
    png_set_cHRM_XYZ_fixed(png_ptr_write, info_ptr_write,
                          red_X, red_Y, red_Z,
                          green_X, green_Y, green_Z,
                          blue_X, blue_Y, blue_Z);
    
    // Step 9: Set iCCP profile (use part of data as profile)
    const char* icc_name = "Test Profile";
    int icc_compression_type = PNG_COMPRESSION_TYPE_BASE;
    png_uint_32 proflen = 0;
    const png_byte* profile_data = nullptr;
    
    if (size >= 100) {
        proflen = (png_uint_32)(size - 50 > 50 ? 50 : size - 50);
        profile_data = (const png_byte*)&data[50];
        png_set_iCCP(png_ptr_write, info_ptr_write, icc_name,
                    icc_compression_type, profile_data, proflen);
    }
    
    // Step 10: Call benign error (simulate a warning)
    png_chunk_benign_error(png_ptr_write, "Test benign error from fuzzer");
    
    // Step 11: Convert time
    time_t current_time = time(nullptr);
    png_time png_time_struct;
    png_convert_from_time_t(&png_time_struct, current_time);
    
    // Step 12: Write image data
    png_write_info(png_ptr_write, info_ptr_write);
    
    // Allocate row buffer
    png_bytep row = (png_bytep)png_malloc(png_ptr_write, 
                                         png_get_rowbytes(png_ptr_write, info_ptr_write));
    
    // Write some dummy rows
    for (png_uint_32 y = 0; y < height && y < 10; y++) {
        memset(row, y % 256, png_get_rowbytes(png_ptr_write, info_ptr_write));
        png_write_row(png_ptr_write, row);
    }
    
    png_write_end(png_ptr_write, info_ptr_write);
    
    // Step 13: Free data using png_data_freer
    png_data_freer(png_ptr_read, info_ptr_read, PNG_DESTROY_WILL_FREE_DATA, PNG_FREE_ALL);
    
    // Cleanup
    if (row) png_free(png_ptr_write, row);
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}