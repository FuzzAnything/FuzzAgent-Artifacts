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
//<ID> 589
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 1
//<Quality> {"density":36,"unique_branches":{"png_set_crc_action":[[37,7,37,28,0,0,4,0]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_crc_action","png_set_sig_bytes","png_read_info","png_get_user_transform_ptr","png_get_next_frame_height","png_set_gAMA_fixed","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_set_text_compression_mem_level","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_IHDR","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_write_end","png_free","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_crc_action","png_set_sig_bytes","png_read_info","png_get_user_transform_ptr","png_get_next_frame_height","png_set_gAMA_fixed","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_text_compression_mem_level","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_set_IHDR","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_write_end","png_free","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process PNG data:

*/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	//fuzzer shim end}




    // Early return if data is too small to be a PNG
    if (size < 8) {
        return 0;
    }
    
    // Check PNG signature
    if (png_sig_cmp((png_bytep)data, fuzz_uint64_t_1, size > 8 ? 8 : size) != 0) {
        return 0;
    }
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    
    // Create PNG read structure
    png_ptr = png_create_read_struct(fuzz_str_2, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create PNG info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, in_file);
    
    // Set CRC action - handle CRC errors gracefully
    png_set_crc_action(png_ptr, fuzz_int32_t_3, fuzz_int32_t_4);
    
    // Inform PNG we already read signature
    png_set_sig_bytes(png_ptr, fuzz_int32_t_5);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Get user transform pointer (API #3)
    void *user_transform_ptr = png_get_user_transform_ptr(png_ptr);
    
    // Get next frame height (API #4)
    png_uint_32 next_frame_height = png_get_next_frame_height(png_ptr, info_ptr);
    
    // Set gamma in fixed point format (API #2)
    png_fixed_point gamma_fixed = 45455; // ~1/2.2 gamma
    png_set_gAMA_fixed(png_ptr, info_ptr, fuzz_int32_t_6);
    
    // Create write structure for output
    png_structp write_ptr = png_create_write_struct(fuzz_str_7, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Open output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O for writing
    png_init_io(write_ptr, out_file);
    
    // Set text compression memory level (API #1)
    png_set_text_compression_mem_level(write_ptr, fuzz_int32_t_8);
    
    // Copy image info from read to write structure
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Set basic image parameters for output
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                 fuzz_int32_t_9, fuzz_int32_t_10, fuzz_int32_t_11);
    
    // Write info to output
    png_write_info(write_ptr, write_info);
    
    // Read image data row by row and write it out
    png_bytep row_pointers = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
    
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(png_ptr, row_pointers, nullptr);
        png_write_row(write_ptr, row_pointers);
    }
    
    // Finish writing
    png_write_end(write_ptr, nullptr);
    
    // Cleanup
    png_free(png_ptr, row_pointers);
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}