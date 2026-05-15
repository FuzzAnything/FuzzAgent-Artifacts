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
//<ID> 375
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["png_create_write_struct_2","png_create_info_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_write_struct","png_init_io","png_set_IHDR","png_set_sCAL_fixed","png_set_alpha_mode_fixed","png_set_shift","png_write_info","png_destroy_write_struct","png_save_uint_16","png_save_uint_16","png_write_row","png_write_end","png_destroy_write_struct"],"critical_calls":["png_create_write_struct_2","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_sCAL_fixed","png_set_alpha_mode_fixed","png_set_shift","png_write_info","png_save_uint_16","png_write_row","png_write_end","png_destroy_write_struct"],"visited":0}
/*Here's a fuzz driver that creates a PNG writer and uses the specified APIs to write PNG data:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Too small for meaningful processing
    }

    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create a FILE* for writing output
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_bytep row = nullptr;
    
    // Custom memory allocation functions
    auto malloc_fn = [](png_structp png_ptr, png_alloc_size_t size) -> void* {
        return malloc(size);
    };
    
    auto free_fn = [](png_structp png_ptr, void *ptr) {
        free(ptr);
    };

    // Create write structure with custom memory functions
    png_ptr = png_create_write_struct_2(
        fuzz_str_1,
        nullptr, // error_ptr
        [](png_structp png_ptr, png_const_charp msg) { // error_fn
            // Error handler
        },
        [](png_structp png_ptr, png_const_charp msg) { // warn_fn
            // Warning handler
        },
        nullptr, // mem_ptr
        malloc_fn,
        free_fn
    );
    
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_ptr, &info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        if (row) free(row);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set output to file
    png_init_io(png_ptr, out_file);

    // Set image parameters based on input data
    // Use first 4 bytes for width/height (minimum reasonable size)
    png_uint_32 width = (data[0] << 8) | data[1];
    png_uint_32 height = (data[2] << 8) | data[3];
    
    // Limit to reasonable sizes for fuzzing
    width = width % 1000 + 1;
    height = height % 1000 + 1;
    
    int bit_depth = 16; // Use 16-bit for png_set_shift
    int color_type = PNG_COLOR_TYPE_RGB_ALPHA; // For alpha mode

    png_set_IHDR(png_ptr, info_ptr, width, height, fuzz_int32_t_2,
                 color_type, fuzz_int32_t_3,
                 fuzz_int32_t_4, fuzz_int32_t_5);

    // Use png_set_sCAL_fixed - set physical scale
    png_set_sCAL_fixed(png_ptr, info_ptr, 
                      fuzz_int32_t_6, 
                      png_fixed_point(1000), // width scale
                      png_fixed_point(1000)); // height scale

    // Use png_set_alpha_mode_fixed - set alpha handling
    png_set_alpha_mode_fixed(png_ptr, 
                           fuzz_int32_t_7, // mode
                           png_fixed_point(45455)); // gamma * 100000

    // Use png_set_shift - set shift for 16-bit data
    png_color_8 true_bits;
    true_bits.red = 16;
    true_bits.green = 16;
    true_bits.blue = 16;
    true_bits.gray = 16;
    true_bits.alpha = 16;
    png_set_shift(png_ptr, &true_bits);

    // Write header
    png_write_info(png_ptr, info_ptr);

    // Prepare row buffer
    int bytes_per_pixel = 8; // 16-bit * 4 channels
    int rowbytes = width * bytes_per_pixel;
    row = (png_bytep)malloc(rowbytes);
    if (!row) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Generate image data using png_save_uint_16
    // Use input data to fill image rows
    size_t data_offset = 4; // Skip width/height bytes
    
    for (png_uint_32 y = 0; y < height; y++) {
        png_bytep row_ptr = row;
        
        for (png_uint_32 x = 0; x < width; x++) {
            // Fill each 16-bit channel with data from input
            for (int c = 0; c < 4; c++) { // 4 channels: R,G,B,A
                if (data_offset + 1 < size) {
                    // Create a 16-bit value from two bytes
                    unsigned int value = (data[data_offset] << 8) | 
                                        data[data_offset + 1];
                    data_offset += 2;
                    
                    // Use png_save_uint_16 to store in buffer
                    png_save_uint_16(row_ptr, value);
                    row_ptr += 2; // Move to next 16-bit channel
                } else {
                    // Fill with pattern if out of data
                    unsigned int value = (x + y) * 100 + c * 25;
                    png_save_uint_16(row_ptr, value);
                    row_ptr += 2;
                }
            }
        }
        
        png_write_row(png_ptr, row);
        
        // Reset to beginning of input if we run out
        if (data_offset >= size) {
            data_offset = 4;
        }
    }

    // End writing
    png_write_end(png_ptr, info_ptr);

    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    free(row);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}