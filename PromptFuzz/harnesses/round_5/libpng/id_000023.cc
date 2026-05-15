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
//<ID> 238
//<Prompt> []
/*<Combination>: [
*/
//<score> 15.5, nr_unique_branch: 0
//<Quality> {"density":31,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_read_png","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_set_swap_alpha","png_set_tIME","png_init_io","png_set_write_user_transform_fn","png_write_info","png_get_rows","png_write_image","png_write_end","png_get_io_chunk_type","png_get_next_frame_fcTL","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_write_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_read_png","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_set_swap_alpha","png_set_tIME","png_init_io","png_set_write_user_transform_fn","png_write_info","png_get_rows","png_write_image","png_write_end","png_get_io_chunk_type","png_get_next_frame_fcTL","png_destroy_read_struct","png_destroy_write_struct"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Return early if input is too small to be a valid PNG
    if (size < 8) {
	return 0;
	}
	

    // Create FILE pointer for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create FILE pointer for output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Error handling variables
    jmp_buf jmpbuf;
    png_structp png_ptr_r = nullptr;
    png_structp png_ptr_w = nullptr;
    png_infop info_ptr_r = nullptr;
    png_infop info_ptr_w = nullptr;

    // Set up error handling for read struct
    png_ptr_r = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr_r) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling for write struct
    png_ptr_w = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr_w) {
        png_destroy_read_struct(&png_ptr_r, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create info structs
    info_ptr_r = png_create_info_struct(png_ptr_r);
    info_ptr_w = png_create_info_struct(png_ptr_w);
    if (!info_ptr_r || !info_ptr_w) {
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up longjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr_r)) || setjmp(png_jmpbuf(png_ptr_w))) {
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize IO for reading
    png_init_io(png_ptr_r, in_file);

    // Read the PNG file
    png_read_png(png_ptr_r, info_ptr_r, PNG_TRANSFORM_IDENTITY, nullptr);

    // Get image properties for writing
    png_uint_32 width = png_get_image_width(png_ptr_r, info_ptr_r);
    png_uint_32 height = png_get_image_height(png_ptr_r, info_ptr_r);
    png_byte bit_depth = png_get_bit_depth(png_ptr_r, info_ptr_r);
    png_byte color_type = png_get_color_type(png_ptr_r, info_ptr_r);
    png_byte interlace_type = png_get_interlace_type(png_ptr_r, info_ptr_r);
    png_byte compression_type = png_get_compression_type(png_ptr_r, info_ptr_r);
    png_byte filter_type = png_get_filter_type(png_ptr_r, info_ptr_r);

    // Set up IHDR for output
    png_set_IHDR(png_ptr_w, info_ptr_w, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);

    // Set swap alpha for writing
    png_set_swap_alpha(png_ptr_w);

    // Set tIME chunk (dummy time)
    png_time mod_time;
    mod_time.year = 2024;
    mod_time.month = 1;
    mod_time.day = 1;
    mod_time.hour = 0;
    mod_time.minute = 0;
    mod_time.second = 0;
    png_set_tIME(png_ptr_w, info_ptr_w, &mod_time);

    // Initialize IO for writing
    png_init_io(png_ptr_w, out_file);

    // Set write user transform function (null for now)
    png_set_write_user_transform_fn(png_ptr_w, nullptr);

    // Write info
    png_write_info(png_ptr_w, info_ptr_w);

    // Get rows from input image
    png_bytepp rows = png_get_rows(png_ptr_r, info_ptr_r);

    // Write image
    png_write_image(png_ptr_w, rows);

    // Write end
    png_write_end(png_ptr_w, info_ptr_w);

    // Get chunk type (should be 0 after reading complete)
    png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr_r);

    // Get next frame fcTL for APNG (if exists)
    png_uint_32 next_width, next_height, x_offset, y_offset;
    png_uint_16 delay_num, delay_den;
    png_byte dispose_op, blend_op;
    png_get_next_frame_fcTL(png_ptr_r, info_ptr_r, &next_width, &next_height,
                           &x_offset, &y_offset, &delay_num, &delay_den,
                           &dispose_op, &blend_op);

    // Cleanup
    png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
    png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}