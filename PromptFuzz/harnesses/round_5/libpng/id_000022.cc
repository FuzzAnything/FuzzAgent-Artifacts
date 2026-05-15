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
//<ID> 237
//<Prompt> []
/*<Combination>: [
*/
//<score> 55.5, nr_unique_branch: 17
//<Quality> {"density":37,"unique_branches":{"png_do_read_transformations":[[5096,8,5096,56,0,0,4,0]],"png_do_read_swap_alpha":[[2607,8,2607,56,0,0,4,0],[2607,8,2607,56,0,0,4,1],[2610,11,2610,35,0,0,4,0],[2610,11,2610,35,0,0,4,1],[2617,22,2617,35,0,0,4,0],[2617,22,2617,35,0,0,4,1],[2636,22,2636,35,0,0,4,0],[2636,22,2636,35,0,0,4,1],[2653,13,2653,62,0,0,4,0],[2653,13,2653,62,0,0,4,1],[2656,11,2656,35,0,0,4,0],[2656,11,2656,35,0,0,4,1],[2663,22,2663,35,0,0,4,0],[2663,22,2663,35,0,0,4,1],[2680,22,2680,35,0,0,4,0],[2680,22,2680,35,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_swap_alpha","png_set_progressive_frame_fn","png_read_png","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_get_io_chunk_type","png_get_rows","png_get_next_frame_fcTL","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_set_write_user_transform_fn","png_set_IHDR","png_convert_from_struct_tm","png_set_tIME","png_write_info","png_write_image","png_write_end","png_set_first_frame_is_hidden","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_swap_alpha","png_set_progressive_frame_fn","png_read_png","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_get_io_chunk_type","png_get_rows","png_get_next_frame_fcTL","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_set_write_user_transform_fn","png_set_IHDR","png_convert_from_struct_tm","png_set_tIME","png_write_info","png_write_image","png_write_end","png_set_first_frame_is_hidden","png_destroy_read_struct","png_destroy_write_struct"],"visited":1}
/**/


// Custom read function for memory buffer
void  id_000022_user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    FILE* fp = (FILE*)io_ptr;
    fread(data, 1, length, fp);
}

// Custom write function for memory buffer (not used in this fuzzer but required for structure)
void  id_000022_user_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    FILE* fp = (FILE*)io_ptr;
    fwrite(data, 1, length, fp);
}

// Custom flush function (not used in this fuzzer but required for structure)
void  id_000022_user_flush_data(png_structp png_ptr) {
    // Do nothing for memory buffer
}

// User transform function for writing
void  id_000022_user_transform_fn(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    // Simple transform: invert all bytes
    for (png_size_t i = 0; i < row_info->rowbytes; i++) {
        data[i] = ~data[i];
    }
}

// Frame info callback for progressive reading
void  id_000022_frame_info_fn(png_structp png_ptr, png_uint_32 frame_number) {
    // Called when frame info is available
}

// Frame end callback for progressive reading
void  id_000022_frame_end_fn(png_structp png_ptr, png_uint_32 frame_number) {
    // Called when frame ends
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for PNG operations
    
    // Create input file from memory buffer
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    jmp_buf jmpbuf;
    png_structp png_ptr_read = NULL;
    png_structp png_ptr_write = NULL;
    png_infop info_ptr_read = NULL;
    png_infop info_ptr_write = NULL;
    
    // Create read structure
    png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                         NULL, NULL, NULL);
    if (!png_ptr_read) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure for reading
    info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up longjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr_read))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for reading
    png_init_io(png_ptr_read, in_file);
    
    // Set swap alpha
    png_set_swap_alpha(png_ptr_read);
    
    // Set progressive frame functions
    png_set_progressive_frame_fn(png_ptr_read,  id_000022_frame_info_fn,  id_000022_frame_end_fn);
    
    // Read the PNG with all transforms
    png_read_png(png_ptr_read, info_ptr_read, PNG_TRANSFORM_IDENTITY, NULL);
    
    // Get various image properties
    png_uint_32 width = png_get_image_width(png_ptr_read, info_ptr_read);
    png_uint_32 height = png_get_image_height(png_ptr_read, info_ptr_read);
    png_byte bit_depth = png_get_bit_depth(png_ptr_read, info_ptr_read);
    png_byte color_type = png_get_color_type(png_ptr_read, info_ptr_read);
    png_byte interlace_type = png_get_interlace_type(png_ptr_read, info_ptr_read);
    png_byte compression_type = png_get_compression_type(png_ptr_read, info_ptr_read);
    png_byte filter_type = png_get_filter_type(png_ptr_read, info_ptr_read);
    
    // Get chunk type
    png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr_read);
    
    // Get rows
    png_bytepp rows = png_get_rows(png_ptr_read, info_ptr_read);
    
    // Try to get next frame fcTL (for APNG)
    png_uint_32 next_width, next_height, x_offset, y_offset;
    png_uint_16 delay_num, delay_den;
    png_byte dispose_op, blend_op;
    png_get_next_frame_fcTL(png_ptr_read, info_ptr_read, 
                           &next_width, &next_height, &x_offset, &y_offset,
                           &delay_num, &delay_den, &dispose_op, &blend_op);
    
    // Create write structure
    png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                           NULL, NULL, NULL);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure for writing
    info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
        png_destroy_write_struct(&png_ptr_write, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up longjmp for write structure
    png_set_longjmp_fn(png_ptr_write, longjmp, sizeof(jmp_buf));
    
    if (setjmp(png_jmpbuf(png_ptr_write))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for writing
    png_init_io(png_ptr_write, out_file);
    
    // Set write user transform function
    png_set_write_user_transform_fn(png_ptr_write,  id_000022_user_transform_fn);
    
    // Set IHDR for output
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height,
                 bit_depth, color_type, interlace_type,
                 compression_type, filter_type);
    
    // Set tIME (using current time as dummy)
    png_time mod_time;
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    png_convert_from_struct_tm(&mod_time, t);
    png_set_tIME(png_ptr_write, info_ptr_write, &mod_time);
    
    // Write info
    png_write_info(png_ptr_write, info_ptr_write);
    
    // Write image rows
    png_write_image(png_ptr_write, rows);
    
    // Write end
    png_write_end(png_ptr_write, info_ptr_write);
    
    // Set first frame as hidden (for APNG)
    png_set_first_frame_is_hidden(png_ptr_write, info_ptr_write, 0);
    
    // Clean up
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, NULL);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}