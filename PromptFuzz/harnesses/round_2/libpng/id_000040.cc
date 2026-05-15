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
//<ID> 733
//<Prompt> []
/*<Combination>: [
*/
//<score> 82, nr_unique_branch: 1
//<Quality> {"density":41,"unique_branches":{"png_set_compression_buffer_size":[[1899,8,1899,49,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_chunk_cache_max","png_set_compression_buffer_size","png_read_info","png_handle_as_unknown","png_get_sPLT","png_get_io_chunk_type","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_set_pHYs","png_set_text","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_free","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_chunk_cache_max","png_set_compression_buffer_size","png_read_info","png_handle_as_unknown","png_get_sPLT","png_get_io_chunk_type","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_get_filter_type","png_set_IHDR","png_set_pHYs","png_set_text","png_write_info","png_malloc","png_get_rowbytes","png_read_row","png_write_row","png_free","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // Need at least PNG signature bytes
    
    // Create read structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
	return 0;
	}
	
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling with longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Create FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, in_file);
    
    // Read PNG signature
    png_set_sig_bytes(png_ptr, 8);
    
    // Set chunk cache max
    png_set_chunk_cache_max(png_ptr, 1000);
    
    // Set compression buffer size
    png_set_compression_buffer_size(png_ptr, 8192);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Check for unknown chunks
    png_byte chunk_name[5];
    memcpy(chunk_name, "tEXt", 5); // Example chunk name
    int handle_status = png_handle_as_unknown(png_ptr, chunk_name);
    
    // Get sPLT chunks if present
    png_sPLT_tp entries;
    int num_entries = png_get_sPLT(png_ptr, info_ptr, &entries);
    
    // Get current chunk type during reading
    png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr);
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create write structures
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!write_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_destroy_write_struct(&write_ptr, nullptr);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling for write structure
    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize write I/O
    png_init_io(write_ptr, out_file);
    
    // Copy image info from read to write
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    int interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    int compression_type = png_get_compression_type(png_ptr, info_ptr);
    int filter_type = png_get_filter_type(png_ptr, info_ptr);
    
    // Set basic image info for writing
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);
    
    // Set pHYs (physical dimensions) - using dummy values
    png_set_pHYs(write_ptr, write_info, 3000, 3000, PNG_RESOLUTION_METER);
    
    // Set text chunks - create a simple text entry
    png_text text_ptr[1];
    char key[] = "Title";
    char text[] = "Fuzzed PNG";
    text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
    text_ptr[0].key = key;
    text_ptr[0].text = text;
    text_ptr[0].text_length = strlen(text);
    
    png_set_text(write_ptr, write_info, text_ptr, 1);
    
    // Write image info
    png_write_info(write_ptr, write_info);
    
    // Read and write image data row by row
    png_bytep row = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(png_ptr, row, nullptr);
        png_write_row(write_ptr, row);
    }
    png_free(png_ptr, row);
    
    // End write
    png_write_end(write_ptr, write_info);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    png_destroy_write_struct(&write_ptr, &write_info);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}