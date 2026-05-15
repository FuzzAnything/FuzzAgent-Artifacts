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
//<ID> 731
//<Prompt> []
/*<Combination>: [
*/
//<score> 140, nr_unique_branch: 13
//<Quality> {"density":35,"unique_branches":{"png_get_sPLT":[[750,8,750,23,0,0,4,0],[750,27,750,43,0,0,4,0],[750,47,750,64,0,0,4,0]],"png_set_compression_buffer_size":[[1892,8,1892,23,0,0,4,1],[1895,8,1895,17,0,0,4,1],[1895,21,1895,43,0,0,4,1],[1899,8,1899,49,0,0,4,1],[1907,8,1907,49,0,0,4,0],[1909,11,1909,31,0,0,4,1],[1921,11,1921,29,0,0,4,1],[1929,11,1929,19,0,0,4,1],[1940,11,1940,40,0,0,4,1]],"png_set_text":[[929,8,929,16,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_set_chunk_cache_max","png_read_info","png_get_io_chunk_type","png_handle_as_unknown","png_get_sPLT","png_get_sPLT","png_set_text","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_set_compression_buffer_size","png_set_pHYs","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_set_IHDR","png_write_info","png_write_end","png_longjmp","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_chunk_cache_max","png_read_info","png_get_io_chunk_type","png_handle_as_unknown","png_get_sPLT","png_get_sPLT","png_set_text","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_compression_buffer_size","png_set_pHYs","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_set_IHDR","png_write_info","png_write_end","png_longjmp","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Setup error handling
    jmp_buf jmpbuf;
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_infop end_info = nullptr;
    
    // Create read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }
    
    // Create info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Create end info structure
    end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling with longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return 0;
    }
    
    // Create input file from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize IO
    png_init_io(png_ptr, in_file);
    
    // Set chunk cache max
    png_set_chunk_cache_max(png_ptr, 1000);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Get current chunk type
    png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr);
    
    // Check how certain chunks are handled as unknown
    const png_byte test_chunk[] = {'t', 'E', 'S', 't'};
    int handle_status = png_handle_as_unknown(png_ptr, test_chunk);
    
    // Try to get sPLT chunks
    png_sPLT_tp entries;
    int num_entries = 0;
    if (png_get_sPLT(png_ptr, info_ptr, &entries) > 0) {
        // sPLT entries found
        num_entries = png_get_sPLT(png_ptr, info_ptr, &entries);
    }
    
    // Set up text chunks
    png_text text_chunks[1];
    memset(text_chunks, 0, sizeof(text_chunks));
    
    text_chunks[0].compression = PNG_TEXT_COMPRESSION_NONE;
    text_chunks[0].key = (char *)"FuzzTest";
    text_chunks[0].text = (char *)"This is a fuzz test";
    text_chunks[0].text_length = strlen(text_chunks[0].text);
    
    png_set_text(png_ptr, info_ptr, text_chunks, 1);
    
    // Create output structures for writing
    png_structp write_ptr = nullptr;
    png_infop write_info = nullptr;
    
    // Create write structure
    write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create write info structure
    write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling for write
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set text compression memory level
    #ifdef PNG_TEXT_COMPRESSION_MEM_LEVEL_SUPPORTED
    png_set_text_compression_mem_level(write_ptr, 8);
    #endif
    
    // Set compression buffer size
    png_set_compression_buffer_size(write_ptr, 8192);
    
    // Set pHYs chunk
    png_set_pHYs(write_ptr, write_info, 72, 72, PNG_RESOLUTION_METER);
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        png_init_io(write_ptr, out_file);
        
        // Copy basic info from read to write
        png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        int color_type = png_get_color_type(png_ptr, info_ptr);
        
        png_set_IHDR(write_ptr, write_info, width, height, bit_depth,
                     color_type, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        
        // Write PNG
        png_write_info(write_ptr, write_info);
        png_write_end(write_ptr, write_info);
        
        assert_file_closed(&out_file);;
    }
    
    // Test png_longjmp in error condition
    if (size < 8) {
        png_longjmp(png_ptr, 1);
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    png_destroy_write_struct(&write_ptr, &write_info);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}