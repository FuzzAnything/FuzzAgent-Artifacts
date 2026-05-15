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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 29, nr_unique_branch: 0
//<Quality> {"density":29,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_read_info","png_get_image_width","png_get_image_height","png_get_uint_31","png_save_uint_16","png_get_mDCV_fixed","png_get_num_frames","png_get_first_frame_is_hidden","png_get_next_frame_delay_num","png_get_mem_ptr","png_benign_error","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_IHDR","png_set_IHDR","png_write_info","png_get_rowbytes","png_read_image","png_write_image","png_write_end","png_destroy_write_struct","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_image_width","png_get_image_height","png_get_uint_31","png_save_uint_16","png_get_mDCV_fixed","png_get_num_frames","png_get_first_frame_is_hidden","png_get_next_frame_delay_num","png_get_mem_ptr","png_benign_error","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_IHDR","png_set_IHDR","png_write_info","png_get_rowbytes","png_read_image","png_write_image","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<51) return 0;

	FUZZ_FILE_INIT();
	
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

    // Create read structure
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_ptr) {
	return 0;
	}
	
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Create input file from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, in_file);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    
    // Use png_get_uint_31 to read a 31-bit value from data
    png_uint_32 uint31_val = 0;
    if (size >= 4) {
        uint31_val = png_get_uint_31(png_ptr, data);
    }
    
    // Use png_save_uint_16 to save a 16-bit value
    png_byte buf[2];
    png_save_uint_16(buf, uint31_val & 0xFFFF);
    
    // Try to get mDCV fixed point values
    png_fixed_point int_white_x, int_white_y;
    png_fixed_point int_red_x, int_red_y;
    png_fixed_point int_green_x, int_green_y;
    png_fixed_point int_blue_x, int_blue_y;
    png_uint_32 max_lum_scaled, min_lum_scaled;
    
    // This may fail if mDCV chunk doesn't exist, but we call it anyway
    png_get_mDCV_fixed(png_ptr, info_ptr, 
                      &int_white_x, &int_white_y,
                      &int_red_x, &int_red_y,
                      &int_green_x, &int_green_y,
                      &int_blue_x, &int_blue_y,
                      &max_lum_scaled, &min_lum_scaled);
    
    // Try to get animation frame info
    png_uint_32 num_frames = png_get_num_frames(png_ptr, info_ptr);
    if (num_frames > 0) {
        // Get first frame hidden status
        png_byte is_hidden = png_get_first_frame_is_hidden(png_ptr, info_ptr);
        
        // Get next frame delay numerator
        png_uint_16 delay_num = png_get_next_frame_delay_num(png_ptr, info_ptr);
    }
    
    // Get memory pointer
    void *mem_ptr = png_get_mem_ptr(png_ptr);
    
    // Generate benign error (warning)
    png_benign_error(png_ptr, fuzz_str_2);
    
    // Create write structure for output
    png_structp write_ptr = png_create_write_struct(fuzz_str_3, nullptr, nullptr, nullptr);
    if (write_ptr) {
        png_infop write_info = png_create_info_struct(write_ptr);
        if (write_info) {
            if (!setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILENAME_PUSH("output_file")
                FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
                if (out_file) {
                    png_init_io(write_ptr, out_file);
                    
                    // Copy basic info from input
                    int bit_depth, color_type, interlace_type;
                    int compression_type, filter_type;
                    
                    png_get_IHDR(png_ptr, info_ptr, &width, &height,
                                &bit_depth, &color_type, &interlace_type,
                                &compression_type, &filter_type);
                    
                    png_set_IHDR(write_ptr, write_info, width, height,
                                bit_depth, color_type, interlace_type,
                                compression_type, filter_type);
                    
                    // Write info
                    png_write_info(write_ptr, write_info);
                    
                    // Read image data
                    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
                    for (png_uint_32 y = 0; y < height; y++) {
                        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
                    }
                    
                    png_read_image(png_ptr, row_pointers);
                    
                    // Write image data
                    png_write_image(write_ptr, row_pointers);
                    png_write_end(write_ptr, nullptr);
                    
                    // Clean up row pointers
                    for (png_uint_32 y = 0; y < height; y++) {
                        free(row_pointers[y]);
                    }
                    free(row_pointers);
                    
                    assert_file_closed(&out_file);;
                }
            }
            png_destroy_write_struct(&write_ptr, &write_info);
        } else {
            png_destroy_write_struct(&write_ptr, nullptr);
        }
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    assert_file_closed(&in_file);
	return 0;
}