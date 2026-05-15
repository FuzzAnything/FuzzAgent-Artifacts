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
//<ID> 1075
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 1
//<Quality> {"density":42,"unique_branches":{"png_destroy_write_struct":[[1055,11,1055,26,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_free","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_rgb_to_gray","png_get_next_frame_blend_op","png_get_uint_31","png_set_gamma_fixed","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_channels","png_get_rowbytes","png_malloc","png_malloc","png_read_image","png_create_write_struct","png_free","png_destroy_read_struct","png_set_longjmp_fn","png_free","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_get_chunk_malloc_max","png_write_image","png_write_end","png_write_flush","png_image_free","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_read_info","png_set_rgb_to_gray","png_get_next_frame_blend_op","png_get_uint_31","png_set_gamma_fixed","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_channels","png_get_rowbytes","png_malloc","png_malloc","png_read_image","png_create_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_get_chunk_malloc_max","png_write_image","png_write_end","png_write_flush","png_image_free","png_free","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":1}
/**/


// Include necessary headers for libpng

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create FILE pointers for input and output
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    jmp_buf jump_buffer;
    png_structp read_ptr = nullptr;
    png_structp write_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_infop end_info = nullptr;
    png_bytep *row_pointers = nullptr;

    // Initialize libpng structures
    png_const_charp user_png_ver = PNG_LIBPNG_VER_STRING;
    
    // Create read structure
    read_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create info structure
    info_ptr = png_create_info_struct(read_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create end info structure
    end_info = png_create_info_struct(read_ptr);
    if (!end_info) {
        png_destroy_read_struct(&read_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling with longjmp
    if (setjmp(png_jmpbuf(read_ptr))) {
	FUZZ_FILE_CLOSE();
        png_free(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &info_ptr, &end_info);
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set longjmp function for read structure
    png_set_longjmp_fn(read_ptr, longjmp, sizeof(jmp_buf));

    // Initialize IO for reading
    png_init_io(read_ptr, in_file);

    // Read PNG info
    png_read_info(read_ptr, info_ptr);

    // Set RGB to grayscale conversion
    png_set_rgb_to_gray(read_ptr, fuzz_int32_t_2, 0.299, 0.587);

    // Get frame blend op (for APNG)
    png_byte blend_op = png_get_next_frame_blend_op(read_ptr, info_ptr);

    // Read and parse a 31-bit unsigned integer from buffer
    // Using first 4 bytes of data if available
    if (size >= 4) {
        png_uint_32 uint31_val = png_get_uint_31(read_ptr, data);
        (void)uint31_val; // Use the value to avoid compiler warning
    }

    // Set gamma correction
    png_set_gamma_fixed(read_ptr, fuzz_int32_t_3, fuzz_int32_t_4); // 2.2 gamma * 100000

    // Update info after transformations
    png_read_update_info(read_ptr, info_ptr);

    // Get image dimensions and properties
    png_uint_32 width = png_get_image_width(read_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(read_ptr, info_ptr);
    png_byte color_type = png_get_color_type(read_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(read_ptr, info_ptr);
    png_byte channels = png_get_channels(read_ptr, info_ptr);

    // Calculate row bytes
    size_t rowbytes = png_get_rowbytes(read_ptr, info_ptr);

    // Allocate memory for row pointers
    row_pointers = (png_bytep*)png_malloc(read_ptr, height * sizeof(png_bytep));

    // Allocate memory for each row
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(read_ptr, rowbytes);
    }

    // Read the image
    png_read_image(read_ptr, row_pointers);

    // Create write structure
    write_ptr = png_create_write_struct(fuzz_str_5, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_free(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_free(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &info_ptr, &end_info);
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set longjmp function for write structure
    png_set_longjmp_fn(write_ptr, longjmp, sizeof(jmp_buf));

    // Initialize IO for writing
    png_init_io(write_ptr, out_file);

    // Set IHDR for output image
    int interlace_method = PNG_INTERLACE_NONE;
    int compression_method = PNG_COMPRESSION_TYPE_DEFAULT;
    int filter_method = PNG_FILTER_TYPE_DEFAULT;
    
    // Convert to grayscale if needed
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
        color_type = PNG_COLOR_TYPE_GRAY;
        if (bit_depth < 8) {
            bit_depth = 8;
        }
    }
    
    png_set_IHDR(write_ptr, info_ptr, width, height, bit_depth, 
                 color_type, fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8);

    // Write info
    png_write_info(write_ptr, info_ptr);

    // Get chunk malloc max
    png_alloc_size_t chunk_malloc_max = png_get_chunk_malloc_max(read_ptr);
    (void)chunk_malloc_max;

    // Write image
    png_write_image(write_ptr, row_pointers);

    // Write end
    png_write_end(write_ptr, nullptr);

    // Write flush
    png_write_flush(write_ptr);

    // Create and free a png_image structure (even though not used)
    png_image image_struct = {0};
    png_image_free(&image_struct);

    // Cleanup
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(read_ptr, row_pointers[y]);
    }
    png_free(read_ptr, row_pointers);
    
    png_destroy_read_struct(&read_ptr, &info_ptr, &end_info);
    png_destroy_write_struct(&write_ptr, nullptr);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}