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
//<ID> 256
//<Prompt> []
/*<Combination>: [
*/
//<score> 203, nr_unique_branch: 49
//<Quality> {"density":29,"unique_branches":{"png_set_sCAL_s":[[612,8,612,17,0,0,4,0],[612,21,612,30,0,0,4,0]],"png_ascii_from_fp":[[2297,8,2297,21,0,0,4,1],[2301,8,2301,29,0,0,4,1],[2305,8,2305,27,0,0,4,0],[2307,11,2307,17,0,0,4,1],[2314,11,2314,24,0,0,4,0],[2314,28,2314,41,0,0,4,0],[2335,17,2335,31,0,0,4,1],[2335,35,2335,44,0,0,4,0],[2335,35,2335,44,0,0,4,1],[2340,17,2340,32,0,0,4,0],[2357,17,2357,24,0,0,4,1],[2375,17,2375,28,0,0,4,1],[2399,20,2399,53,0,0,4,0],[2399,20,2399,53,0,0,4,1],[2406,23,2406,28,0,0,4,1],[2471,20,2471,26,0,0,4,1],[2482,26,2482,35,0,0,4,1],[2500,23,2500,38,0,0,4,0],[2500,23,2500,38,0,0,4,1],[2502,26,2502,38,0,0,4,0],[2502,26,2502,38,0,0,4,1],[2512,20,2512,51,0,0,4,0],[2512,20,2512,51,0,0,4,1],[2512,55,2512,67,0,0,4,0],[2523,17,2523,32,0,0,4,0],[2523,36,2523,48,0,0,4,0],[2532,23,2532,36,0,0,4,1]],"png_get_pHYs":[[1094,8,1094,23,0,0,4,0],[1094,27,1094,43,0,0,4,0],[1095,8,1095,46,0,0,4,0],[1095,8,1095,46,0,0,4,1],[1097,11,1097,24,0,0,4,0],[1103,11,1103,24,0,0,4,0],[1109,11,1109,28,0,0,4,0]],"png_pow10":[[2260,8,2260,17,0,0,4,1],[2266,8,2266,17,0,0,4,0],[2266,8,2266,17,0,0,4,1],[2272,14,2272,23,0,0,4,0],[2276,14,2276,23,0,0,4,1],[2278,11,2278,21,0,0,4,1]],"png_get_x_pixels_per_meter":[[131,8,131,23,0,0,4,0],[131,27,131,43,0,0,4,0],[132,8,132,46,0,0,4,0],[132,8,132,46,0,0,4,1],[134,11,134,59,0,0,4,0]],"png_set_sCAL":[[671,8,671,18,0,0,4,1],[674,13,674,24,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_pHYs","png_get_x_pixels_per_meter","png_get_pixel_aspect_ratio","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_get_IHDR","png_set_IHDR","png_set_sCAL","png_set_text_compression_window_bits","png_save_uint_32","png_write_info","png_get_rowbytes","png_read_row","png_write_row","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_pHYs","png_get_x_pixels_per_meter","png_get_pixel_aspect_ratio","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_IHDR","png_set_IHDR","png_set_sCAL","png_set_text_compression_window_bits","png_save_uint_32","png_write_info","png_get_rowbytes","png_read_row","png_write_row","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/*Here's a fuzz driver that uses the specified libpng APIs to process PNG image data:

*/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for PNG operations

    // Create a temporary file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_infop end_info = nullptr;
    
    // Initialize libpng structures
    png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, in_file);
    
    // Read the PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Get physical dimensions (pHYs chunk)
    png_uint_32 res_x, res_y;
    int unit_type;
    png_uint_32 phys_result = png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    
    // Get x pixels per meter
    png_uint_32 x_pixels_per_meter = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    
    // Get pixel aspect ratio
    float pixel_aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);
    
    // Create write structure for output
    png_structp write_ptr = png_create_write_struct(fuzz_str_2, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize write I/O
    png_init_io(write_ptr, out_file);
    
    // Copy basic image info from read to write
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, &compression_type, &filter_type);
    
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    // Set sCAL chunk using data from input
    double scale_width = 1.0;
    double scale_height = 1.0;
    if (size > 20) {
        // Use some bytes from input to determine scale values
        scale_width = 0.1 + (data[0] / 255.0) * 10.0;
        scale_height = 0.1 + (data[1] / 255.0) * 10.0;
    }
    png_set_sCAL(write_ptr, write_info, fuzz_int32_t_3, scale_width, scale_height);
    
    // Set text compression window bits
    int window_bits = 15;  // Default value
    if (size > 30) {
        window_bits = 9 + (data[2] % 7);  // Range 9-15
    }
    png_set_text_compression_window_bits(write_ptr, fuzz_int32_t_4);
    
    // Use png_save_uint_32 to save some data
    png_byte buffer[4];
    png_uint_32 value_to_save = 0;
    if (size >= 8) {
        value_to_save = png_get_uint_32(data + 4);
    }
    png_save_uint_32(buffer, fuzz_uint32_t_5);
    
    // Write the PNG data
    png_write_info(write_ptr, write_info);
    
    // Read and write image data
    png_bytep row = (png_bytep)malloc(png_get_rowbytes(png_ptr, info_ptr));
    if (row) {
        for (png_uint_32 y = 0; y < height; y++) {
            png_read_row(png_ptr, row, nullptr);
            png_write_row(write_ptr, row);
        }
        free(row);
    }
    
    png_write_end(write_ptr, write_info);
    
    // Clean up
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}