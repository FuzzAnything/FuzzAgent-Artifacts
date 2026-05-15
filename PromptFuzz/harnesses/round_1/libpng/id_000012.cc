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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 29, nr_unique_branch: 0
//<Quality> {"density":29,"unique_branches":{},"library_calls":["png_image_begin_read_from_stdio","png_image_free","png_create_write_struct","png_image_free","png_set_longjmp_fn","png_destroy_write_struct","png_image_free","png_create_info_struct","png_destroy_write_struct","png_image_free","png_destroy_write_struct","png_image_free","png_init_io","png_set_IHDR","png_set_hIST","png_set_text_compression_window_bits","png_write_info","png_get_rowbytes","png_malloc","png_malloc","png_write_image","png_free","png_free","png_write_chunk","png_write_end","png_get_compression_buffer_size","png_get_io_chunk_type","png_convert_from_struct_tm","png_convert_to_rfc1123_buffer","png_destroy_write_struct","png_image_free"],"critical_calls":["png_image_begin_read_from_stdio","png_create_write_struct","png_set_longjmp_fn","png_create_info_struct","png_init_io","png_set_IHDR","png_set_hIST","png_set_text_compression_window_bits","png_write_info","png_get_rowbytes","png_malloc","png_malloc","png_write_image","png_free","png_free","png_write_chunk","png_write_end","png_get_compression_buffer_size","png_get_io_chunk_type","png_convert_from_struct_tm","png_convert_to_rfc1123_buffer","png_destroy_write_struct","png_image_free"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified libpng APIs to achieve the event of reading a PNG file from input data, modifying it, and writing it back out:

*/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need enough data for PNG operations
    
    // 1. Create read structures for input PNG
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize png_image for reading
    png_image read_image;
    memset(&read_image, 0, sizeof(read_image));
    read_image.version = PNG_IMAGE_VERSION;
    
    // Use png_image_begin_read_from_stdio
    if (!png_image_begin_read_from_stdio(&read_image, in_file)) {
        assert_file_closed(&in_file);;
        png_image_free(&read_image);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Create write structures for output PNG
    png_structp png_ptr = png_create_write_struct(
        fuzz_str_1, 
        nullptr, 
        nullptr, 
        nullptr
    );
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        png_image_free(&read_image);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling with longjmp
    jmp_buf jmpbuf;
    png_set_longjmp_fn(png_ptr, (png_longjmp_ptr)longjmp, sizeof(jmpbuf));
    
    if (setjmp(jmpbuf)) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_ptr, nullptr);
        assert_file_closed(&in_file);;
        png_image_free(&read_image);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        assert_file_closed(&in_file);;
        png_image_free(&read_image);
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // 3. Open output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        assert_file_closed(&in_file);;
        png_image_free(&read_image);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize IO
    png_init_io(png_ptr, out_file);
    
    // 4. Set PNG header using values from read image
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    if (read_image.format & PNG_FORMAT_FLAG_COLOR) {
        color_type = PNG_COLOR_TYPE_RGB;
    } else {
        color_type = PNG_COLOR_TYPE_GRAY;
    }
    
    png_set_IHDR(
        png_ptr, 
        info_ptr, 
        read_image.width, 
        read_image.height,
        bit_depth, 
        color_type, 
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, 
        PNG_FILTER_TYPE_BASE
    );
    
    // 5. Create and set histogram (hIST)
    png_uint_16 hist[256];
    size_t hist_size = (color_type == PNG_COLOR_TYPE_PALETTE) ? 256 : 1;
    for (size_t i = 0; i < hist_size; i++) {
        hist[i] = (png_uint_16)(i % 256);
    }
    png_set_hIST(png_ptr, info_ptr, hist);
    
    // 6. Set text compression
    png_set_text_compression_window_bits(png_ptr, fuzz_int32_t_2);
    
    // 7. Write info
    png_write_info(png_ptr, info_ptr);
    
    // 8. Prepare and write image data
    png_bytep *row_pointers = nullptr;
    png_alloc_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    
    if (row_bytes > 0 && read_image.height > 0) {
        // Allocate memory for rows
        row_pointers = (png_bytep *)png_malloc(png_ptr, 
            sizeof(png_bytep) * read_image.height);
        
        for (png_uint_32 y = 0; y < read_image.height; y++) {
            row_pointers[y] = (png_byte *)png_malloc(png_ptr, row_bytes);
            // Fill with some pattern based on input data
            for (png_alloc_size_t x = 0; x < row_bytes; x++) {
                row_pointers[y][x] = (png_byte)((x + y + data[y % size]) % 256);
            }
        }
        
        png_write_image(png_ptr, row_pointers);
        
        // Free row memory
        for (png_uint_32 y = 0; y < read_image.height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
    }
    
    // 9. Create and write a custom chunk
    const png_byte chunk_name[4] = {'t', 'E', 'S', 't'};  // Custom chunk type
    png_byte chunk_data[50];
    size_t chunk_data_len = (size < 50) ? size : 50;
    memcpy(chunk_data, data, chunk_data_len);
    png_write_chunk(png_ptr, chunk_name, chunk_data, chunk_data_len);
    
    // 10. Write end
    png_write_end(png_ptr, info_ptr);
    
    // 11. Use utility functions
    // Get compression buffer size
    size_t comp_buf_size = png_get_compression_buffer_size(png_ptr);
    (void)comp_buf_size;  // Use to avoid unused variable warning
    
    // Get IO chunk type
    png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr);
    (void)chunk_type;  // Use to avoid unused variable warning
    
    // Convert time to RFC1123 format
    png_time ptime;
    time_t current_time = time(nullptr);
    struct tm *tm_time = gmtime(&current_time);
    png_convert_from_struct_tm(&ptime, tm_time);
    
    char time_buffer[29];
    if (png_convert_to_rfc1123_buffer(time_buffer, &ptime)) {
        // Time converted successfully
    }
    
    // 12. Clean up everything
    png_destroy_write_struct(&png_ptr, &info_ptr);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_image_free(&read_image);
    
    // 13. Clean up output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}