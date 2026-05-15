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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 64.5, nr_unique_branch: 14
//<Quality> {"density":43,"unique_branches":{"png_set_filter":[[1072,8,1072,23,0,0,4,1],[1076,8,1076,71,0,0,4,1],[1081,8,1081,38,0,0,4,0],[1086,10,1086,16,0,0,4,1],[1087,10,1087,16,0,0,4,1],[1088,10,1088,16,0,0,4,1],[1091,10,1091,36,0,0,4,1],[1095,10,1095,35,0,0,4,1],[1098,10,1098,34,0,0,4,1],[1101,10,1101,35,0,0,4,1],[1104,10,1104,37,0,0,4,1],[1107,10,1107,17,0,0,4,0],[1129,11,1129,35,0,0,4,1]],"png_set_read_user_chunk_fn":[[1858,8,1858,23,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_read_user_chunk_fn","png_set_filter","png_read_info","png_set_progressive_read_fn","png_process_data","png_set_invalid","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_malloc_default","png_malloc_default","png_get_rowbytes","png_read_rows","png_read_row","png_free_default","png_free_default","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_malloc_default","png_get_rowbytes","png_malloc_default","png_write_rows","png_free_default","png_free_default","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_read_user_chunk_fn","png_set_filter","png_read_info","png_set_progressive_read_fn","png_process_data","png_set_invalid","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_malloc_default","png_malloc_default","png_get_rowbytes","png_read_rows","png_read_row","png_free_default","png_free_default","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_malloc_default","png_get_rowbytes","png_malloc_default","png_write_rows","png_free_default","png_free_default","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_destroy_read_struct"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process PNG data:

*/


// Include necessary libpng headers

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
        return 0; // Too small to be a PNG
    }

    // Check if the data has PNG signature
    if (png_sig_cmp(data, 0, 8) != 0) {
        return 0; // Not a PNG file
    }

    // Create FILE* for reading from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize libpng structures
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_info *end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up reading from FILE*
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, 8);

    // 1. Set custom chunk handler
    png_set_read_user_chunk_fn(png_ptr, nullptr, nullptr);

    // 2. Set filter method
    png_set_filter(png_ptr, PNG_FILTER_TYPE_BASE, PNG_FILTER_NONE);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // 3. Process data in chunks
    // We'll process the image in chunks using png_process_data
    // First, we need to set up the progressive reading
    png_set_progressive_read_fn(png_ptr, nullptr, nullptr, nullptr, nullptr);
    
    // Process the data in chunks
    size_t processed = 0;
    const size_t chunk_size = 4096;
    while (processed < size) {
        size_t to_process = (size - processed) > chunk_size ? 
                           chunk_size : (size - processed);
        png_process_data(png_ptr, info_ptr, 
                        (png_byte *)(data + processed), to_process);
        processed += to_process;
    }

    // 4. Mark the PNG as invalid for testing
    png_set_invalid(png_ptr, info_ptr, PNG_INFO_IDAT);

    // 5. Read rows of the image
    png_read_update_info(png_ptr, info_ptr);
    
    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Allocate row pointers
    png_bytep *row_pointers = (png_bytep*)png_malloc_default(png_ptr, 
                                                           sizeof(png_bytep) * height);
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc_default(png_ptr, 
                                                       png_get_rowbytes(png_ptr, info_ptr));
    }
    
    // Read all rows at once
    png_read_rows(png_ptr, row_pointers, nullptr, height);
    
    // Alternatively, read rows one by one
    for (png_uint_32 y = 0; y < height; y++) {
        png_read_row(png_ptr, row_pointers[y], nullptr);
    }

    // Clean up row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        png_free_default(png_ptr, row_pointers[y]);
    }
    png_free_default(png_ptr, row_pointers);

    // Read the rest of the image
    png_read_end(png_ptr, end_info);
	FUZZ_FILENAME_PUSH("output_file")

    // Create output FILE for testing
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        // Initialize write structures
        png_struct *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                       nullptr, nullptr, nullptr);
        if (write_ptr) {
            png_info *write_info = png_create_info_struct(write_ptr);
            if (write_info) {
                if (!setjmp(png_jmpbuf(write_ptr))) {
                    png_init_io(write_ptr, out_file);
                    
                    // Copy basic image info
                    png_set_IHDR(write_ptr, write_info, width, height,
                                bit_depth, color_type, PNG_INTERLACE_NONE,
                                PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
                    
                    // Write the image
                    png_write_info(write_ptr, write_info);
                    
                    // Allocate and write rows
                    png_bytep *write_rows = (png_bytep*)png_malloc_default(write_ptr,
                                                                          sizeof(png_bytep) * height);
                    for (png_uint_32 y = 0; y < height; y++) {
                        size_t rowbytes = png_get_rowbytes(write_ptr, write_info);
                        write_rows[y] = (png_byte*)png_malloc_default(write_ptr, rowbytes);
                        memset(write_rows[y], 0, rowbytes); // Create dummy data
                    }
                    png_write_rows(write_ptr, write_rows, height);
                    
                    // Clean up
                    for (png_uint_32 y = 0; y < height; y++) {
                        png_free_default(write_ptr, write_rows[y]);
                    }
                    png_free_default(write_ptr, write_rows);
                    
                    png_write_end(write_ptr, nullptr);
                }
                png_destroy_info_struct(write_ptr, &write_info);
            }
            png_destroy_write_struct(&write_ptr, nullptr);
        }
        assert_file_closed(&out_file);;
    }

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    assert_file_closed(&in_file);;

    // Remove the output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}