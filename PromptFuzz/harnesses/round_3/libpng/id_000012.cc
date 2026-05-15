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
//<ID> 135
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 10
//<Quality> {"density":24,"unique_branches":{"png_free":[[240,8,240,32,0,0,4,0]],"png_set_compression_buffer_size":[[1892,8,1892,23,0,0,4,1],[1895,8,1895,17,0,0,4,1],[1895,21,1895,43,0,0,4,1],[1899,8,1899,49,0,0,4,1],[1907,8,1907,49,0,0,4,0],[1909,11,1909,31,0,0,4,1],[1921,11,1921,29,0,0,4,1],[1929,11,1929,19,0,0,4,1],[1940,11,1940,40,0,0,4,1]]},"library_calls":["png_image_begin_read_from_stdio","png_create_write_struct","png_image_free","png_create_info_struct","png_destroy_write_struct","png_image_free","png_set_longjmp_fn","png_destroy_write_struct","png_image_free","png_init_io","png_set_mem_fn","png_set_swap","png_set_compression_buffer_size","png_set_IHDR","png_set_sRGB","png_write_info","png_destroy_write_struct","png_image_free","png_image_finish_read","png_destroy_write_struct","png_image_free","png_write_image","png_write_end","png_destroy_write_struct","png_image_free"],"critical_calls":["png_image_begin_read_from_stdio","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_mem_fn","png_set_swap","png_set_compression_buffer_size","png_set_IHDR","png_set_sRGB","png_write_info","png_image_finish_read","png_write_image","png_write_end","png_destroy_write_struct","png_image_free"],"visited":2}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to read and process PNG data from a byte stream:

*/


// Custom memory functions
static void*  id_000012_custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
    return malloc(size);
}

static void  id_000012_custom_free(png_structp png_ptr, void* ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* from the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create output file for writing
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize png_image for reading
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // 1. Use png_image_begin_read_from_stdio to start reading PNG from stream
    int read_result = png_image_begin_read_from_stdio(&image, in_file);
    
    // Close the input file since we've consumed the data
    assert_file_closed(&in_file);;
    
    if (!read_result) {
        // Reading failed, clean up and exit
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create png_struct for writing
    png_structp png_ptr = png_create_write_struct(fuzz_str_1, 
                                                   nullptr, nullptr, nullptr);
    if (!png_ptr) {
        png_image_free(&image);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create png_info for writing
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        png_image_free(&image);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_ptr, &info_ptr);
        png_image_free(&image);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O for writing
    png_init_io(png_ptr, out_file);

    // 2. Use png_set_mem_fn to set custom memory functions
    png_set_mem_fn(png_ptr, nullptr,  id_000012_custom_malloc,  id_000012_custom_free);

    // 3. Use png_set_swap for 16-bit images
    png_set_swap(png_ptr);

    // 4. Use png_set_compression_buffer_size to set compression buffer
    png_set_compression_buffer_size(png_ptr, fuzz_uint64_t_2);

    // Set up image parameters for writing
    int color_type = PNG_COLOR_TYPE_RGB;
    if (image.format & PNG_FORMAT_FLAG_ALPHA) {
        color_type = PNG_COLOR_TYPE_RGBA;
    } else if (image.format & PNG_FORMAT_FLAG_COLOR) {
        color_type = PNG_COLOR_TYPE_RGB;
    } else {
        color_type = PNG_COLOR_TYPE_GRAY;
    }

    // Set image header
    png_set_IHDR(png_ptr, info_ptr, image.width, image.height,
                 fuzz_int32_t_3, fuzz_int32_t_4, fuzz_int32_t_5,
                 fuzz_int32_t_6, fuzz_int32_t_7);

    // 5. Use png_set_sRGB to set sRGB intent
    png_set_sRGB(png_ptr, info_ptr, fuzz_int32_t_8);

    // Write image header
    png_write_info(png_ptr, info_ptr);

    // Allocate memory for image data
    png_bytep row_pointers[image.height];
    size_t row_bytes = PNG_IMAGE_ROW_STRIDE(image);
    
    for (png_uint_32 y = 0; y < image.height; y++) {
        row_pointers[y] = (png_bytep)malloc(row_bytes);
        if (!row_pointers[y]) {
            // Clean up allocated rows
            for (png_uint_32 i = 0; i < y; i++) {
                free(row_pointers[i]);
            }
            png_destroy_write_struct(&png_ptr, &info_ptr);
            png_image_free(&image);
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }

    // Read the image data
    if (!png_image_finish_read(&image, nullptr, (void**)row_pointers, 
                               fuzz_int32_t_9, nullptr)) {
        // Clean up
        for (png_uint_32 y = 0; y < image.height; y++) {
            free(row_pointers[y]);
        }
        png_destroy_write_struct(&png_ptr, &info_ptr);
        png_image_free(&image);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Write image data
    png_write_image(png_ptr, row_pointers);

    // Write end of PNG
    png_write_end(png_ptr, info_ptr);

    // Clean up allocated resources
    for (png_uint_32 y = 0; y < image.height; y++) {
        free(row_pointers[y]);
    }

    // Clean up libpng structures
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    // Clean up png_image
    png_image_free(&image);
    
    // Close output file
    assert_file_closed(&out_file);;
    
    // Remove the output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}