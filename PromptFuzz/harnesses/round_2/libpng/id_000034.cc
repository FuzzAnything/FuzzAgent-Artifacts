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
//<ID> 681
//<Prompt> []
/*<Combination>: [
*/
//<score> 204, nr_unique_branch: 19
//<Quality> {"density":34,"unique_branches":{"png_get_sCAL":[[1054,8,1054,23,0,0,4,0],[1054,27,1054,43,0,0,4,0],[1055,8,1055,46,0,0,4,0],[1055,8,1055,46,0,0,4,1]],"png_get_next_frame_fcTL":[[1407,8,1407,23,0,0,4,0],[1407,27,1407,43,0,0,4,0],[1408,8,1408,41,0,0,4,1]],"png_write_IHDR":[[713,13,713,20,0,0,4,0]],"png_get_pixel_aspect_ratio_fixed":[[219,8,219,23,0,0,4,0],[219,27,219,43,0,0,4,0],[220,8,220,46,0,0,4,0],[220,8,220,46,0,0,4,1],[221,8,221,39,0,0,4,0],[221,43,221,74,0,0,4,0],[222,8,222,54,0,0,4,0],[223,8,223,54,0,0,4,0],[230,11,231,56,0,0,4,0]],"png_get_next_frame_height":[[1443,8,1443,23,0,0,4,0],[1443,27,1443,43,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_init_io","png_read_info","png_get_sCAL","png_get_next_frame_fcTL","png_get_next_frame_height","png_get_pixel_aspect_ratio_fixed","png_write_info_before_PLTE","png_write_chunk","png_get_image_height","png_get_rowbytes","png_malloc","png_malloc","png_read_rows","png_get_image_width","png_malloc","png_image_write_to_stdio","png_free","png_free","png_free","png_read_end","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_init_io","png_read_info","png_get_sCAL","png_get_next_frame_fcTL","png_get_next_frame_height","png_get_pixel_aspect_ratio_fixed","png_write_info_before_PLTE","png_write_chunk","png_get_image_height","png_get_rowbytes","png_malloc","png_malloc","png_read_rows","png_get_image_width","png_malloc","png_image_write_to_stdio","png_free","png_free","png_free","png_read_end","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need enough data for PNG header

    // Create input file from memory
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize read structures
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                     nullptr, nullptr, nullptr);
    if (!png_ptr_read) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize write structures
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                       nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr_read)) || setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr_read, in_file);
    png_init_io(png_ptr_write, out_file);

    // Read PNG info
    png_read_info(png_ptr_read, info_ptr_read);

    // Get sCAL information
    int unit_sCAL;
    double width_sCAL, height_sCAL;
    png_get_sCAL(png_ptr_read, info_ptr_read, &unit_sCAL, &width_sCAL, &height_sCAL);

    // Get next frame fcTL (for APNG)
    png_uint_32 next_width, next_height, x_offset, y_offset;
    png_uint_16 delay_num, delay_den;
    png_byte dispose_op, blend_op;
    png_get_next_frame_fcTL(png_ptr_read, info_ptr_write, 
                           &next_width, &next_height, &x_offset, &y_offset,
                           &delay_num, &delay_den, &dispose_op, &blend_op);

    // Get next frame height
    png_uint_32 next_frame_height = png_get_next_frame_height(png_ptr_read, info_ptr_read);

    // Get pixel aspect ratio (fixed point)
    png_fixed_point pixel_aspect_ratio_fixed = 
        png_get_pixel_aspect_ratio_fixed(png_ptr_read, info_ptr_read);

    // Set up write info
    png_write_info_before_PLTE(png_ptr_write, info_ptr_write);

    // Write a custom chunk (example: tEXt chunk)
    const png_byte chunk_name[] = {116, 69, 88, 116}; // "tEXt" in ASCII
    const char* chunk_data = "FuzzerTest:Generated by libpng fuzzer";
    png_write_chunk(png_ptr_write, chunk_name, 
                    (const png_byte*)chunk_data, strlen(chunk_data) + 1);

    // Prepare for reading rows
    png_uint_32 height = png_get_image_height(png_ptr_read, info_ptr_read);
    png_uint_32 rowbytes = png_get_rowbytes(png_ptr_read, info_ptr_read);
    
    png_byte** row_pointers = (png_byte**)png_malloc(png_ptr_read, 
                                                    sizeof(png_byte*) * height);
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_ptr_read, rowbytes);
    }

    // Read image rows
    png_read_rows(png_ptr_read, row_pointers, nullptr, height);

    // Setup png_image for high-level write
    png_image image_write;
    memset(&image_write, 0, sizeof(image_write));
    image_write.version = PNG_IMAGE_VERSION;
    image_write.width = png_get_image_width(png_ptr_read, info_ptr_read);
    image_write.height = height;
    image_write.format = PNG_FORMAT_RGBA;

    // Write image using high-level API
    int convert_to_8_bit = 0;
    png_int_32 row_stride = rowbytes;
    
    // Prepare buffer for writing (using the read data)
    void* write_buffer = png_malloc(png_ptr_read, PNG_IMAGE_SIZE(image_write));
    if (write_buffer) {
        // Copy row data to contiguous buffer
        png_byte* dst = (png_byte*)write_buffer;
        for (png_uint_32 y = 0; y < height; y++) {
            memcpy(dst, row_pointers[y], rowbytes);
            dst += rowbytes;
        }
        
        png_image_write_to_stdio(&image_write, out_file, convert_to_8_bit, 
                                write_buffer, row_stride, nullptr);
        
        png_free(png_ptr_read, write_buffer);
    }

    // Cleanup
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr_read, row_pointers[y]);
    }
    png_free(png_ptr_read, row_pointers);

    // Close files and clean up
    png_read_end(png_ptr_read, info_ptr_read);
    png_write_end(png_ptr_write, info_ptr_write);
    
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}