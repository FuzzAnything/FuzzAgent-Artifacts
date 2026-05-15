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
//<ID> 202
//<Prompt> []
/*<Combination>: [
*/
//<score> 14.5, nr_unique_branch: 0
//<Quality> {"density":29,"unique_branches":{},"library_calls":["png_image_begin_read_from_stdio","png_create_write_struct","png_image_free","png_set_longjmp_fn","png_create_info_struct","png_destroy_write_struct","png_image_free","png_destroy_write_struct","png_image_free","png_init_io","png_get_compression_buffer_size","png_set_text_compression_window_bits","png_set_IHDR","png_malloc","png_set_hIST","png_write_info","png_malloc","png_malloc","png_write_image","png_convert_from_struct_tm","png_convert_to_rfc1123_buffer","png_malloc","png_write_chunk","png_free","png_get_io_chunk_type","png_write_end","png_free","png_free","png_free","png_destroy_write_struct","png_image_free"],"critical_calls":["png_image_begin_read_from_stdio","png_create_write_struct","png_set_longjmp_fn","png_create_info_struct","png_init_io","png_get_compression_buffer_size","png_set_text_compression_window_bits","png_set_IHDR","png_malloc","png_set_hIST","png_write_info","png_malloc","png_malloc","png_write_image","png_convert_from_struct_tm","png_convert_to_rfc1123_buffer","png_malloc","png_write_chunk","png_free","png_get_io_chunk_type","png_write_end","png_free","png_free","png_free","png_destroy_write_struct","png_image_free"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size < 50) {
	return 0;
	}
	  // Need minimum data for PNG operations
    
    // Create a temporary file for reading PNG data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize png_image for reading
    png_image image_read;
    memset(&image_read, 0, sizeof(image_read));
    image_read.version = PNG_IMAGE_VERSION;
    
    // Begin reading PNG from input
    if (!png_image_begin_read_from_stdio(&image_read, in_file)) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create write structures
    jmp_buf jmpbuf;
    png_structp png_ptr = png_create_write_struct(
        fuzz_str_1, 
        nullptr, 
        nullptr, 
        nullptr
    );
    
    if (!png_ptr) {
        png_image_free(&image_read);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up longjmp for error handling
    png_set_longjmp_fn(png_ptr, longjmp, sizeof(jmp_buf));
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        png_image_free(&image_read);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(jmpbuf)) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_ptr, &info_ptr);
        png_image_free(&image_read);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, out_file);
    
    // Get compression buffer size
    size_t comp_buffer_size = png_get_compression_buffer_size(png_ptr);
    
    // Set text compression window bits
    png_set_text_compression_window_bits(png_ptr, fuzz_int32_t_2);
    
    // Set image header with dimensions from read image
    png_set_IHDR(
        png_ptr,
        info_ptr,
        image_read.width > 0 ? image_read.width : 100,
        image_read.height > 0 ? image_read.height : 100,
        8,  // bit_depth
        PNG_COLOR_TYPE_RGB,  // color_type
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    
    // Create and set histogram (hIST chunk)
    int hist_entries = 256;
    png_uint_16 *hist = (png_uint_16*)png_malloc(png_ptr, 
        hist_entries * sizeof(png_uint_16));
    
    // Fill histogram with data from input
    for (int i = 0; i < hist_entries && i * 2 + 2 < (int)size; i++) {
        hist[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
    png_set_hIST(png_ptr, info_ptr, hist);
    
    // Write info before PLTE
    png_write_info(png_ptr, info_ptr);
    
    // Allocate image rows for writing
    int width = image_read.width > 0 ? image_read.width : 100;
    int height = image_read.height > 0 ? image_read.height : 100;
    png_byte **row_pointers = (png_byte**)png_malloc(png_ptr, 
        height * sizeof(png_byte*));
    
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_ptr, 
            width * 3 * sizeof(png_byte));  // RGB
        // Fill with pattern based on input data
        for (int x = 0; x < width * 3; x++) {
            int data_idx = (y * width * 3 + x) % size;
            row_pointers[y][x] = data[data_idx];
        }
    }
    
    // Write image data
    png_write_image(png_ptr, row_pointers);
    
    // Create time for tEXt chunk
    png_time ptime;
    time_t current_time = time(nullptr);
    struct tm *tm_time = localtime(&current_time);
    png_convert_from_struct_tm(&ptime, tm_time);
    
    // Write a custom chunk using png_write_chunk
    // Creating a tEXt chunk
    const png_byte chunk_name[] = {'t', 'E', 'X', 't'};
    char time_str[29];
    png_convert_to_rfc1123_buffer(time_str, &ptime);
    
    // Create chunk data: keyword + null + text
    const char *keyword = "Creation Time";
    size_t keyword_len = strlen(keyword);
    size_t chunk_data_size = keyword_len + 1 + strlen(time_str) + 1;
    png_byte *chunk_data = (png_byte*)png_malloc(png_ptr, chunk_data_size);
    
    memcpy(chunk_data, keyword, keyword_len);
    chunk_data[keyword_len] = 0;
    memcpy(chunk_data + keyword_len + 1, time_str, strlen(time_str) + 1);
    
    png_write_chunk(png_ptr, chunk_name, chunk_data, chunk_data_size);
    png_free(png_ptr, chunk_data);
    
    // Get IO chunk type
    png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr);
    
    // Write end
    png_write_end(png_ptr, info_ptr);
    
    // Clean up allocated memory
    png_free(png_ptr, hist);
    
    for (int y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    // Destroy structures
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    // Free read image
    png_image_free(&image_read);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}