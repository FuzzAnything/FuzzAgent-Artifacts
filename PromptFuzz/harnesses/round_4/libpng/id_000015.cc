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
//<ID> 510
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 0
//<Quality> {"density":45,"unique_branches":{},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_sCAL_s","png_get_mDCV","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_set_IHDR","png_set_scale_16","png_set_compression_window_bits","png_set_cICP","png_set_alpha_mode_fixed","png_set_cHRM_XYZ","png_set_flush","png_write_info","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_write_image","png_write_end","png_write_png","png_free","png_free","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_sCAL_s","png_get_mDCV","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_scale_16","png_set_compression_window_bits","png_set_cICP","png_set_alpha_mode_fixed","png_set_cHRM_XYZ","png_set_flush","png_write_info","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_write_image","png_write_end","png_write_png","png_free","png_free","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to read a PNG from input data, process it, and write it back out:

*/


extern "C" {
}

// Custom error handling functions
static void  id_000015_png_error_handler(png_structp png_ptr, const char* error_msg) {
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void  id_000015_png_warning_handler(png_structp png_ptr, const char* warning_msg) {
    // Do nothing for warnings in fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<68) return 0;

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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	  // Need at least PNG signature
    
    // Check if it's a PNG file
    if (png_sig_cmp(data, fuzz_uint64_t_1, 8) != 0) {
        return 0;
    }
    
    // Create read structure
    png_structp png_read_ptr = png_create_read_struct(
        fuzz_str_2, 
        nullptr, 
         id_000015_png_error_handler, 
         id_000015_png_warning_handler
    );
    if (!png_read_ptr) {
	return 0;
	}
	
    
    // Create info structure for reading
    png_infop read_info_ptr = png_create_info_struct(png_read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling with longjmp
    if (setjmp(png_jmpbuf(png_read_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        return 0;
    }
    
    // Create FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O and read PNG info
    png_init_io(png_read_ptr, in_file);
    png_set_sig_bytes(png_read_ptr, fuzz_int32_t_3);
    png_read_info(png_read_ptr, read_info_ptr);
    
    // Get image dimensions and color type
    png_uint_32 width = png_get_image_width(png_read_ptr, read_info_ptr);
    png_uint_32 height = png_get_image_height(png_read_ptr, read_info_ptr);
    png_byte color_type = png_get_color_type(png_read_ptr, read_info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_read_ptr, read_info_ptr);
    
    // Try to get sCAL information (if present)
    int unit = 0;
    char* swidth = nullptr;
    char* sheight = nullptr;
    png_get_sCAL_s(png_read_ptr, read_info_ptr, &unit, &swidth, &sheight);
    
    // Try to get mDCV information (if present)
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    double max_lum, min_lum;
    png_get_mDCV(png_read_ptr, read_info_ptr, &white_x, &white_y, &red_x, &red_y,
                 &green_x, &green_y, &blue_x, &blue_y, &max_lum, &min_lum);
    
    // Create write structure
    png_structp png_write_ptr = png_create_write_struct(
        fuzz_str_4,
        nullptr,
         id_000015_png_error_handler,
         id_000015_png_warning_handler
    );
    if (!png_write_ptr) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure for writing
    png_infop write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling for write structure
    if (setjmp(png_jmpbuf(png_write_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for writing
    png_init_io(png_write_ptr, out_file);
    
    // Set IHDR for output
    png_set_IHDR(png_write_ptr, write_info_ptr, width, height, bit_depth,
                 color_type, fuzz_int32_t_5, fuzz_int32_t_6,
                 fuzz_int32_t_7);
    
    // Apply various transformations
    if (bit_depth == 16) {
        png_set_scale_16(png_write_ptr);
    }
    
    // Set compression window bits
    png_set_compression_window_bits(png_write_ptr, fuzz_int32_t_8);
    
    // Set cICP with dummy values (if we had real values, we'd use them)
    png_set_cICP(png_write_ptr, write_info_ptr, 1, 1, 1, 0);
    
    // Set alpha mode
    png_set_alpha_mode_fixed(png_write_ptr, fuzz_int32_t_9, fuzz_int32_t_10);
    
    // Set cHRM XYZ with dummy values
    png_set_cHRM_XYZ(png_write_ptr, write_info_ptr, 0.64, 0.33, 0.30,
                     0.60, 0.15, 0.06, 0.33, 0.06, 0.01);
    
    // Set flush
    png_set_flush(png_write_ptr, fuzz_int32_t_11);
    
    // Write info
    png_write_info(png_write_ptr, write_info_ptr);
    
    // Allocate memory for image rows
    png_bytep* row_pointers = (png_bytep*)png_malloc(
        png_write_ptr, height * sizeof(png_bytep));
    
    size_t rowbytes = png_get_rowbytes(png_read_ptr, read_info_ptr);
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_write_ptr, rowbytes);
    }
    
    // Read image data
    png_read_image(png_read_ptr, row_pointers);
    
    // Write image data
    png_write_image(png_write_ptr, row_pointers);
    
    // Write end
    png_write_end(png_write_ptr, write_info_ptr);
    
    // Also write using png_write_png
    png_write_png(png_write_ptr, write_info_ptr, fuzz_int32_t_12, nullptr);
    
    // Free allocated memory
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_write_ptr, row_pointers[y]);
    }
    png_free(png_write_ptr, row_pointers);
    
    // Clean up sCAL strings if allocated
    if (swidth) png_free(png_read_ptr, swidth);
    if (sheight) png_free(png_read_ptr, sheight);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Destroy structures
    png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}