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
//<ID> 162
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 0
//<Quality> {"density":32,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_next_frame_delay_den","png_get_uint_31","png_benign_error","png_get_sCAL_fixed","png_save_uint_16","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_read_update_info","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_read_end","png_free","png_free","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_write_struct","png_init_io","png_set_IHDR","png_write_info","png_destroy_write_struct","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_next_frame_delay_den","png_get_uint_31","png_benign_error","png_get_sCAL_fixed","png_save_uint_16","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_read_update_info","png_malloc","png_get_rowbytes","png_malloc","png_read_image","png_read_end","png_free","png_free","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


extern "C" {
}

// Custom error function that will be called by libpng
void  id_000014_custom_benign_error(png_structp png_ptr, png_const_charp warning_message) {
    // We can call png_benign_error from here if needed
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if we don't have enough data for basic PNG operations
    if (size < 100) {
	return 0;
	}
	
    
    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    // Create FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 NULL, NULL, NULL);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up PNG reading from FILE*
    png_init_io(png_ptr, in_file);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // 1. Use png_get_next_frame_delay_den - for APNG frames
    png_uint_16 delay_den = png_get_next_frame_delay_den(png_ptr, info_ptr);
    
    // 2. Use png_get_uint_31 - read a 31-bit unsigned integer from buffer
    // We'll use part of the input data as buffer
    if (size >= 4) {
        png_byte buf[4];
        // Copy 4 bytes from input data for testing
        memcpy(buf, data, 4);
        png_uint_32 uint_31_val = png_get_uint_31(png_ptr, buf);
        
        // Use the value for something (e.g., check if it's reasonable)
        if (uint_31_val > 1000000) {
            // Could trigger some behavior
        }
    }
    
    // 3. Use png_benign_error - issue a benign error/warning
    // We'll create a benign error condition
    if (size < 200) {
        png_benign_error(png_ptr, "Input data is relatively small");
    }
    
    // 4. Use png_get_sCAL_fixed - get physical scale information
    int unit = 0;
    png_fixed_point width_fixed = 0, height_fixed = 0;
    png_uint_32 scal_result = png_get_sCAL_fixed(png_ptr, info_ptr, 
                                               &unit, &width_fixed, &height_fixed);
    
    // 5. Use png_save_uint_16 - save a 16-bit integer to buffer
    png_byte uint16_buf[2];
    // Save the delay denominator we got earlier
    png_save_uint_16(uint16_buf, delay_den);
    
    // Try to read the PNG image data
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Update info based on transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Allocate memory for row pointers
    png_bytep* row_pointers = (png_bytep*)png_malloc(png_ptr, 
                                                    height * sizeof(png_bytep));
    
    // Allocate memory for each row
    png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_ptr, row_bytes);
    }
    
    // Read the image data
    png_read_image(png_ptr, row_pointers);
    
    // Read the end of PNG file
    png_read_end(png_ptr, NULL);
    
    // Clean up row memory
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    // Now create a writing structure
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                   NULL, NULL, NULL);
    if (write_ptr) {
        png_infop write_info = png_create_info_struct(write_ptr);
        if (write_info) {
            if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
                png_destroy_write_struct(&write_ptr, &write_info);
            } else {
                png_init_io(write_ptr, out_file);
                
                // Write header
                png_set_IHDR(write_ptr, write_info, width, height,
                           bit_depth, color_type, PNG_INTERLACE_NONE,
                           PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
                
                png_write_info(write_ptr, write_info);
                
                // For demonstration, we could write a simple image
                // But for fuzzing we'll just clean up
                
                png_destroy_write_struct(&write_ptr, &write_info);
            }
        } else {
            png_destroy_write_struct(&write_ptr, NULL);
        }
    }
    
    // Clean up reading structures
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file we created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}