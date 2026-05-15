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
//<score> 80, nr_unique_branch: 5
//<Quality> {"density":16,"unique_branches":{"png_read_image":[[708,8,708,49,0,0,4,0]],"png_malloc_base":[[91,30,91,56,0,0,4,0]],"png_start_read_image":[[295,8,295,23,0,0,4,0],[297,11,297,52,0,0,4,0]],"png_set_compression_buffer_size":[[1899,8,1899,49,0,0,4,0]]},"library_calls":["png_image_begin_read_from_stdio","png_create_read_struct","png_image_free","png_create_info_struct","png_destroy_read_struct","png_image_free","png_set_mem_fn","png_set_swap","png_set_compression_buffer_size","png_set_IHDR","png_set_sRGB","png_get_rowbytes","png_set_longjmp_fn","png_destroy_read_struct","png_image_free","png_read_image","png_destroy_read_struct","png_image_free"],"critical_calls":["png_image_begin_read_from_stdio","png_create_read_struct","png_create_info_struct","png_set_mem_fn","png_set_swap","png_set_compression_buffer_size","png_set_IHDR","png_set_sRGB","png_get_rowbytes","png_set_longjmp_fn","png_read_image","png_destroy_read_struct","png_image_free"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to read a PNG image from memory, apply transformations, and handle the image data:

*/


// Include libpng headers (these would typically come from system includes)
extern "C" {
}

// Custom memory functions for png_set_mem_fn
static void*  id_000013_custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
    return malloc(size);
}

static void  id_000013_custom_free(png_structp png_ptr, void* ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for PNG operations

    FILE* in_file = nullptr;
    png_image image = {};
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_bytep* row_pointers = nullptr;

    // 1. Create FILE* from input data
    in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Initialize png_image structure
    image.version = PNG_IMAGE_VERSION;
    image.opaque = nullptr;

    // 3. Use png_image_begin_read_from_stdio to start reading PNG
    if (!png_image_begin_read_from_stdio(&image, in_file)) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Create png_struct for traditional API usage
    png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        png_image_free(&image);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Create info struct
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        png_image_free(&image);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Use png_set_mem_fn to set custom memory functions
    png_set_mem_fn(png_ptr, nullptr,  id_000013_custom_malloc,  id_000013_custom_free);

    // 7. Use png_set_swap for 16-bit images
    png_set_swap(png_ptr);

    // 8. Use png_set_compression_buffer_size
    png_set_compression_buffer_size(png_ptr, 8192);

    // 9. Use png_set_sRGB (need to set up image info first)
    // Set up basic image info
    png_set_IHDR(png_ptr, info_ptr, 
                 image.width, image.height, 8,
                 PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    
    // Set sRGB intent
    png_set_sRGB(png_ptr, info_ptr, fuzz_int32_t_2);

    // 10. Allocate memory for row pointers
    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * image.height);
    if (row_pointers) {
        for (png_uint_32 y = 0; y < image.height; y++) {
            row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
        }
    }

    // 11. Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        // PNG error occurred - clean up
        if (row_pointers) {
            for (png_uint_32 y = 0; y < image.height; y++) {
                free(row_pointers[y]);
            }
            free(row_pointers);
        }
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 12. Read the image data
    png_read_image(png_ptr, row_pointers);

    // 13. Clean up all allocated resources
    if (row_pointers) {
        for (png_uint_32 y = 0; y < image.height; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    png_image_free(&image);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}