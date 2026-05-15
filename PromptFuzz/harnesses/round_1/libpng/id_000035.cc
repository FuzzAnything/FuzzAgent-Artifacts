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
//<ID> 568
//<Prompt> []
/*<Combination>: [
*/
//<score> 68.333336, nr_unique_branch: 7
//<Quality> {"density":41,"unique_branches":{"png_set_first_frame_is_hidden":[[1463,8,1463,23,0,0,4,1],[1466,8,1466,17,0,0,4,1]],"png_rtran_ok":[[106,29,106,65,0,0,4,0]],"png_get_iCCP":[[724,8,724,23,0,0,4,0],[724,27,724,43,0,0,4,0],[725,8,725,46,0,0,4,1]],"png_set_rgb_to_gray_fixed":[[1039,7,1039,33,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_get_iCCP","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_set_first_frame_is_hidden","png_set_rgb_to_gray","png_set_compression_mem_level","png_set_option","png_read_end","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_write_png","png_free","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_iCCP","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_set_first_frame_is_hidden","png_set_rgb_to_gray","png_set_compression_mem_level","png_set_option","png_read_end","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_write_png","png_free","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<56) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    // Early return if input is too small to be a valid PNG
    if (size < 8) {
	return 0;
	}
	
    
    // Open input from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Check PNG signature
    unsigned char sig[8];
    if (fread(sig, 1, 8, in_file) != 8) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    if (png_sig_cmp(sig, fuzz_uint64_t_1, 8) != 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create read structure
    png_struct *read_ptr = png_create_read_struct(fuzz_str_2, 
                                                   nullptr, nullptr, nullptr);
    if (!read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure
    png_info *read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(read_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize IO
    png_init_io(read_ptr, in_file);
    png_set_sig_bytes(read_ptr, fuzz_int32_t_3);
    
    // Read PNG info
    png_read_info(read_ptr, read_info);
    
    // Call png_get_iCCP to get ICC profile if present
    char *name = nullptr;
    int compression_type = 0;
    png_byte *profile = nullptr;
    png_uint_32 proflen = 0;
    png_uint_32 iccp_result = png_get_iCCP(read_ptr, read_info, &name, 
                                           &compression_type, &profile, &proflen);
    
    // Create write structure for output
    png_struct *write_ptr = png_create_write_struct(fuzz_str_4, 
                                                    nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create write info structure
    png_info *write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling for write
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Open output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize write IO
    png_init_io(write_ptr, out_file);
    
    // Copy image data from read to write structures
    png_uint_32 width = png_get_image_width(read_ptr, read_info);
    png_uint_32 height = png_get_image_height(read_ptr, read_info);
    int bit_depth = png_get_bit_depth(read_ptr, read_info);
    int color_type = png_get_color_type(read_ptr, read_info);
    int interlace_method = png_get_interlace_type(read_ptr, read_info);
    int compression_method = png_get_compression_type(read_ptr, read_info);
    int filter_method = png_get_filter_type(read_ptr, read_info);
    
    // Set IHDR for output
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth,
                 color_type, interlace_method, compression_method, filter_method);
    
    // Call png_set_first_frame_is_hidden (for APNG support)
    png_uint_32 hidden_result = png_set_first_frame_is_hidden(write_ptr, write_info, 0);
    
    // Call png_set_rgb_to_gray_fixed for potential RGB to grayscale conversion
    // Using standard coefficients for RGB to grayscale conversion
    png_fixed_point red_coef = 21265;   // 0.2126 * 100000
    png_fixed_point green_coef = 71514; // 0.7152 * 100000
    // Note: png_set_rgb_to_gray_fixed is not in the provided list, but we use
    // png_set_rgb_to_gray with fixed-point parameters
    png_set_rgb_to_gray(write_ptr, fuzz_int32_t_5, red_coef/100000.0, green_coef/100000.0);
    
    // Call png_set_compression_mem_level
    png_set_compression_mem_level(write_ptr, fuzz_int32_t_6);
    
    // Call png_set_option
    int option_result = png_set_option(write_ptr, fuzz_int32_t_7, fuzz_int32_t_8);
    
    // Copy other chunks from input to output
    png_read_end(read_ptr, nullptr);
    
    // Prepare for writing
    png_write_info(write_ptr, write_info);
    
    // Allocate row buffers
    png_bytep row = (png_bytep)png_malloc(read_ptr, png_get_rowbytes(read_ptr, read_info));
    
    // Read and write image data
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(read_ptr, row, nullptr);
        png_write_row(write_ptr, row);
    }
    
    // Write PNG using png_write_png (convenience function)
    // First, we need to set up transforms
    int transforms = PNG_TRANSFORM_IDENTITY;
    png_write_png(write_ptr, write_info, fuzz_int32_t_9, nullptr);
    
    // Clean up
    png_free(read_ptr, row);
    png_write_end(write_ptr, nullptr);
    
    // Close files
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Destroy structures
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}