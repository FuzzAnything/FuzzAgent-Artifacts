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
//<ID> 933
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 3
//<Quality> {"density":15,"unique_branches":{"png_set_cLLI_fixed":[[174,8,174,28,0,0,4,0],[174,32,174,53,0,0,4,0]],"png_set_compression_method":[[1273,8,1273,19,0,0,4,1]]},"library_calls":["png_create_write_struct","png_create_info_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_write_struct","png_init_io","png_set_IHDR","png_set_cLLI_fixed","png_set_compression_method","png_set_add_alpha","png_write_info","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_x_offset_pixels","png_handle_as_unknown","png_set_text","png_set_unknown_chunk_location","png_destroy_read_struct","png_destroy_read_struct","png_write_row","png_write_end","png_destroy_write_struct"],"critical_calls":["png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_set_cLLI_fixed","png_set_compression_method","png_set_add_alpha","png_write_info","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_x_offset_pixels","png_handle_as_unknown","png_set_text","png_set_unknown_chunk_location","png_destroy_read_struct","png_write_row","png_write_end","png_destroy_write_struct"],"visited":0}
/**/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<67) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
	//fuzzer shim end}




    if (size < 50) {
	return 0;
	}
	 // Need minimum data for PNG operations

    // Create FILE pointers for reading and writing
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG structures
    png_struct *png_ptr = png_create_write_struct(fuzz_str_1, 
                                                   nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_ptr, &info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        remove("output_file");
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, out_file);
    
    // Create a dummy image for testing
    const png_uint_32 width = 16;
    const png_uint_32 height = 16;
    const int color_type = PNG_COLOR_TYPE_RGB;
    const int bit_depth = 8;
    
    png_set_IHDR(png_ptr, info_ptr, width, fuzz_uint32_t_2, fuzz_int32_t_3, fuzz_int32_t_4,
                 fuzz_int32_t_5, fuzz_int32_t_6,
                 fuzz_int32_t_7);

    // API 1: Set cLLI (Content Light Level Information) - fixed point version
    // Extract values from input data
    if (size >= 8) {
        png_uint_32 max_content_light = png_get_uint_32(data);
        png_uint_32 max_frame_avg_light = png_get_uint_32(data + 4);
        png_set_cLLI_fixed(png_ptr, info_ptr, max_content_light, max_frame_avg_light);
    }

    // API 2: Set compression method
    // Extract method from input data (mask to valid range 0-9)
    int compression_method = data[8] % 10;
    png_set_compression_method(png_ptr, compression_method);

    // API 3: Add alpha channel if needed
    // Use filler value from input data
    png_uint_32 filler = (size >= 12) ? png_get_uint_32(data + 8) : 0xFF;
    png_set_add_alpha(png_ptr, filler, fuzz_int32_t_8);

    // Write image info
    png_write_info(png_ptr, info_ptr);

    // API 4: Get x offset pixels (for APNG or similar)
    // This requires reading an existing PNG, so we'll create a read structure
    png_struct *read_ptr = png_create_read_struct(fuzz_str_9,
                                                  nullptr, nullptr, nullptr);
    if (read_ptr) {
        png_info *read_info = png_create_info_struct(read_ptr);
        if (read_info) {
            if (!setjmp(png_jmpbuf(read_ptr))) {
                // Reset file pointer to beginning
                fseek(in_file, 0, SEEK_SET);
                png_init_io(read_ptr, in_file);
                
                // Read PNG info to get valid x_offset
                png_read_info(read_ptr, read_info);
                
                // API 5: Get x offset pixels
                png_int_32 x_offset = png_get_x_offset_pixels(read_ptr, read_info);
                (void)x_offset; // Use to avoid unused variable warning
                
                // API 6: Set unknown chunk location
                // We need to have an unknown chunk first. Let's create one.
                png_byte chunk_name[5] = "tEXt";
                chunk_name[4] = '\0';
                
                // Check if we can handle this as unknown
                int is_unknown = png_handle_as_unknown(read_ptr, fuzz_str_10);
                (void)is_unknown; // Use to avoid warning
                
                // For setting location, we need an actual chunk in info
                // We'll create a simple text chunk
                png_text text_chunk;
                text_chunk.compression = PNG_TEXT_COMPRESSION_NONE;
                text_chunk.key = (char*)"Comment";
                text_chunk.text = (char*)"Fuzzer generated";
                text_chunk.text_length = strlen(text_chunk.text);
                text_chunk.itxt_length = 0;
                text_chunk.lang = nullptr;
                text_chunk.lang_key = nullptr;
                
                png_set_text(png_ptr, info_ptr, &text_chunk, 1);
                
                // Now we can set its location
                png_set_unknown_chunk_location(png_ptr, info_ptr, fuzz_int32_t_11, fuzz_int32_t_12);
            }
            png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        } else {
            png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        }
    }

    // Create dummy image data
    png_byte row[width * 3]; // RGB, no alpha
    for (png_uint_32 y = 0; y < height; y++) {
        for (png_uint_32 x = 0; x < width * 3; x++) {
            row[x] = (png_byte)((x + y) % 256);
        }
        png_write_row(png_ptr, row);
    }

    // Finish writing
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file to avoid accumulation
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}