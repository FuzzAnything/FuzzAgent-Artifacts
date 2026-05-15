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
//<ID> 348
//<Prompt> []
/*<Combination>: [
*/
//<score> 84, nr_unique_branch: 1
//<Quality> {"density":42,"unique_branches":{"png_get_valid":[[29,11,29,32,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_init_io","png_set_write_status_fn","png_set_read_user_transform_fn","png_read_info","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_valid","png_destroy_write_struct","png_destroy_read_struct","png_set_rgb_to_gray","png_set_check_for_invalid_index","png_set_compression_mem_level","png_set_gAMA_fixed","png_init_io","png_set_IHDR","png_write_info","png_get_rowbytes","png_malloc","png_destroy_write_struct","png_destroy_read_struct","png_malloc","png_read_row","png_set_rows","png_write_image","png_write_end","png_free","png_free","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_set_write_status_fn","png_set_read_user_transform_fn","png_read_info","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_valid","png_set_rgb_to_gray","png_set_check_for_invalid_index","png_set_compression_mem_level","png_set_gAMA_fixed","png_init_io","png_set_IHDR","png_write_info","png_get_rowbytes","png_malloc","png_malloc","png_read_row","png_set_rows","png_write_image","png_write_end","png_free","png_free","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE pointer for output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create read structure
    png_struct *read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                   nullptr, nullptr, nullptr);
    if (!read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create read info structure
    png_info *read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create write structure
    png_struct *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                    nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create write info structure
    png_info *write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling with longjmp
    if (setjmp(png_jmpbuf(read_ptr)) || setjmp(png_jmpbuf(write_ptr))) {
        // Error occurred during PNG processing
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set longjmp function for read structure
    png_set_longjmp_fn(read_ptr, longjmp, sizeof(jmp_buf));

    // Initialize I/O for reading
    png_init_io(read_ptr, in_file);

    // Set write status function (dummy)
    png_set_write_status_fn(write_ptr, [](png_structp, png_uint_32, int) {});

    // Set read user transform function (dummy)
    png_set_read_user_transform_fn(read_ptr, [](png_structp, png_row_infop, png_bytep) {});

    // Read PNG info
    png_read_info(read_ptr, read_info);

    // Get image properties
    png_uint_32 width = png_get_image_width(read_ptr, read_info);
    png_uint_32 height = png_get_image_height(read_ptr, read_info);
    png_byte bit_depth = png_get_bit_depth(read_ptr, read_info);
    png_byte color_type = png_get_color_type(read_ptr, read_info);

    // Check for valid PNG data
    png_uint_32 valid = png_get_valid(read_ptr, read_info, PNG_INFO_IDAT);
    if (!valid) {
        // No image data found
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set RGB to gray conversion parameters
    png_set_rgb_to_gray(read_ptr, 1, 0.2126, 0.7152);

    // Set check for invalid index
    png_set_check_for_invalid_index(read_ptr, 1);

    // Set compression memory level for writing
    png_set_compression_mem_level(write_ptr, 8);

    // Set gamma for writing (using fixed point)
    png_set_gAMA_fixed(write_ptr, write_info, 45445); // 1.0 gamma

    // Initialize I/O for writing
    png_init_io(write_ptr, out_file);

    // Set IHDR for output image
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, 
                 color_type, PNG_INTERLACE_NONE, 
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Write PNG info
    png_write_info(write_ptr, write_info);

    // Get row bytes
    size_t rowbytes = png_get_rowbytes(read_ptr, read_info);

    // Allocate memory for row pointers
    png_byte **row_pointers = (png_byte **)png_malloc(read_ptr, height * sizeof(png_byte *));
    if (!row_pointers) {
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Read image data row by row
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte *)png_malloc(read_ptr, rowbytes);
        png_read_row(read_ptr, row_pointers[y], nullptr);
    }

    // Set rows for writing
    png_set_rows(write_ptr, write_info, row_pointers);

    // Write image
    png_write_image(write_ptr, row_pointers);

    // Write end
    png_write_end(write_ptr, write_info);

    // Clean up row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(read_ptr, row_pointers[y]);
    }
    png_free(read_ptr, row_pointers);

    // Clean up
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}