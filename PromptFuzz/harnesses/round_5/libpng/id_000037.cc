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
//<ID> 492
//<Prompt> []
/*<Combination>: [
*/
//<score> 126, nr_unique_branch: 3
//<Quality> {"density":42,"unique_branches":{"png_handle_PLTE":[[981,13,981,49,0,0,4,0]],"png_read_info":[[133,19,133,56,0,0,4,0],[139,16,139,52,0,0,4,0]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_set_sig_bytes","png_read_info","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_get_io_chunk_type","png_get_next_frame_fcTL","png_read_png","png_get_rows","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_set_write_user_transform_fn","png_set_IHDR","png_write_info","png_write_image","png_write_end","png_calloc","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_get_io_chunk_type","png_get_next_frame_fcTL","png_read_png","png_get_rows","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_set_write_user_transform_fn","png_set_IHDR","png_write_info","png_write_image","png_write_end","png_calloc","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


// Error handler callback
static void  id_000037_png_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    longjmp(png_jmpbuf(png_ptr), 1);
}

// Warning handler callback
static void  id_000037_png_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - just suppress warnings
}

// User transform function for writing
static void  id_000037_write_user_transform_fn(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    // Simple identity transform
}

// Longjmp function wrapper
static void  id_000037_png_longjmp_wrapper(jmp_buf *env, int value) {
    longjmp(*env, value);
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // Need at least PNG signature
    
    FILE *in_file = NULL;
    FILE *out_file = NULL;
    png_structp read_ptr = NULL;
    png_structp write_ptr = NULL;
    png_infop read_info = NULL;
    png_infop write_info = NULL;
    png_bytepp rows = NULL;
    png_bytep buffer = NULL;
    
    // Create memory file for input
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Check PNG signature
    png_byte sig[8];
    if (fread(sig, 1, 8, in_file) != 8) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    if (png_sig_cmp(sig, 0, 8) != 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create read structure
    read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, 
                                      id_000037_png_error_handler,  id_000037_png_warning_handler);
    if (!read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure for reading
    read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up longjmp for read struct
    jmp_buf *read_jmpbuf = png_set_longjmp_fn(read_ptr, 
        (png_longjmp_ptr) id_000037_png_longjmp_wrapper, sizeof(jmp_buf));
    if (!read_jmpbuf) {
        png_destroy_read_struct(&read_ptr, &read_info, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set error handling
    if (setjmp(png_jmpbuf(read_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&read_ptr, &read_info, NULL);
        assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        if (write_ptr) png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for reading
    png_init_io(read_ptr, in_file);
    
    // Set signature bytes we already read
    png_set_sig_bytes(read_ptr, 8);
    
    // Read PNG info
    png_read_info(read_ptr, read_info);
    
    // Get image properties
    png_uint_32 width = png_get_image_width(read_ptr, read_info);
    png_uint_32 height = png_get_image_height(read_ptr, read_info);
    png_byte bit_depth = png_get_bit_depth(read_ptr, read_info);
    png_byte color_type = png_get_color_type(read_ptr, read_info);
    png_byte interlace_type = png_get_interlace_type(read_ptr, read_info);
    png_byte compression_type = png_get_compression_type(read_ptr, read_info);
    png_byte filter_type = png_get_filter_type(read_ptr, read_info);
    
    // Get chunk type
    png_uint_32 chunk_type = png_get_io_chunk_type(read_ptr);
    
    // Try to get APNG frame control (may fail for non-APNG)
    png_uint_32 fctl_width, fctl_height, fctl_x_offset, fctl_y_offset;
    png_uint_16 delay_num, delay_den;
    png_byte dispose_op, blend_op;
    int has_fctl = png_get_next_frame_fcTL(read_ptr, read_info, 
        &fctl_width, &fctl_height, &fctl_x_offset, &fctl_y_offset,
        &delay_num, &delay_den, &dispose_op, &blend_op);
    
    // Read the entire PNG image
    png_read_png(read_ptr, read_info, PNG_TRANSFORM_IDENTITY, NULL);
    
    // Get rows
    rows = png_get_rows(read_ptr, read_info);
    
    // Create write structure
    write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL,
                                        id_000037_png_error_handler,  id_000037_png_warning_handler);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure for writing
    write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_read_struct(&read_ptr, &read_info, NULL);
        png_destroy_write_struct(&write_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up longjmp for write struct
    jmp_buf *write_jmpbuf = png_set_longjmp_fn(write_ptr,
        (png_longjmp_ptr) id_000037_png_longjmp_wrapper, sizeof(jmp_buf));
    if (!write_jmpbuf) {
        png_destroy_read_struct(&read_ptr, &read_info, NULL);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set error handling for write
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&read_ptr, &read_info, NULL);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        png_destroy_read_struct(&read_ptr, &read_info, NULL);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for writing
    png_init_io(write_ptr, out_file);
    
    // Set write user transform function
    png_set_write_user_transform_fn(write_ptr,  id_000037_write_user_transform_fn);
    
    // Set IHDR for output
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth,
                 color_type, interlace_type, compression_type, filter_type);
    
    // Write info
    png_write_info(write_ptr, write_info);
    
    // Write image
    png_write_image(write_ptr, rows);
    
    // Write end
    png_write_end(write_ptr, write_info);
    
    // Allocate and free memory using png_calloc and png_free
    buffer = (png_bytep)png_calloc(read_ptr, 64);
    if (buffer) {
        memset(buffer, 0xAA, 64); // Use the buffer
        png_free(read_ptr, buffer);
        buffer = NULL;
    }
    
    // Clean up
    png_destroy_read_struct(&read_ptr, &read_info, NULL);
    png_destroy_write_struct(&write_ptr, &write_info);
    
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}