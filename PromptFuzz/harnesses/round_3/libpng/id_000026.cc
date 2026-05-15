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
//<ID> 462
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 21
//<Quality> {"density":15,"unique_branches":{"png_get_cICP":[[769,9,769,24,0,0,4,0],[769,28,769,44,0,0,4,0],[770,9,770,47,0,0,4,0],[770,9,770,47,0,0,4,1],[771,9,771,33,0,0,4,0],[771,37,771,62,0,0,4,0],[772,9,772,36,0,0,4,0],[772,40,772,69,0,0,4,0]],"png_push_fill_buffer":[[426,8,426,38,0,0,4,0],[430,11,430,45,0,0,4,0],[430,11,430,45,0,0,4,1]],"png_get_channels":[[483,8,483,23,0,0,4,0],[483,27,483,43,0,0,4,0]],"png_push_save_buffer":[[463,8,463,38,0,0,4,0],[465,11,465,59,0,0,4,1],[502,11,502,21,0,0,4,0]],"png_get_sBIT":[[1145,8,1145,23,0,0,4,0],[1145,27,1145,43,0,0,4,0],[1146,8,1146,46,0,0,4,0],[1146,8,1146,46,0,0,4,1],[1146,50,1146,65,0,0,4,0]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_set_progressive_read_fn","png_process_data","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_set_IHDR","png_build_grayscale_palette","png_set_shift","png_write_info","png_write_chunk_start","png_write_chunk_data","png_write_chunk_end","png_write_row","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_progressive_read_fn","png_process_data","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_build_grayscale_palette","png_set_shift","png_write_info","png_write_chunk_start","png_write_chunk_data","png_write_chunk_end","png_write_row","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":1}
/**/


// Callback structure for progressive reading
struct FuzzProgressiveInfo {
    png_structp png_ptr;
    png_infop info_ptr;
    bool info_called;
    bool row_called;
    bool end_called;
};

// Progressive read callbacks
void  id_000026_info_callback(png_structp png_ptr, png_infop info_ptr) {
    FuzzProgressiveInfo* fpi = (FuzzProgressiveInfo*)png_get_progressive_ptr(png_ptr);
    fpi->info_called = true;
    
    // Use png_get_channels
    png_byte channels = png_get_channels(png_ptr, info_ptr);
    
    // Use png_get_sBIT
    png_color_8* sig_bits = nullptr;
    if (png_get_sBIT(png_ptr, info_ptr, &sig_bits)) {
        // sBIT information available
    }
    
    // Use png_get_cICP
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    if (png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function, 
                     &matrix_coefficients, &video_full_range_flag)) {
        // CICP information available
    }
}

void  id_000026_row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) {
    FuzzProgressiveInfo* fpi = (FuzzProgressiveInfo*)png_get_progressive_ptr(png_ptr);
    fpi->row_called = true;
    
    // Use png_get_current_pass_number
    png_byte current_pass = png_get_current_pass_number(png_ptr);
}

void  id_000026_end_callback(png_structp png_ptr, png_infop info_ptr) {
    FuzzProgressiveInfo* fpi = (FuzzProgressiveInfo*)png_get_progressive_ptr(png_ptr);
    fpi->end_called = true;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<74) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_10, fuzz_char_array_size_10, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_11, fuzz_str_sz_11, fdp);
	
		const png_byte * fuzzer_var_png_write_chunk_start_24_1 = fuzz_char_array_10;//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	  // Need at least PNG signature
    
    // Check PNG signature
    if (png_sig_cmp((const png_byte*)data, fuzz_uint64_t_1, 8) != 0) {
        return 0;  // Not a PNG file
    }
    
    // Create read structure
    png_structp png_ptr = png_create_read_struct(fuzz_str_2, 
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
    
    // Create memory file for reading
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize progressive reading
    FuzzProgressiveInfo fpi = {png_ptr, info_ptr, false, false, false};
    png_set_progressive_read_fn(png_ptr, &fpi,  id_000026_info_callback,  id_000026_row_callback,  id_000026_end_callback);
    
    // Process PNG data in chunks
    const size_t CHUNK_SIZE = 4096;
    png_byte buffer[CHUNK_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, CHUNK_SIZE, in_file)) > 0) {
        png_process_data(png_ptr, info_ptr, buffer, bytes_read);
    }
    
    assert_file_closed(&in_file);;
    
    // Use png_get_uint_32 on some data
    if (size >= 12) {
        png_uint_32 value = png_get_uint_32((const png_byte*)data + 8);
    }
    
    // Create write structure for output
    png_structp write_ptr = png_create_write_struct(fuzz_str_3, 
                                                    nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling for write
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_init_io(write_ptr, out_file);
    
    // Set up a simple grayscale image for writing
    png_set_IHDR(write_ptr, write_info, fuzz_uint32_t_4, 1, fuzz_int32_t_5, 
                fuzz_int32_t_6, fuzz_int32_t_7,
                fuzz_int32_t_8, fuzz_int32_t_9);
    
    // Use png_build_grayscale_palette
    png_color palette[2];
    png_build_grayscale_palette(1, palette);
    
    // Use png_set_shift
    png_color_8 true_bits;
    true_bits.gray = 1;
    true_bits.alpha = 0;
    png_set_shift(write_ptr, &true_bits);
    
    // Write PNG info
    png_write_info(write_ptr, write_info);
    
    // Use png_write_chunk_start to write a custom chunk
    png_byte chunk_name[] = {'t', 'E', 'X', 't'};
    const char* chunk_data = "FuzzTest";
    png_write_chunk_start(write_ptr, fuzzer_var_png_write_chunk_start_24_1, strlen(chunk_data));
    png_write_chunk_data(write_ptr, fuzz_str_11, static_cast<size_t>(fuzz_str_sz_11));
    png_write_chunk_end(write_ptr);
    
    // Write a single pixel
    png_byte pixel = 0;
    png_write_row(write_ptr, &pixel);
    
    // Finish writing
    png_write_end(write_ptr, write_info);
    
    // Cleanup
    assert_file_closed(&out_file);;
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}