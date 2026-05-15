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
//<ID> 201
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 1
//<Quality> {"density":36,"unique_branches":{"png_write_info_before_PLTE":[[90,8,90,57,0,0,4,1]]},"library_calls":["png_image_begin_read_from_stdio","png_create_write_struct","png_image_free","png_create_info_struct","png_destroy_write_struct","png_image_free","png_set_longjmp_fn","png_free","png_free","png_free","png_free","png_destroy_write_struct","png_image_free","png_init_io","png_set_IHDR","png_malloc","png_set_hIST","png_set_text_compression_window_bits","png_write_info_before_PLTE","png_write_info","png_get_rowbytes","png_malloc","png_malloc","png_write_image","png_write_chunk","png_convert_from_struct_tm","png_malloc","png_convert_to_rfc1123_buffer","png_write_end","png_get_compression_buffer_size","png_get_io_chunk_type","png_free","png_free","png_free","png_free","png_destroy_write_struct","png_image_free"],"critical_calls":["png_image_begin_read_from_stdio","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_malloc","png_set_hIST","png_set_text_compression_window_bits","png_write_info_before_PLTE","png_write_info","png_get_rowbytes","png_malloc","png_malloc","png_write_image","png_write_chunk","png_convert_from_struct_tm","png_malloc","png_convert_to_rfc1123_buffer","png_write_end","png_get_compression_buffer_size","png_get_io_chunk_type","png_free","png_free","png_free","png_free","png_destroy_write_struct","png_image_free"],"visited":0}
/*Looking at the APIs provided, I can create a fuzz driver that reads PNG data from the input, processes it, and writes a modified version. The event I'll implement is: **Read a PNG from input data, modify its metadata, and write it back out with a custom chunk.**

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<57) return 0;

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
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // PNG signature is 8 bytes
    
    // 1. Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    // 2. Create a FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Variables to track allocated resources
    png_image read_image = {0};
    png_struct *write_png_ptr = nullptr;
    png_info *write_info_ptr = nullptr;
    png_byte **row_pointers = nullptr;
    png_uint_16 *hist = nullptr;
    char *time_buffer = nullptr;
    
    // 3. Initialize png_image for reading
    read_image.version = PNG_IMAGE_VERSION;
    
    // 4. Begin reading PNG from input
    if (!png_image_begin_read_from_stdio(&read_image, in_file)) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Create write struct
    write_png_ptr = png_create_write_struct(fuzz_str_1, 
                                           nullptr, nullptr, nullptr);
    if (!write_png_ptr) {
        png_image_free(&read_image);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Create info struct
    write_info_ptr = png_create_info_struct(write_png_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&write_png_ptr, nullptr);
        png_image_free(&read_image);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Set up error handling with longjmp
    if (setjmp(png_jmpbuf(write_png_ptr))) {
	FUZZ_FILE_CLOSE();
        // Cleanup on error
        if (row_pointers) {
            for (png_uint_32 i = 0; i < read_image.height; i++) {
                if (row_pointers[i]) png_free(write_png_ptr, row_pointers[i]);
            }
            png_free(write_png_ptr, row_pointers);
        }
        if (hist) png_free(write_png_ptr, hist);
        if (time_buffer) png_free(write_png_ptr, time_buffer);
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        png_image_free(&read_image);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 8. Initialize I/O for writing
    png_init_io(write_png_ptr, out_file);
    
    // 9. Set IHDR based on read image
    int color_type = PNG_COLOR_TYPE_RGB; // Default to RGB
    if (read_image.format & PNG_FORMAT_FLAG_COLOR) {
        if (read_image.format & PNG_FORMAT_FLAG_ALPHA) {
            color_type = PNG_COLOR_TYPE_RGB_ALPHA;
        } else {
            color_type = PNG_COLOR_TYPE_RGB;
        }
    } else {
        if (read_image.format & PNG_FORMAT_FLAG_ALPHA) {
            color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
        } else {
            color_type = PNG_COLOR_TYPE_GRAY;
        }
    }
    
    int bit_depth = (read_image.format & PNG_FORMAT_FLAG_LINEAR) ? 16 : 8;
    
    png_set_IHDR(write_png_ptr, write_info_ptr, 
                 read_image.width, read_image.height,
                 bit_depth, fuzz_int32_t_2,
                 fuzz_int32_t_3,
                 fuzz_int32_t_4,
                 fuzz_int32_t_5);
    
    // 10. Create and set histogram (hIST chunk)
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        // Allocate histogram for palette image
        hist = (png_uint_16*)png_malloc(write_png_ptr, 
                                       sizeof(png_uint_16) * 256);
        for (int i = 0; i < 256; i++) {
            hist[i] = (png_uint_16)(i % 256);
        }
        png_set_hIST(write_png_ptr, write_info_ptr, hist);
    }
    
    // 11. Set text compression window bits
    png_set_text_compression_window_bits(write_png_ptr, fuzz_int32_t_6);
    
    // 12. Write info before PLTE
    png_write_info_before_PLTE(write_png_ptr, write_info_ptr);
    
    // 13. Write info
    png_write_info(write_png_ptr, write_info_ptr);
    
    // 14. Allocate and create dummy image data
    png_uint_32 rowbytes = png_get_rowbytes(write_png_ptr, write_info_ptr);
    row_pointers = (png_byte**)png_malloc(write_png_ptr, 
                                         sizeof(png_byte*) * read_image.height);
    
    for (png_uint_32 i = 0; i < read_image.height; i++) {
        row_pointers[i] = (png_byte*)png_malloc(write_png_ptr, rowbytes);
        // Fill with pattern based on row number
        for (png_uint_32 j = 0; j < rowbytes; j++) {
            row_pointers[i][j] = (png_byte)((i + j) % 256);
        }
    }
    
    // 15. Write image
    png_write_image(write_png_ptr, row_pointers);
    
    // 16. Create and write a custom chunk
    const png_byte chunk_name[5] = {'c', 'u', 's', 't', '\0'};
    const char *chunk_data = "Fuzzer generated chunk";
    png_write_chunk(write_png_ptr, chunk_name, 
                   fuzz_str_7, static_cast<size_t>(fuzz_str_sz_7));
    
    // 17. Create time for RFC1123 conversion
    png_time pngtime;
    time_t current_time = time(nullptr);
    struct tm *timeinfo = gmtime(&current_time);
    png_convert_from_struct_tm(&pngtime, timeinfo);
    
    // 18. Convert to RFC1123 buffer
    time_buffer = (char*)png_malloc(write_png_ptr, 29);
    if (png_convert_to_rfc1123_buffer(time_buffer, &pngtime)) {
        // Could write another chunk with time data if needed
    }
    
    // 19. Write end
    png_write_end(write_png_ptr, write_info_ptr);
    
    // 20. Get compression buffer size
    size_t comp_buffer_size = png_get_compression_buffer_size(write_png_ptr);
    (void)comp_buffer_size; // Use if needed
    
    // 21. Get IO chunk type
    png_uint_32 chunk_type = png_get_io_chunk_type(write_png_ptr);
    (void)chunk_type; // Use if needed
    
    // Cleanup
    for (png_uint_32 i = 0; i < read_image.height; i++) {
        png_free(write_png_ptr, row_pointers[i]);
    }
    png_free(write_png_ptr, row_pointers);
    if (hist) png_free(write_png_ptr, hist);
    if (time_buffer) png_free(write_png_ptr, time_buffer);
    
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    png_image_free(&read_image);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}