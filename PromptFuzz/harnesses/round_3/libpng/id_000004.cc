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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 62, nr_unique_branch: 8
//<Quality> {"density":31,"unique_branches":{"png_rtran_ok":[[106,16,106,25,0,0,4,0],[106,29,106,65,0,0,4,0]],"png_set_rgb_to_gray_fixed":[[1034,8,1034,37,0,0,4,1],[1039,7,1039,33,0,0,4,1],[1043,7,1043,33,0,0,4,1],[1047,7,1047,34,0,0,4,1],[1051,7,1051,14,0,0,4,1]],"png_set_expand_16":[[1003,8,1003,37,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_init_io","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_expand_16","png_set_rgb_to_gray_fixed","png_set_write_status_fn","png_set_iCCP","png_convert_from_time_t","png_set_tIME","png_set_IHDR","png_write_info","png_get_rowbytes","png_read_image","png_write_image","png_write_end","png_get_io_chunk_type","png_image_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_init_io","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_expand_16","png_set_rgb_to_gray_fixed","png_set_write_status_fn","png_set_iCCP","png_convert_from_time_t","png_set_tIME","png_set_IHDR","png_write_info","png_get_rowbytes","png_read_image","png_write_image","png_write_end","png_get_io_chunk_type","png_image_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":1}
/**/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<68) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_7, fuzz_char_array_size_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	
		const png_byte * fuzzer_var_png_set_iCCP_22_4 = fuzz_char_array_7;
		png_uint_32 fuzzer_size_png_set_iCCP_22_4 = static_cast<png_uint_32>(fuzz_char_array_size_7);//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful PNG operations

    // Create input FILE* from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output FILE* for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize libpng structures
    png_struct *png_read_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_info *read_info_ptr = png_create_info_struct(png_read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_struct *png_write_ptr = png_create_write_struct(fuzz_str_2, nullptr, nullptr, nullptr);
    if (!png_write_ptr) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_info *write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_read_ptr)) || setjmp(png_jmpbuf(png_write_ptr))) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up I/O
    png_init_io(png_read_ptr, in_file);
    png_init_io(png_write_ptr, out_file);

    // Read PNG info
    png_read_info(png_read_ptr, read_info_ptr);

    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_read_ptr, read_info_ptr);
    png_uint_32 height = png_get_image_height(png_read_ptr, read_info_ptr);
    png_byte color_type = png_get_color_type(png_read_ptr, read_info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_read_ptr, read_info_ptr);

    // Set up transformations for writing
    png_set_expand_16(png_write_ptr);  // Expand to 16-bit if needed
    
    // Set RGB to gray conversion with fixed point coefficients
    // Using default coefficients: red = 0.2126, green = 0.7152
    png_fixed_point red_coeff = 2126;   // 0.2126 * 10000
    png_fixed_point green_coeff = 7152; // 0.7152 * 10000
    png_set_rgb_to_gray_fixed(png_write_ptr, fuzz_int32_t_3, fuzz_int32_t_4, fuzz_int32_t_5);

    // Set write status function
    png_set_write_status_fn(png_write_ptr, [](png_structp png_ptr, png_uint_32 row, int pass) {
        // Simple callback for write status
    });

    // Create a dummy ICC profile
    const char *icc_name = "sRGB";
    png_byte icc_profile[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
    png_uint_32 icc_profile_len = sizeof(icc_profile);
    
    // Set ICC profile (using read_info_ptr as source for this example)
    png_set_iCCP(png_write_ptr, write_info_ptr, fuzz_str_6, fuzz_int32_t_8, fuzzer_var_png_set_iCCP_22_4, fuzzer_size_png_set_iCCP_22_4);

    // Convert time for timestamp
    png_time png_time_struct;
    time_t current_time = time(nullptr);
    png_convert_from_time_t(&png_time_struct, current_time);
    png_set_tIME(png_write_ptr, write_info_ptr, &png_time_struct);

    // Copy relevant info from read to write struct
    png_set_IHDR(png_write_ptr, write_info_ptr, width, height, bit_depth,
                 color_type, fuzz_int32_t_9,
                 fuzz_int32_t_10, fuzz_int32_t_11);

    // Write info
    png_write_info(png_write_ptr, write_info_ptr);

    // Read and write image data
    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_read_ptr, read_info_ptr));
    }
    
    png_read_image(png_read_ptr, row_pointers);
    png_write_image(png_write_ptr, row_pointers);
    png_write_end(png_write_ptr, nullptr);

    // Get IO chunk type (for reading)
    png_uint_32 chunk_type = png_get_io_chunk_type(png_read_ptr);
    (void)chunk_type; // Use variable to avoid unused warning

    // Create and free png_image structure
    png_image image_info;
    memset(&image_info, 0, sizeof(image_info));
    image_info.version = PNG_IMAGE_VERSION;
    image_info.width = width;
    image_info.height = height;
    image_info.format = PNG_FORMAT_RGBA;
    
    // Free the png_image structure
    png_image_free(&image_info);

    // Clean up
    for (png_uint_32 y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    
    png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}