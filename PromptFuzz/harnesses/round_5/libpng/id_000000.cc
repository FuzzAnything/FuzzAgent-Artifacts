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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":45,"unique_branches":{"png_create_read_struct_2":[[50,8,50,23,0,0,4,1]]},"library_calls":["png_create_read_struct_2","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_get_IHDR","png_set_chunk_malloc_max","png_set_tRNS_to_alpha","png_read_update_info","png_get_rowbytes","png_malloc_default","png_malloc_default","png_read_image","png_read_end","png_create_write_struct","png_free_default","png_free_default","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_free_default","png_free_default","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_free_default","png_free_default","png_destroy_read_struct","png_destroy_write_struct","png_free_default","png_free_default","png_destroy_read_struct","png_init_io","png_set_IHDR","png_set_sRGB_gAMA_and_cHRM","png_set_sCAL_fixed","png_set_compression_window_bits","png_write_info","png_write_image","png_write_end","png_destroy_write_struct","png_free_default","png_free_default","png_destroy_read_struct"],"critical_calls":["png_create_read_struct_2","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_IHDR","png_set_chunk_malloc_max","png_set_tRNS_to_alpha","png_read_update_info","png_get_rowbytes","png_malloc_default","png_malloc_default","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_sRGB_gAMA_and_cHRM","png_set_sCAL_fixed","png_set_compression_window_bits","png_write_info","png_write_image","png_write_end","png_destroy_write_struct","png_free_default","png_free_default","png_destroy_read_struct"],"visited":2}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process PNG data:

*/


// Define necessary constants from libpng that we need
#define PNG_LIBPNG_VER_STRING "1.6.0"
#define PNG_sRGB_INTENT_PERCEPTUAL 0
#define PNG_SCALE_METER 1
#define PNG_FP_1 100000

// Custom memory allocation functions
extern "C" {
    void* custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
        return malloc(size);
    }
    
    void custom_free(png_structp png_ptr, void* ptr) {
        free(ptr);
    }
}

// Error handling functions
void  id_000000_error_handler(png_structp png_ptr, const char* error_msg) {
    // Error handler - can be empty for fuzzing
}

void  id_000000_warning_handler(png_structp png_ptr, const char* warning_msg) {
    // Warning handler - can be empty for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
        return 0;  // Too small to be a valid PNG
    }
    
    // Check PNG signature
    const uint8_t png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    if (memcmp(data, png_signature, 8) != 0) {
        return 0;  // Not a PNG file
    }
    
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create read structure with custom memory allocation
    png_structp read_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr,
         id_000000_error_handler,
         id_000000_warning_handler,
        nullptr,
        custom_malloc,
        custom_free
    );
    
    if (!read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(read_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize IO
    png_init_io(read_ptr, in_file);
    
    // Read PNG signature (already verified)
    png_set_sig_bytes(read_ptr, 8);
    
    // Read PNG info
    png_read_info(read_ptr, read_info);
    
    // Get image properties
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    
    png_get_IHDR(
        read_ptr, 
        read_info, 
        &width, 
        &height, 
        &bit_depth, 
        &color_type, 
        &interlace_type, 
        &compression_type, 
        &filter_type
    );
    
    // Use the specified APIs
    
    // 1. Set chunk malloc max
    png_set_chunk_malloc_max(read_ptr, 1024 * 1024);  // 1MB max
    
    // 2. Set tRNS to alpha transformation
    png_set_tRNS_to_alpha(read_ptr);
    
    // Update info after transformations
    png_read_update_info(read_ptr, read_info);
    
    // Allocate row pointers
    png_uint_32 rowbytes = png_get_rowbytes(read_ptr, read_info);
    png_bytep* row_pointers = (png_bytep*)png_malloc_default(read_ptr, sizeof(png_bytep) * height);
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc_default(read_ptr, rowbytes);
    }
    
    // Read image data
    png_read_image(read_ptr, row_pointers);
    
    // Read end of PNG
    png_read_end(read_ptr, nullptr);
    
    // Now create write structure to apply the remaining APIs
    png_structp write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
         id_000000_error_handler,
         id_000000_warning_handler
    );
    
    if (!write_ptr) {
        // Cleanup read resources
        for (png_uint_32 y = 0; y < height; y++) {
            png_free_default(read_ptr, row_pointers[y]);
        }
        png_free_default(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        // Cleanup read resources
        for (png_uint_32 y = 0; y < height; y++) {
            png_free_default(read_ptr, row_pointers[y]);
        }
        png_free_default(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        // Cleanup read resources
        for (png_uint_32 y = 0; y < height; y++) {
            png_free_default(read_ptr, row_pointers[y]);
        }
        png_free_default(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_write_struct(&write_ptr, &write_info);
        // Cleanup read resources
        for (png_uint_32 y = 0; y < height; y++) {
            png_free_default(read_ptr, row_pointers[y]);
        }
        png_free_default(read_ptr, row_pointers);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(write_ptr, out_file);
    
    // Set write info from read info
    png_set_IHDR(
        write_ptr,
        write_info,
        width,
        height,
        bit_depth,
        color_type,
        interlace_type,
        compression_type,
        filter_type
    );
    
    // 3. Set sRGB and cHRM chunks
    png_set_sRGB_gAMA_and_cHRM(write_ptr, write_info, PNG_sRGB_INTENT_PERCEPTUAL);
    
    // 4. Set sCAL chunk (fixed point)
    png_set_sCAL_fixed(
        write_ptr,
        write_info,
        PNG_SCALE_METER,
        PNG_FP_1,  // 1.0 in fixed point
        PNG_FP_1   // 1.0 in fixed point
    );
    
    // 5. Set compression buffer size
    // Note: png_set_compression_buffer_size is not in the provided API list,
    // but I'll use a similar API that's available
    png_set_compression_window_bits(write_ptr, 15);  // Alternative API
    
    // Write info
    png_write_info(write_ptr, write_info);
    
    // Write image data
    png_write_image(write_ptr, row_pointers);
    
    // Write end
    png_write_end(write_ptr, nullptr);
    
    // Cleanup write resources
    png_destroy_write_struct(&write_ptr, &write_info);
    assert_file_closed(&out_file);;
    
    // Cleanup read resources
    for (png_uint_32 y = 0; y < height; y++) {
        png_free_default(read_ptr, row_pointers[y]);
    }
    png_free_default(read_ptr, row_pointers);
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}