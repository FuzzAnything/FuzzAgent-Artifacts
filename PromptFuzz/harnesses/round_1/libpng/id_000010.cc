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
//<ID> 137
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["png_image_begin_read_from_stdio","png_create_write_struct","png_image_free","png_create_info_struct","png_destroy_write_struct","png_image_free","png_destroy_write_struct","png_image_free","png_set_longjmp_fn","png_destroy_write_struct","png_image_free","png_init_io","png_malloc","png_malloc","png_set_IHDR","png_set_hIST","png_write_info","png_write_image","png_write_end","png_get_compression_buffer_size","png_get_io_chunk_type","png_free","png_free","png_destroy_write_struct","png_image_free"],"critical_calls":["png_image_begin_read_from_stdio","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_malloc","png_malloc","png_set_IHDR","png_set_hIST","png_write_info","png_write_image","png_write_end","png_get_compression_buffer_size","png_get_io_chunk_type","png_free","png_free","png_destroy_write_struct","png_image_free"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<527) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegralArray(uint16_t, fuzz_uint16_t_array_2, fuzz_uint16_t_array_size_2, fdp);
	
		const png_uint_16 * fuzzer_var_png_set_hIST_15_2 = fuzz_uint16_t_array_2;//fuzzer shim end}




    if (size < 50) {
        // Too small for meaningful PNG processing
        return 0;
    }

    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize png_image structure for high-level API
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // Step 1: Begin reading image from input data
    if (png_image_begin_read_from_stdio(&image, in_file) != 1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a write structure for low-level API
    png_structp png_ptr_write = png_create_write_struct(fuzz_str_1, 
                                                        nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        png_image_free(&image);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, nullptr);
        png_image_free(&image);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        png_image_free(&image);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr_write))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&out_file);;
        png_image_free(&image);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize write IO
    png_init_io(png_ptr_write, out_file);

    // Step 2: Create dummy image data for writing
    png_uint_32 width = image.width > 100 ? 100 : image.width;
    png_uint_32 height = image.height > 100 ? 100 : image.height;
    png_byte channels = 4; // RGBA
    size_t row_bytes = width * channels;
    
    // Allocate image buffer
    png_bytep *row_pointers = (png_bytep *)png_malloc(png_ptr_write, 
                                                      sizeof(png_bytep) * height);
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte *)png_malloc(png_ptr_write, row_bytes);
        memset(row_pointers[y], y % 256, row_bytes); // Fill with pattern
    }

    // Set IHDR
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height, 8,
                 PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Step 3: Set histogram (hIST chunk)
    png_uint_16 hist[256] = {0};
    for (int i = 0; i < 256; i++) {
        hist[i] = (png_uint_16)(i * 100);
    }
    png_set_hIST(png_ptr_write, info_ptr_write, fuzzer_var_png_set_hIST_15_2);

    // Write info
    png_write_info(png_ptr_write, info_ptr_write);

    // Step 4: Write the image
    png_write_image(png_ptr_write, row_pointers);
    png_write_end(png_ptr_write, info_ptr_write);

    // Step 5: Get compression buffer size
    size_t comp_buffer_size = png_get_compression_buffer_size(png_ptr_write);

    // Step 6: Get IO chunk type (call after writing)
    png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr_write);

    // Cleanup write structures
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr_write, row_pointers[y]);
    }
    png_free(png_ptr_write, row_pointers);
    
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    // Cleanup read structures
    png_image_free(&image);
    
    // Close files
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;

    // Remove output file to avoid accumulation
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}