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
//<ID> 241
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 2
//<Quality> {"density":38,"unique_branches":{"png_free":[[236,8,236,23,0,0,4,0]],"png_image_write_main":[[2123,13,2123,69,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_png","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_get_io_chunk_type","png_get_rows","png_get_next_frame_fcTL","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_set_write_user_transform_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_set_IHDR","png_convert_from_struct_tm","png_set_tIME","png_set_first_frame_is_hidden","png_write_info","png_write_image","png_write_end","png_image_begin_read_from_memory","png_image_finish_read","png_image_write_to_stdio","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_read_png","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_get_io_chunk_type","png_get_rows","png_get_next_frame_fcTL","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_write_user_transform_fn","png_init_io","png_set_IHDR","png_convert_from_struct_tm","png_set_tIME","png_set_first_frame_is_hidden","png_write_info","png_write_image","png_write_end","png_image_begin_read_from_memory","png_image_finish_read","png_image_write_to_stdio","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":2}
/*Here's a C++ fuzz driver that reads and writes PNG data using the libpng APIs you specified:

*/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if data is too small to be a valid PNG
    if (size < 8) {
	return 0;
	}
	
    
    // Create read structure
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                      nullptr, nullptr, nullptr);
    if (!png_ptr_read) {
	return 0;
	}
	
    
    // Create info structure for reading
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr_read))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Set up longjmp function
    jmp_buf *jmpbuf = png_set_longjmp_fn(png_ptr_read, longjmp, sizeof(jmp_buf));
    
    // Open input from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for reading
    png_init_io(png_ptr_read, in_file);
    
    // Read PNG info
    png_read_png(png_ptr_read, info_ptr_read, PNG_TRANSFORM_IDENTITY, nullptr);
    
    // Get image properties
    png_uint_32 width = png_get_image_width(png_ptr_read, info_ptr_read);
    png_uint_32 height = png_get_image_height(png_ptr_read, info_ptr_read);
    png_byte bit_depth = png_get_bit_depth(png_ptr_read, info_ptr_read);
    png_byte color_type = png_get_color_type(png_ptr_read, info_ptr_read);
    png_byte interlace_type = png_get_interlace_type(png_ptr_read, info_ptr_read);
    png_byte compression_type = png_get_compression_type(png_ptr_read, info_ptr_read);
    png_byte filter_type = png_get_filter_type(png_ptr_read, info_ptr_read);
    png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr_read);
    
    // Get image rows
    png_bytepp rows = png_get_rows(png_ptr_read, info_ptr_read);
    
    // Try to get APNG frame control (if present)
    png_uint_32 fc_width, fc_height, fc_x_offset, fc_y_offset;
    png_uint_16 delay_num, delay_den;
    png_byte dispose_op, blend_op;
    int has_fctl = png_get_next_frame_fcTL(png_ptr_read, info_ptr_read, 
                                           &fc_width, &fc_height, 
                                           &fc_x_offset, &fc_y_offset,
                                           &delay_num, &delay_den,
                                           &dispose_op, &blend_op);
    
    // Create write structure
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                       nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure for writing
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling for writing
    if (setjmp(png_jmpbuf(png_ptr_write))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set write user transform function (NULL for now)
    png_set_write_user_transform_fn(png_ptr_write, nullptr);
	FUZZ_FILENAME_PUSH("output_file")
    
    // Open output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for writing
    png_init_io(png_ptr_write, out_file);
    
    // Set IHDR for output
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height,
                 bit_depth, color_type, interlace_type,
                 compression_type, filter_type);
    
    // Create and set time information
    png_time pngtime;
    struct tm ttime;
    time_t current_time = time(nullptr);
    localtime_r(&current_time, &ttime);
    png_convert_from_struct_tm(&pngtime, &ttime);
    png_set_tIME(png_ptr_write, info_ptr_write, &pngtime);
    
    // If we have APNG frame control, set first frame hidden flag
    if (has_fctl) {
        png_set_first_frame_is_hidden(png_ptr_write, info_ptr_write, 0);
    }
    
    // Write info
    png_write_info(png_ptr_write, info_ptr_write);
    
    // Write image data
    png_write_image(png_ptr_write, rows);
    
    // Write end
    png_write_end(png_ptr_write, info_ptr_write);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Try to use simplified image API functions
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    // Try to read from memory using simplified API
    int read_result = png_image_begin_read_from_memory(&image, data, size);
    if (read_result) {
        // Allocate buffer for image data
        png_alloc_size_t buffer_size = PNG_IMAGE_SIZE(image);
        void *buffer = malloc(buffer_size);
        if (buffer) {
            // Finish reading
            int finish_result = png_image_finish_read(&image, nullptr, buffer, 0, nullptr);
            
            if (finish_result) {
                // Open another output file for simplified API
                FILE *simplified_out = fopen("output_simplified.png", "wb");
	FUZZ_FILE_PUSH(simplified_out)
                if (simplified_out) {
                    // Write using simplified API
                    png_image_write_to_stdio(&image, simplified_out, 0, buffer, 0, nullptr);
                    assert_file_closed(&simplified_out);;
                }
            }
            
            // Free buffer
            free(buffer);
        }
    }
    
    // Free any allocated memory by png_image
    png_free(nullptr, image.opaque);
    
    // Clean up read structures
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    
    // Clean up write structures
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}