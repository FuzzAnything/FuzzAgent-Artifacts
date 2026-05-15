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
//<ID> 478
//<Prompt> []
/*<Combination>: [
*/
//<score> 86, nr_unique_branch: 1
//<Quality> {"density":43,"unique_branches":{"png_set_swap_alpha":[[255,8,255,23,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_set_sig_bytes","png_set_packing","png_set_swap_alpha","png_set_benign_errors","png_read_info","png_get_IHDR","png_set_expand","png_set_strip_16","png_set_palette_to_rgb","png_set_swap_alpha","png_read_update_info","png_get_IHDR","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_read_end","png_init_io","png_set_benign_errors","png_set_filter","png_set_write_user_transform_fn","png_set_IHDR","png_set_first_frame_is_hidden","png_write_info","png_write_image","png_write_end","png_free","png_free","png_get_x_offset_pixels","png_get_pixels_per_meter","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_packing","png_set_swap_alpha","png_set_benign_errors","png_read_info","png_get_IHDR","png_set_expand","png_set_strip_16","png_set_palette_to_rgb","png_set_swap_alpha","png_read_update_info","png_get_IHDR","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_read_end","png_init_io","png_set_benign_errors","png_set_filter","png_set_write_user_transform_fn","png_set_IHDR","png_set_first_frame_is_hidden","png_write_info","png_write_image","png_write_end","png_free","png_free","png_get_x_offset_pixels","png_get_pixels_per_meter","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


// Include necessary headers for libpng

// User transform function - required by png_set_write_user_transform_fn
static void  id_000028_user_transform_fn(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    // Simple example transform: invert all bytes in the row
    for (png_size_t i = 0; i < row_info->rowbytes; i++) {
        data[i] = ~data[i];
    }
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
        return 0;  // Need at least PNG signature
    }
    
    // Check PNG signature
    if (png_sig_cmp(data, 0, 8) != 0) {
        return 0;  // Not a PNG file
    }
    
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
    
    // Initialize structures
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set error handling
    if (setjmp(png_jmpbuf(read_ptr)) || setjmp(png_jmpbuf(write_ptr))) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 1: Setup reading
    png_init_io(read_ptr, in_file);
    png_set_sig_bytes(read_ptr, 8);  // Already checked signature
    
    // Apply required APIs for reading
    png_set_packing(read_ptr);  // Pack pixels to bytes
    png_set_swap_alpha(read_ptr);  // Swap RGBA to ARGB or BGRA to ABGR
    png_set_benign_errors(read_ptr, 1);  // Allow benign errors
    
    // Read image info
    png_read_info(read_ptr, read_info);
    
    // Get image dimensions
    png_uint_32 width, height;
    int bit_depth, color_type;
    png_get_IHDR(read_ptr, read_info, &width, &height, &bit_depth, 
                 &color_type, nullptr, nullptr, nullptr);
    
    // Apply transformations
    png_set_expand(read_ptr);
    if (bit_depth == 16) {
        png_set_strip_16(read_ptr);
    }
    if (color_type & PNG_COLOR_MASK_PALETTE) {
        png_set_palette_to_rgb(read_ptr);
    }
    if (color_type & PNG_COLOR_MASK_ALPHA) {
        png_set_swap_alpha(read_ptr);
    }
    
    // Update info after transformations
    png_read_update_info(read_ptr, read_info);
    png_get_IHDR(read_ptr, read_info, &width, &height, &bit_depth, 
                 &color_type, nullptr, nullptr, nullptr);
    
    // Read image data
    png_bytep* row_pointers = (png_bytep*)png_malloc(read_ptr, height * sizeof(png_bytep));
    png_size_t rowbytes = png_get_rowbytes(read_ptr, read_info);
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(read_ptr, rowbytes);
    }
    
    png_read_image(read_ptr, row_pointers);
    png_read_end(read_ptr, nullptr);
    
    // Step 2: Setup writing
    png_init_io(write_ptr, out_file);
    
    // Apply required APIs for writing
    png_set_benign_errors(write_ptr, 1);  // Allow benign errors
    png_set_filter(write_ptr, 0, PNG_FILTER_NONE);  // Set filter method
    png_set_write_user_transform_fn(write_ptr,  id_000028_user_transform_fn);  // Set user transform
    
    // Set image dimensions for writing
    png_set_IHDR(write_ptr, write_info, width, height, 8, 
                 PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, 
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    // Set first frame as hidden (for APNG) - use API from step 1
    // Note: This function may not be available in all libpng versions
    // We'll call it but it may have no effect on non-APNG images
    png_byte is_hidden = 0;
    png_set_first_frame_is_hidden(write_ptr, write_info, is_hidden);
    
    // Write header
    png_write_info(write_ptr, write_info);
    
    // Write image data
    png_write_image(write_ptr, row_pointers);
    png_write_end(write_ptr, nullptr);
    
    // Cleanup reading structures
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(read_ptr, row_pointers[y]);
    }
    png_free(read_ptr, row_pointers);
    
    // Use the remaining required getter APIs
    // These are called but results may not be used
    png_int_32 x_offset = png_get_x_offset_pixels(read_ptr, read_info);
    png_uint_32 pixels_per_meter = png_get_pixels_per_meter(read_ptr, read_info);
    
    // Clean up all resources
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    png_destroy_write_struct(&write_ptr, &write_info);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}