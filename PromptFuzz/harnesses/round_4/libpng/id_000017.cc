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
//<ID> 520
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 0
//<Quality> {"density":27,"unique_branches":{},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_read_user_transform_fn","png_set_filler","png_read_info","png_get_progressive_ptr","png_get_sPLT","png_get_cLLI","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_malloc","png_malloc","png_get_rowbytes","png_read_image","png_read_end","png_image_write_to_memory","png_process_data_skip","png_free","png_free","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_read_user_transform_fn","png_set_filler","png_read_info","png_get_progressive_ptr","png_get_sPLT","png_get_cLLI","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_malloc","png_malloc","png_get_rowbytes","png_read_image","png_read_end","png_image_write_to_memory","png_process_data_skip","png_free","png_free","png_destroy_read_struct"],"visited":0}
/**/


// Custom user transform function
static void  id_000017_user_transform_fn(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    // Simple transform: invert all bytes
    for (png_uint_32 i = 0; i < row_info->rowbytes; i++) {
        data[i] = ~data[i];
    }
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	
    
    // Check if data has PNG signature
    if (png_sig_cmp((png_bytep)data, fuzz_uint64_t_1, 8) != 0) {
        return 0;  // Not a PNG file
    }
    
    // Create read structure
    png_structp png_ptr = png_create_read_struct(fuzz_str_2, NULL, NULL, NULL);
    if (!png_ptr) {
	return 0;
	}
	
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Create memory file for input
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize PNG reading
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, 8);
    
    // Set up user transform function
    png_set_read_user_transform_fn(png_ptr,  id_000017_user_transform_fn);
    
    // Set filler (add alpha channel if needed)
    png_set_filler(png_ptr, fuzz_uint32_t_3, fuzz_int32_t_4);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Get progressive pointer (should be NULL since we're not using progressive reading)
    void *prog_ptr = png_get_progressive_ptr(png_ptr);
    
    // Try to get sPLT entries
    png_sPLT_tp entries;
    int num_entries = png_get_sPLT(png_ptr, info_ptr, &entries);
    
    // Try to get cLLI data
    double max_content_light = 0, max_frame_avg_light = 0;
    png_uint_32 clli_result = png_get_cLLI(png_ptr, info_ptr, &max_content_light, &max_frame_avg_light);
    
    // Read image data
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Allocate row pointers
    png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
    }
    
    // Read image data
    png_read_image(png_ptr, row_pointers);
    png_read_end(png_ptr, info_ptr);
    
    // Create png_image structure for writing
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = width;
    image.height = height;
    image.format = PNG_FORMAT_RGBA;
    
    // Calculate required memory for output
    png_alloc_size_t memory_bytes = PNG_IMAGE_SIZE(image);
    void *memory = malloc(memory_bytes);
    
    if (memory) {
        // Try to write image to memory (this might fail for various reasons)
        int write_result = png_image_write_to_memory(&image, memory, &memory_bytes, 
                                                    fuzz_int32_t_5, row_pointers, width * 4, NULL);
        // Clean up memory
        free(memory);
    }
    
    // Skip some data (if we were processing progressively)
    // This is a no-op in our case since we're not using progressive reading
    png_uint_32 skip_result = png_process_data_skip(png_ptr);
    
    // Use png_get_uint_32 on some data
    if (size >= 12) {
        png_uint_32 value = png_get_uint_32(data + 8);
        (void)value;  // Use value to avoid unused variable warning
    }
    
    // Clean up row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    // Close input file
    assert_file_closed(&in_file);;
    
    // Clean up PNG structures
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    assert_file_closed(&in_file);
	return 0;
}