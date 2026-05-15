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
//<ID> 106
//<Prompt> []
/*<Combination>: [
*/
//<score> 57, nr_unique_branch: 3
//<Quality> {"density":38,"unique_branches":{"png_get_user_width_max":[[1307,12,1307,19,0,0,4,0]],"png_get_palette_max":[[1351,8,1351,23,0,0,4,0],[1351,27,1351,43,0,0,4,0]]},"library_calls":["png_sig_cmp","png_create_read_struct_2","png_set_longjmp_fn","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_init_io","png_read_info","png_set_rgb_to_gray_fixed","png_set_flush","png_get_user_width_max","png_reset_zstream","png_get_palette_max","png_read_png","png_get_image_width","png_get_color_type","png_get_bit_depth","png_get_rows","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_set_IHDR","png_get_image_height","png_get_interlace_type","png_write_info","png_write_rows","png_get_image_height","png_write_end","png_malloc","png_get_image_height","png_image_write_to_memory","png_free","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct_2","png_set_longjmp_fn","png_create_info_struct","png_init_io","png_read_info","png_set_rgb_to_gray_fixed","png_set_flush","png_get_user_width_max","png_reset_zstream","png_get_palette_max","png_read_png","png_get_image_width","png_get_color_type","png_get_bit_depth","png_get_rows","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_get_interlace_type","png_get_image_height","png_write_info","png_write_rows","png_get_image_height","png_write_end","png_malloc","png_get_image_height","png_image_write_to_memory","png_free","png_destroy_write_struct","png_destroy_read_struct"],"visited":1}
/*Looking at the APIs provided and the requirements, I'll create a fuzz driver that reads a PNG image from memory, applies transformations, and writes it back. Here's a step-by-step implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	  // Need at least PNG signature
    
    // Check if data has PNG signature
    if (png_sig_cmp(data, 0, 8) != 0) {
	return 0;
	}
	
    
    // Create FILE* for reading from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    // Create FILE* for writing output
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Error handling variables
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    jmp_buf *jmp_context = nullptr;
    
    // Step 1: Create read structure with custom memory management
    png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr,  // error_ptr
        nullptr,  // error_fn
        nullptr,  // warn_fn
        nullptr,  // mem_ptr
        nullptr,  // malloc_fn - use default
        nullptr   // free_fn - use default
    );
    
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 2: Set up longjmp for error handling
    jmp_context = png_set_longjmp_fn(png_ptr, longjmp, sizeof(jmp_buf));
    if (setjmp(*jmp_context)) {
	FUZZ_FILE_CLOSE();
        // Error occurred, clean up
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Create info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 4: Initialize I/O
    png_init_io(png_ptr, in_file);
    
    // Step 5: Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Step 6: Apply RGB to grayscale transformation
    // Using fixed-point values for coefficients (0.299 and 0.587)
    png_set_rgb_to_gray_fixed(png_ptr, 1, 29900, 58700);
    
    // Step 7: Set flush to write after every row
    png_set_flush(png_ptr, 1);
    
    // Step 8: Get user width max
    png_uint_32 width_max = png_get_user_width_max(png_ptr);
    (void)width_max;  // Use the value to satisfy the requirement
    
    // Step 9: Reset zstream (though this is typically for writing)
    png_reset_zstream(png_ptr);
    
    // Step 10: Get palette max
    int palette_max = png_get_palette_max(png_ptr, info_ptr);
    (void)palette_max;  // Use the value
    
    // Step 11: Read the entire PNG
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
    
    // Step 12: Get image width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    
    // Step 13: Get color type
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    
    // Step 14: Get bit depth
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Step 15: Get rows
    png_bytepp rows = png_get_rows(png_ptr, info_ptr);
    
    // Prepare for writing
    // We need to create a write structure and info
    png_structp write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    
    if (!write_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up write error handling
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(write_ptr, out_file);
    
    // Copy basic info
    png_set_IHDR(write_ptr, write_info, width, 
                 png_get_image_height(png_ptr, info_ptr),
                 bit_depth, color_type, 
                 png_get_interlace_type(png_ptr, info_ptr),
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    
    png_write_info(write_ptr, write_info);
    
    // Write rows
    png_write_rows(write_ptr, rows, 
                   png_get_image_height(png_ptr, info_ptr));
    
    png_write_end(write_ptr, nullptr);
    
    // Step 16: Allocate memory using png_malloc
    png_alloc_size_t alloc_size = 1024;
    void *allocated_mem = png_malloc(png_ptr, alloc_size);
    
    // Step 17: Prepare for png_image_write_to_memory
    // We need to convert the data to a format suitable for png_image
    png_image image = {};
    image.version = PNG_IMAGE_VERSION;
    image.width = width;
    image.height = png_get_image_height(png_ptr, info_ptr);
    image.format = PNG_FORMAT_RGBA;
    
    // Calculate buffer size needed
    png_alloc_size_t memory_bytes = 0;
    if (PNG_IMAGE_SIZE(image) > 0) {
        memory_bytes = PNG_IMAGE_SIZE(image);
    }
    
    // Step 18: Try to write to memory (this may fail but we call it anyway)
    int result = png_image_write_to_memory(
        &image,
        allocated_mem,  // memory
        &memory_bytes,  // memory_bytes
        0,              // convert_to_8_bit
        rows[0],        // buffer - using first row as example
        0,              // row_stride
        nullptr         // colormap
    );
    (void)result;  // Result may be 0 but we don't care for fuzzing
    
    // Step 19: Clean up memory
    png_free(png_ptr, allocated_mem);
    
    // Clean up all structures
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}