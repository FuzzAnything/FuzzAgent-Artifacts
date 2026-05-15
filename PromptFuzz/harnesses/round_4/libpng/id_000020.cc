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
//<ID> 542
//<Prompt> []
/*<Combination>: [
*/
//<score> 132, nr_unique_branch: 6
//<Quality> {"density":44,"unique_branches":{"png_read_frame_head":[[201,8,201,40,0,0,4,0]],"png_get_gAMA":[[683,8,683,23,0,0,4,0],[683,27,683,43,0,0,4,0],[684,8,684,46,0,0,4,0],[684,8,684,46,0,0,4,1],[686,11,686,29,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_get_gAMA","png_read_frame_head","png_get_next_frame_height","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_set_alpha_mode","png_set_invert_mono","png_write_info","png_get_rowbytes","png_malloc","png_read_image","png_write_chunk_start","png_write_chunk_data","png_write_chunk_end","png_write_rows","png_data_freer","png_write_end","png_free","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_gAMA","png_read_frame_head","png_get_next_frame_height","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_set_alpha_mode","png_set_invert_mono","png_write_info","png_get_rowbytes","png_malloc","png_read_image","png_write_chunk_start","png_write_chunk_data","png_write_chunk_end","png_write_rows","png_data_freer","png_write_end","png_free","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process PNG data:

*/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_5, fuzz_char_array_size_5, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
	
		const png_byte * fuzzer_var_png_write_chunk_start_37_1 = fuzz_char_array_5;
		FDPGetRandomInteger(png_uint_32, fuzzer_index_png_write_chunk_start_37_1, fuzz_char_array_size_5)//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	  // Need at least PNG signature
    
    // Create read structure
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, 
                                                 nullptr, nullptr, nullptr);
    if (!png_ptr) {
	return 0;
	}
	
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Create input file from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, in_file);
    
    // Read PNG signature
    png_set_sig_bytes(png_ptr, fuzz_int32_t_2);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Get gamma information
    double file_gamma;
    png_uint_32 gamma_result = png_get_gAMA(png_ptr, info_ptr, &file_gamma);
    
    // Read frame head (for APNG)
    png_read_frame_head(png_ptr, info_ptr);
    
    // Get next frame height
    png_uint_32 next_frame_height = png_get_next_frame_height(png_ptr, info_ptr);
    
    // Create write structure
    png_structp write_ptr = png_create_write_struct(fuzz_str_3,
                                                    nullptr, nullptr, nullptr);
    if (!write_ptr) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create write info structure
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling for write
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize write I/O
    png_init_io(write_ptr, out_file);
    
    // Copy info from read to write
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    int interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    int compression_type = png_get_compression_type(png_ptr, info_ptr);
    int filter_type = png_get_filter_type(png_ptr, info_ptr);
    
    // Set write info
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth,
                 color_type, interlace_type, compression_type, filter_type);
    
    // Apply transformations
    png_set_alpha_mode(write_ptr, fuzz_int32_t_4, 2.2);
    png_set_invert_mono(write_ptr);
    
    // Write info
    png_write_info(write_ptr, write_info);
    
    // Allocate row pointers
    png_bytep row_pointers[height];
    png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)png_malloc(png_ptr, rowbytes);
    }
    
    // Read image data
    png_read_image(png_ptr, row_pointers);
    
    // Start a custom chunk
    png_byte chunk_name[4] = {'t', 'E', 'X', 't'};
    const char *chunk_data = "Fuzzed PNG";
    png_uint_32 chunk_length = strlen(chunk_data);
    png_write_chunk_start(write_ptr, fuzzer_var_png_write_chunk_start_37_1, fuzzer_index_png_write_chunk_start_37_1);
    png_write_chunk_data(write_ptr, fuzz_str_6, static_cast<size_t>(fuzz_str_sz_6));
    png_write_chunk_end(write_ptr);
    
    // Write rows
    png_write_rows(write_ptr, row_pointers, height);
    
    // Free data using png_data_freer
    png_data_freer(png_ptr, info_ptr, fuzz_int32_t_7, fuzz_uint32_t_8);
    
    // End write
    png_write_end(write_ptr, write_info);
    
    // Cleanup
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    png_destroy_write_struct(&write_ptr, &write_info);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}