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
//<ID> 353
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 9
//<Quality> {"density":6,"unique_branches":{"png_set_scale_16":[[175,8,175,37,0,0,4,1]],"png_set_text_compression_mem_level":[[1298,8,1298,23,0,0,4,1]],"png_set_user_limits":[[1969,8,1969,23,0,0,4,1]],"png_get_next_frame_blend_op":[[1503,8,1503,23,0,0,4,0],[1503,27,1503,43,0,0,4,0]],"png_write_flush":[[982,8,982,23,0,0,4,1],[986,8,986,48,0,0,4,0]],"png_get_compression_buffer_size":[[1281,8,1281,23,0,0,4,1],[1285,8,1285,49,0,0,4,0]]},"library_calls":["png_image_begin_read_from_stdio","png_create_read_struct","png_set_user_limits","png_set_scale_16","png_set_text_compression_mem_level","png_get_compression_buffer_size","png_create_write_struct","png_init_io","png_create_info_struct","png_get_next_frame_blend_op","png_write_flush","png_destroy_write_struct","png_image_free","png_create_info_struct","png_destroy_read_struct"],"critical_calls":["png_image_begin_read_from_stdio","png_create_read_struct","png_set_user_limits","png_set_scale_16","png_set_text_compression_mem_level","png_get_compression_buffer_size","png_create_write_struct","png_init_io","png_create_info_struct","png_get_next_frame_blend_op","png_write_flush","png_destroy_write_struct","png_image_free","png_create_info_struct","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // API 1: Begin reading PNG from stdio
    if (png_image_begin_read_from_stdio(&image, in_file) == 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // API 2: Set user limits
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, nullptr, nullptr);
    if (png_ptr) {
        png_set_user_limits(png_ptr, 8000, 8000);
    }

    // API 3: Set scale 16
    if (png_ptr) {
        png_set_scale_16(png_ptr);
    }

    // API 4: Set text compression memory level
    if (png_ptr) {
        png_set_text_compression_mem_level(png_ptr, 8);
    }

    // API 5: Get compression buffer size
    size_t comp_buf_size = 0;
    if (png_ptr) {
        comp_buf_size = png_get_compression_buffer_size(png_ptr);
    }

    // Create write structure for output
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                   nullptr, nullptr, nullptr);
    if (write_ptr && out_file) {
        png_init_io(write_ptr, out_file);
        
        // API 6: Get next frame blend op (for APNG support)
        png_infop info_ptr = png_create_info_struct(write_ptr);
        if (info_ptr) {
            png_byte blend_op = png_get_next_frame_blend_op(write_ptr, info_ptr);
            (void)blend_op; // Prevent unused variable warning
        }
        
        // API 7: Write flush
        png_write_flush(write_ptr);
        
        png_destroy_write_struct(&write_ptr, &info_ptr);
    }

    // API 8: Free the image
    png_image_free(&image);

    // Cleanup
    if (png_ptr) {
        png_infop info_ptr = png_create_info_struct(png_ptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}