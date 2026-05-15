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
//<ID> 360
//<Prompt> []
/*<Combination>: [
*/
//<score> 43.5, nr_unique_branch: 5
//<Quality> {"density":29,"unique_branches":{"png_write_end":[[433,19,433,41,0,0,4,0],[438,14,438,47,0,0,4,1],[458,19,458,77,0,0,4,1],[471,19,471,77,0,0,4,1]],"png_set_text":[[929,8,929,16,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_read_info","png_get_signature","png_destroy_read_struct","png_get_sCAL_s","png_get_sCAL","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_get_IHDR","png_set_IHDR","png_set_text","png_write_info","png_get_rowbytes","png_get_rowbytes","png_write_row","png_write_chunk","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_read_fn","png_get_io_ptr","png_get_io_ptr","png_read_info","png_get_signature","png_get_sCAL_s","png_get_sCAL","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_IHDR","png_set_IHDR","png_set_text","png_write_info","png_get_rowbytes","png_get_rowbytes","png_write_row","png_write_chunk","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<35) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_3, fuzz_char_array_size_3, fdp);
	
		const png_byte * fuzzer_var_png_write_chunk_31_2 = fuzz_char_array_3;
		size_t fuzzer_size_png_write_chunk_31_2 = static_cast<size_t>(fuzz_char_array_size_3);//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // Need at least PNG signature

    // Create read structure
    png_structp png_read_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_read_ptr) {
	return 0;
	}
	
    
    // Create info structure
    png_infop info_read_ptr = png_create_info_struct(png_read_ptr);
    if (!info_read_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_read_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        return 0;
    }
    
    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Use png_set_read_fn to set custom read function
    png_set_read_fn(png_read_ptr, in_file, [](png_structp png_ptr, png_bytep out, png_size_t length) {
        FILE *fp = (FILE*)png_get_io_ptr(png_ptr);
        fread(out, 1, length, fp);
    });
    
    // Read PNG info
    png_read_info(png_read_ptr, info_read_ptr);
    
    // Get PNG signature
    const png_byte* signature = png_get_signature(png_read_ptr, info_read_ptr);
    if (!signature) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to get sCAL chunk using both functions
    int unit = 0;
    char *swidth = nullptr, *sheight = nullptr;
    double width = 0.0, height = 0.0;
    
    png_uint_32 has_sCAL_s = png_get_sCAL_s(png_read_ptr, info_read_ptr, &unit, &swidth, &sheight);
    png_uint_32 has_sCAL = png_get_sCAL(png_read_ptr, info_read_ptr, &unit, &width, &height);
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create write structure
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_structp png_write_ptr = png_create_write_struct(fuzz_str_2, nullptr, nullptr, nullptr);
    if (!png_write_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop info_write_ptr = png_create_info_struct(png_write_ptr);
    if (!info_write_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, nullptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    if (setjmp(png_jmpbuf(png_write_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize write
    png_init_io(png_write_ptr, out_file);
    
    // Copy basic info from read to write
    png_uint_32 width_val, height_val;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    
    png_get_IHDR(png_read_ptr, info_read_ptr, &width_val, &height_val, &bit_depth,
                 &color_type, &interlace_type, &compression_type, &filter_type);
    png_set_IHDR(png_write_ptr, info_write_ptr, width_val, height_val, bit_depth,
                 color_type, interlace_type, compression_type, filter_type);
    
    // Add text chunk using png_set_text
    png_text text_chunk;
    const char* keyword = "FuzzTest";
    const char* text_content = "Generated by fuzzer";
    
    text_chunk.compression = PNG_TEXT_COMPRESSION_NONE;
    text_chunk.key = (char*)keyword;
    text_chunk.text = (char*)text_content;
    text_chunk.text_length = strlen(text_content);
    text_chunk.itxt_length = 0;
    text_chunk.lang = nullptr;
    text_chunk.lang_key = nullptr;
    
    png_set_text(png_write_ptr, info_write_ptr, &text_chunk, 1);
    
    // Write PNG info
    png_write_info(png_write_ptr, info_write_ptr);
    
    // Write image data (simplified - just write dummy row)
    if (width_val > 0 && height_val > 0) {
        png_bytep row = (png_bytep)malloc(png_get_rowbytes(png_read_ptr, info_read_ptr));
        if (row) {
            memset(row, 0, png_get_rowbytes(png_read_ptr, info_read_ptr));
            for (png_uint_32 y = 0; y < height_val; y++) {
                png_write_row(png_write_ptr, row);
            }
            free(row);
        }
    }
    
    // Use png_write_chunk_data to write a custom chunk
    const png_byte custom_chunk_name[] = {'c', 'u', 'S', 'T'}; // Must be 4 bytes
    const png_byte custom_chunk_data[] = {'f', 'u', 'z', 'z', 't', 'e', 's', 't'};
    
    png_write_chunk(png_write_ptr, custom_chunk_name, fuzzer_var_png_write_chunk_31_2, fuzzer_size_png_write_chunk_31_2);
    
    // Finish writing
    png_write_end(png_write_ptr, info_write_ptr);
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    png_destroy_read_struct(&png_read_ptr, &info_read_ptr, nullptr);
    png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}