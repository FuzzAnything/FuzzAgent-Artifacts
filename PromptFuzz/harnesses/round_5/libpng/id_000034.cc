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
//<ID> 473
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 0
//<Quality> {"density":48,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_sig_cmp","png_destroy_read_struct","png_set_sig_bytes","png_read_png","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_get_io_chunk_type","png_get_rows","png_get_next_frame_fcTL","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_set_write_user_transform_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_set_IHDR","png_convert_from_struct_tm","png_set_tIME","png_set_first_frame_is_hidden","png_write_info","png_write_image","png_write_end","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_calloc","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_sig_cmp","png_set_sig_bytes","png_read_png","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_get_io_chunk_type","png_get_rows","png_get_next_frame_fcTL","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_write_user_transform_fn","png_init_io","png_set_IHDR","png_convert_from_struct_tm","png_set_tIME","png_set_first_frame_is_hidden","png_write_info","png_write_image","png_write_end","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_calloc","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":1}
/*Looking at the provided APIs, I can see they cover both reading and writing PNG files, including APNG (Animated PNG) functionality. Here's a fuzz driver that reads a PNG from input data, extracts its metadata, then writes a modified version to output:

*/


extern "C" {
}

// Error handling function
static void  id_000034_png_error_handler(png_struct *png_ptr, const char *message) {
    // Do nothing in fuzzing
}

// Warning handling function  
static void  id_000034_png_warning_handler(png_struct *png_ptr, const char *message) {
    // Do nothing in fuzzing
}

// User transform function (dummy)
static void  id_000034_user_transform_fn(png_struct *png_ptr, png_row_info *row_info, png_byte *data) {
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
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
    
    // Create read structure
    png_struct *png_read_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
         id_000034_png_error_handler, 
         id_000034_png_warning_handler
    );
    if (!png_read_ptr) {
	return 0;
	}
	
    
    // Create info structure for reading
    png_info *info_read_ptr = png_create_info_struct(png_read_ptr);
    if (!info_read_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Create end info structure
    png_info *end_info_ptr = png_create_info_struct(png_read_ptr);
    if (!end_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        return 0;
    }
    
    // Set up longjmp for error handling
    jmp_buf *jmpbuf = png_set_longjmp_fn(png_read_ptr, longjmp, sizeof(jmp_buf));
    if (setjmp(*jmpbuf)) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, &end_info_ptr);
        return 0;
    }
    
    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, &end_info_ptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_read_ptr, in_file);
    
    // Check PNG signature
    png_byte sig[8];
    if (fread(sig, 1, 8, in_file) != 8 || png_sig_cmp(sig, 0, 8)) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, &end_info_ptr);
        assert_file_closed(&in_file);
	return 0;
    }
    png_set_sig_bytes(png_read_ptr, 8);
    
    // Read the PNG
    png_read_png(png_read_ptr, info_read_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
    
    // Get image properties
    png_uint_32 width = png_get_image_width(png_read_ptr, info_read_ptr);
    png_uint_32 height = png_get_image_height(png_read_ptr, info_read_ptr);
    png_byte bit_depth = png_get_bit_depth(png_read_ptr, info_read_ptr);
    png_byte color_type = png_get_color_type(png_read_ptr, info_read_ptr);
    png_byte interlace_type = png_get_interlace_type(png_read_ptr, info_read_ptr);
    png_byte compression_type = png_get_compression_type(png_read_ptr, info_read_ptr);
    png_byte filter_type = png_get_filter_type(png_read_ptr, info_read_ptr);
    
    // Get chunk type
    png_uint_32 chunk_type = png_get_io_chunk_type(png_read_ptr);
    
    // Get rows
    png_byte **rows = png_get_rows(png_read_ptr, info_read_ptr);
    
    // Try to get APNG frame control (if APNG)
    png_uint_32 fc_width = 0, fc_height = 0, fc_x_offset = 0, fc_y_offset = 0;
    png_uint_16 delay_num = 0, delay_den = 0;
    png_byte dispose_op = 0, blend_op = 0;
    png_get_next_frame_fcTL(png_read_ptr, info_read_ptr, &fc_width, &fc_height, 
                           &fc_x_offset, &fc_y_offset, &delay_num, &delay_den, 
                           &dispose_op, &blend_op);
    
    // Close input file
    assert_file_closed(&in_file);;
    
    // Now create write structures
    png_struct *png_write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
         id_000034_png_error_handler,
         id_000034_png_warning_handler
    );
    if (!png_write_ptr) {
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, &end_info_ptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *info_write_ptr = png_create_info_struct(png_write_ptr);
    if (!info_write_ptr) {
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, &end_info_ptr);
        png_destroy_write_struct(&png_write_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up longjmp for write structure
    jmp_buf *write_jmpbuf = png_set_longjmp_fn(png_write_ptr, longjmp, sizeof(jmp_buf));
    if (setjmp(*write_jmpbuf)) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, &end_info_ptr);
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set user transform function
    png_set_write_user_transform_fn(png_write_ptr,  id_000034_user_transform_fn);
	FUZZ_FILENAME_PUSH("output_file")
    
    // Open output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, &end_info_ptr);
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for writing
    png_init_io(png_write_ptr, out_file);
    
    // Set IHDR for output
    png_set_IHDR(png_write_ptr, info_write_ptr, width, height, bit_depth,
                color_type, interlace_type, compression_type, filter_type);
    
    // Create and set modification time
    png_time mod_time;
    struct tm ttime;
    time_t now = time(nullptr);
    localtime_r(&now, &ttime);
    png_convert_from_struct_tm(&mod_time, &ttime);
    png_set_tIME(png_write_ptr, info_write_ptr, &mod_time);
    
    // Set first frame hidden flag (for APNG)
    png_set_first_frame_is_hidden(png_write_ptr, info_write_ptr, 0);
    
    // Write info
    png_write_info(png_write_ptr, info_write_ptr);
    
    // Write image rows if we have them
    if (rows) {
        png_write_image(png_write_ptr, rows);
    }
    
    // Write end
    png_write_end(png_write_ptr, info_write_ptr);
    
    // Close output file
    assert_file_closed(&out_file);;
    
    // Get color type again (demonstrating API usage)
    png_byte color_type2 = png_get_color_type(png_read_ptr, info_read_ptr);
    png_byte interlace_type2 = png_get_interlace_type(png_read_ptr, info_read_ptr);
    png_byte compression_type2 = png_get_compression_type(png_read_ptr, info_read_ptr);
    
    // Allocate and free some memory using png_free
    png_byte *temp_buffer = (png_byte*)png_calloc(png_read_ptr, 1024);
    if (temp_buffer) {
        png_free(png_read_ptr, temp_buffer);
    }
    
    // Clean up
    png_destroy_read_struct(&png_read_ptr, &info_read_ptr, &end_info_ptr);
    png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}