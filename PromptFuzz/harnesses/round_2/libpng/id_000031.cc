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
//<ID> 606
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 1
//<Quality> {"density":33,"unique_branches":{"png_fixed_ITU":[[2721,8,2721,23,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_set_mem_fn","png_set_mem_fn","png_init_io","png_read_info","png_set_cLLI","png_set_gamma","png_set_compression_window_bits","png_set_tRNS_to_alpha","png_get_copyright","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_IHDR","png_read_update_info","png_get_rowbytes","png_read_image","png_init_io","png_write_info","png_write_image","png_write_end","png_longjmp","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_set_mem_fn","png_set_mem_fn","png_init_io","png_read_info","png_set_cLLI","png_set_gamma","png_set_compression_window_bits","png_set_tRNS_to_alpha","png_get_copyright","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_IHDR","png_read_update_info","png_get_rowbytes","png_read_image","png_init_io","png_write_info","png_write_image","png_write_end","png_longjmp","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process PNG data:

*/


extern "C" {
}

// Custom memory management functions
void*  id_000031_custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
    return malloc(size);
}

void  id_000031_custom_free(png_structp png_ptr, void* ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for PNG operations
    
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create PNG structures for reading
    png_structp png_read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                     NULL, NULL, NULL);
    if (!png_read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop info_read_ptr = png_create_info_struct(png_read_ptr);
    if (!info_read_ptr) {
        png_destroy_read_struct(&png_read_ptr, NULL, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create PNG structures for writing
    png_structp png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                       NULL, NULL, NULL);
    if (!png_write_ptr) {
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop info_write_ptr = png_create_info_struct(png_write_ptr);
    if (!info_write_ptr) {
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, NULL);
        png_destroy_write_struct(&png_write_ptr, NULL);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_read_ptr)) || setjmp(png_jmpbuf(png_write_ptr))) {
        // Clean up on error
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, NULL);
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // ==================== USAGE OF SPECIFIED APIS ====================
    
    // 1. Set custom memory functions using png_set_mem_fn
    png_set_mem_fn(png_read_ptr, NULL,  id_000031_custom_malloc,  id_000031_custom_free);
    png_set_mem_fn(png_write_ptr, NULL,  id_000031_custom_malloc,  id_000031_custom_free);
    
    // 2. Read PNG from memory file
    png_init_io(png_read_ptr, in_file);
    png_read_info(png_read_ptr, info_read_ptr);
    
    // 3. Extract some data for API parameters
    // Extract values for png_set_cLLI from the input data
    double max_content_light = 1000.0;  // Default value
    double max_frame_avg_light = 500.0; // Default value
    
    if (size >= sizeof(double) * 2 + 2) {
        // Try to extract doubles from data (assuming they're stored consecutively)
        memcpy(&max_content_light, data, sizeof(double));
        memcpy(&max_frame_avg_light, data + sizeof(double), sizeof(double));
    }
    
    // 4. Set CLLI metadata
    // Note: png_set_cLLI requires png_struct* and png_info* parameters
    // We'll use png_write_ptr and info_write_ptr for this
    png_set_cLLI(png_write_ptr, info_write_ptr, 
                max_content_light, max_frame_avg_light);
    
    // 5. Set gamma correction
    double screen_gamma = 2.2;  // Typical screen gamma
    double file_gamma = 1.0;    // Assume no file gamma override
    
    if (size >= sizeof(double) * 4 + 2) {
        memcpy(&screen_gamma, data + sizeof(double) * 2, sizeof(double));
        memcpy(&file_gamma, data + sizeof(double) * 3, sizeof(double));
    }
    
    png_set_gamma(png_write_ptr, screen_gamma, file_gamma);
    
    // 6. Extract a 16-bit value using png_get_uint_16
    png_uint_16 extracted_value = 0;
    if (size >= 2) {
        extracted_value = png_get_uint_16(data);
    }
    
    // 7. Use the extracted value for compression window bits
    // Convert to reasonable range (8-15 for deflate window bits)
    int window_bits = 8 + (extracted_value % 8);
    png_set_compression_window_bits(png_write_ptr, window_bits);
    
    // 8. Convert tRNS to alpha channel
    png_set_tRNS_to_alpha(png_write_ptr);
    
    // 9. Get copyright information
    const char* copyright = png_get_copyright(png_read_ptr);
    // (Can be used for logging or condition checks)
    
    // 10. Copy image info from read to write structures
    png_uint_32 width = png_get_image_width(png_read_ptr, info_read_ptr);
    png_uint_32 height = png_get_image_height(png_read_ptr, info_read_ptr);
    png_byte color_type = png_get_color_type(png_read_ptr, info_read_ptr);
    png_byte bit_depth = png_get_bit_depth(png_read_ptr, info_read_ptr);
    
    png_set_IHDR(png_write_ptr, info_write_ptr, width, height,
                 bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // 11. Read image data
    png_read_update_info(png_read_ptr, info_read_ptr);
    
    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_read_ptr, info_read_ptr));
    }
    
    png_read_image(png_read_ptr, row_pointers);
    
    // 12. Write the processed image
    png_init_io(png_write_ptr, out_file);
    png_write_info(png_write_ptr, info_write_ptr);
    png_write_image(png_write_ptr, row_pointers);
    png_write_end(png_write_ptr, NULL);
    
    // 13. Force a longjmp to demonstrate png_longjmp usage
    // (This would normally be called from error handlers)
    if (size > 0 && data[0] == 0xFF) {  // Trigger condition based on input
        png_longjmp(png_write_ptr, 1);
    }
    
    // ==================== CLEANUP ====================
    
    // Free row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    
    // Cleanup PNG structures
    png_destroy_read_struct(&png_read_ptr, &info_read_ptr, NULL);
    png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}