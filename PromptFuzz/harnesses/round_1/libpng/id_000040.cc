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
//<ID> 711
//<Prompt> []
/*<Combination>: [
*/
//<score> 43.5, nr_unique_branch: 3
//<Quality> {"density":29,"unique_branches":{"png_set_invalid":[[1952,8,1952,23,0,0,4,0],[1952,27,1952,43,0,0,4,0]],"png_start_read_image":[[297,11,297,52,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_user_limits","png_init_io","png_read_info","png_get_color_type","png_get_bit_depth","png_set_invalid","png_set_cICP","png_set_palette_to_rgb","png_set_expand_gray_1_2_4_to_8","png_get_valid","png_set_tRNS_to_alpha","png_set_strip_16","png_set_background","png_set_bKGD","png_read_update_info","png_start_read_image","png_get_next_frame_width","png_set_quantize","png_get_rowbytes","png_malloc","png_get_image_height","png_read_row","png_read_end","png_free","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_user_limits","png_init_io","png_read_info","png_get_color_type","png_get_bit_depth","png_set_invalid","png_set_cICP","png_set_palette_to_rgb","png_set_expand_gray_1_2_4_to_8","png_get_valid","png_set_tRNS_to_alpha","png_set_strip_16","png_set_background","png_set_bKGD","png_read_update_info","png_start_read_image","png_get_next_frame_width","png_set_quantize","png_get_rowbytes","png_malloc","png_get_image_height","png_read_row","png_read_end","png_free","png_destroy_read_struct"],"visited":1}
/**/



extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<559) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegralArray(uint16_t, fuzz_uint16_t_array_8, fuzz_uint16_t_array_size_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
	
		const png_uint_16 * fuzzer_var_png_set_quantize_22_4 = fuzz_uint16_t_array_8;
		int fuzzer_size_png_set_quantize_22_4 = static_cast<int>(fuzz_uint16_t_array_size_8);//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need enough data for PNG header and basic operations
    
    // Create input file from memory buffer
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
    
    // Initialize libpng structures
    png_struct *png_ptr = png_create_read_struct(fuzz_str_1, 
                                                 nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set user limits (API #2)
    png_set_user_limits(png_ptr, fuzz_uint32_t_2, fuzz_uint32_t_3);
    
    // Set read function
    png_init_io(png_ptr, in_file);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Check if we have valid PNG data
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Set invalid chunks (API #1)
    // Mark tIME chunk as invalid to test error handling
    png_set_invalid(png_ptr, info_ptr, fuzz_int32_t_4);
    
    // Set cICP (API #7)
    // Set colorimetric information
    png_set_cICP(png_ptr, info_ptr, 
                 1,  // colour_primaries: BT.709
                 1,  // transfer_function: BT.709
                 1,  // matrix_coefficients: BT.709
                 0); // video_full_range_flag: limited range
    
    // Setup transformations based on image type
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }
    
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    }
    
    if (png_get_valid(png_ptr, info_ptr, fuzz_uint32_t_5)) {
        png_set_tRNS_to_alpha(png_ptr);
    }
    
    if (bit_depth == 16) {
        png_set_strip_16(png_ptr);
    }
    
    // Set background (API #5)
    png_color_16 background_color;
    background_color.red = 255;
    background_color.green = 255;
    background_color.blue = 255;
    background_color.gray = 255;
    background_color.index = 0;
    png_set_background(png_ptr, &background_color, 
                       fuzz_int32_t_6, fuzz_int32_t_7, 1.0);
    
    // Set bKGD (API #8)
    png_color_16 bkgd_color;
    bkgd_color.red = 200;
    bkgd_color.green = 200;
    bkgd_color.blue = 200;
    bkgd_color.gray = 200;
    bkgd_color.index = 0;
    png_set_bKGD(png_ptr, info_ptr, &bkgd_color);
    
    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Start reading image (API #6)
    png_start_read_image(png_ptr);
    
    // Get next frame width for APNG (API #3)
    // This will work if it's an APNG, otherwise returns current width
    png_uint_32 next_frame_width = png_get_next_frame_width(png_ptr, info_ptr);
    
    // Prepare quantization (API #4)
    png_color palette[256];
    png_uint_16 histogram[256] = {0};
    int num_palette = 256;
    
    // Create a simple grayscale palette
    for (int i = 0; i < 256; i++) {
        palette[i].red = i;
        palette[i].green = i;
        palette[i].blue = i;
        histogram[i] = i; // Simple histogram
    }
    
    png_set_quantize(png_ptr, palette, fuzz_int32_t_9, fuzzer_size_png_set_quantize_22_4, fuzzer_var_png_set_quantize_22_4, fuzz_int32_t_10);
    
    // Get int32 from buffer (API #9)
    // Use first 4 bytes of data if available
    png_int_32 int_val = 0;
    if (size >= 4) {
        int_val = png_get_int_32(data);
    }
    
    // Allocate row buffer
    png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    png_byte *row = (png_byte*)png_malloc(png_ptr, rowbytes);
    
    // Read image rows
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(png_ptr, row, nullptr);
    }
    
    // Read rest of image
    png_read_end(png_ptr, info_ptr);
    
    // Cleanup
    png_free(png_ptr, row);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}