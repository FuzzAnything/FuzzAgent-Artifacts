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
//<ID> 844
//<Prompt> []
/*<Combination>: [
*/
//<score> 93, nr_unique_branch: 6
//<Quality> {"density":31,"unique_branches":{"png_build_grayscale_palette":[[866,7,866,13,0,0,4,1],[871,7,871,13,0,0,4,0],[876,7,876,13,0,0,4,0],[881,7,881,13,0,0,4,0]],"png_set_flush":[[970,8,970,23,0,0,4,1],[973,27,973,36,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_longjmp_fn","png_set_progressive_read_fn","png_set_flush","png_process_data","png_get_IHDR","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_set_IHDR","png_set_shift","png_build_grayscale_palette","png_set_PLTE","png_write_info","png_set_chunk_cache_max","png_write_chunk_start","png_write_chunk_data","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_set_progressive_read_fn","png_set_flush","png_process_data","png_get_IHDR","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_shift","png_build_grayscale_palette","png_set_PLTE","png_write_info","png_set_chunk_cache_max","png_write_chunk_start","png_write_chunk_data","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to read a PNG image from input data and write it back to a file:

*/


// Callback structures for progressive reading
struct ProgressiveData {
    png_structp png_ptr;
    png_infop info_ptr;
    bool info_called;
    bool end_called;
};

// Progressive read callbacks
void  id_000041_info_callback(png_structp png_ptr, png_infop info_ptr) {
    ProgressiveData* prog_data = (ProgressiveData*)png_get_progressive_ptr(png_ptr);
    prog_data->info_called = true;
}

void  id_000041_row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) {
    // Row processing callback
}

void  id_000041_end_callback(png_structp png_ptr, png_infop info_ptr) {
    ProgressiveData* prog_data = (ProgressiveData*)png_get_progressive_ptr(png_ptr);
    prog_data->end_called = true;
}

// Error handling
void  id_000041_error_fn(png_structp png_ptr, const char* error_msg) {
    longjmp(png_jmpbuf(png_ptr), 1);
}

void  id_000041_warning_fn(png_structp png_ptr, const char* warning_msg) {
    // Warning handler - do nothing
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<67) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	  // Need at least PNG signature
    
    // 1. Check PNG signature
    if (png_sig_cmp((png_bytep)data, fuzz_uint64_t_1, 8) != 0) {
        return 0;  // Not a PNG file
    }
    
    // 2. Create read structures
    png_structp read_ptr = png_create_read_struct(fuzz_str_2, 
                                                  nullptr,  id_000041_error_fn,  id_000041_warning_fn);
    if (!read_ptr) {
	return 0;
	}
	
    
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        return 0;
    }
    
    // 3. Set up error handling with longjmp
    if (setjmp(png_jmpbuf(read_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        return 0;
    }
    
    jmp_buf* jump_buffer = png_set_longjmp_fn(read_ptr, longjmp, sizeof(jmp_buf));
    
    // 4. Set up progressive reading
    ProgressiveData prog_data = {read_ptr, read_info, false, false};
    png_set_progressive_read_fn(read_ptr, &prog_data,  id_000041_info_callback, 
                                 id_000041_row_callback,  id_000041_end_callback);
    
    // 5. Set flush parameter
    png_set_flush(read_ptr, fuzz_int32_t_3);
    
    // 6. Process the input data
    png_process_data(read_ptr, read_info, (png_bytep)data, size);
    
    // Get image information
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(read_ptr, read_info, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);
    
    // 7. Create write structures
    png_structp write_ptr = png_create_write_struct(fuzz_str_4,
                                                    nullptr,  id_000041_error_fn,  id_000041_warning_fn);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        return 0;
    }
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        return 0;
    }
    
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // 8. Initialize I/O for writing
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        assert_file_closed(&out_file);
	return 0;
    }
    png_init_io(write_ptr, out_file);
    
    // 9. Set image header for writing
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);
    
    // 10. Set shift if needed (for 16-bit images)
    if (bit_depth == 16) {
        png_color_8 sig_bit;
        sig_bit.red = 16;
        sig_bit.green = 16;
        sig_bit.blue = 16;
        sig_bit.gray = 16;
        sig_bit.alpha = 16;
        png_set_shift(write_ptr, &sig_bit);
    }
    
    // 11. Build grayscale palette if needed
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_color palette[256];
        png_build_grayscale_palette(bit_depth, palette);
        png_set_PLTE(write_ptr, write_info, palette, 1 << bit_depth);
    }
    
    // 12. Write info
    png_write_info(write_ptr, write_info);
    
    // 13. Set chunk cache maximum
    png_set_chunk_cache_max(write_ptr, fuzz_uint32_t_5);
    
    // 14. Write a custom chunk (example: tEXt chunk)
    const char* chunk_name = "tEXt";
    const char* chunk_data = "Comment=Fuzzed by libpng";
    png_write_chunk_start(write_ptr, (png_const_bytep)chunk_name, strlen(chunk_data) + 1);
    png_write_chunk_data(write_ptr, fuzz_str_6, static_cast<size_t>(fuzz_str_sz_6));
    
    // Note: We need png_write_chunk_end() but it's not in the provided list
    // We'll skip writing image data rows since we don't have png_write_row() or png_write_image()
    
    // 15. Write end
    png_write_end(write_ptr, write_info);
    
    // 16. Clean up
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	return 0;
}