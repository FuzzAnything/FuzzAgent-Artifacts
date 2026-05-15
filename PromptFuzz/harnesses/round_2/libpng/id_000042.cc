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
//<ID> 738
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_process_data_skip","png_get_IHDR","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_write_fn","png_set_chunk_malloc_max","png_set_filter","png_set_oFFs","png_set_unknown_chunks","png_set_unknown_chunk_location","png_convert_from_struct_tm","png_set_tIME","png_set_IHDR","png_write_info","png_get_rowbytes","png_read_image","png_write_image","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_destroy_info_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_process_data_skip","png_get_IHDR","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_write_fn","png_set_chunk_malloc_max","png_set_filter","png_set_oFFs","png_set_unknown_chunks","png_set_unknown_chunk_location","png_convert_from_struct_tm","png_set_tIME","png_set_IHDR","png_write_info","png_get_rowbytes","png_read_image","png_write_image","png_write_end","png_destroy_info_struct","png_destroy_write_struct","png_destroy_info_struct","png_destroy_read_struct"],"visited":0}
/**/


// Include necessary libpng headers

// Custom write function for output
void  id_000042_user_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    FILE *fp = (FILE*)png_get_io_ptr(png_ptr);
    fwrite(data, 1, length, fp);
}

// Custom flush function for output
void  id_000042_user_flush_data(png_structp png_ptr) {
    FILE *fp = (FILE*)png_get_io_ptr(png_ptr);
    fflush(fp);
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for PNG operations
    
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    png_structp png_read_ptr = nullptr;
    png_structp png_write_ptr = nullptr;
    png_infop read_info_ptr = nullptr;
    png_infop write_info_ptr = nullptr;
    png_bytep *row_pointers = nullptr;
    
    // Step 1: Create input file from memory
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 2: Create read structure
    png_read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Create info structure for reading
    read_info_ptr = png_create_info_struct(png_read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 4: Set up error handling
    if (setjmp(png_jmpbuf(png_read_ptr))) {
	FUZZ_FILE_CLOSE();
        goto cleanup;
    }
    
    // Step 5: Initialize I/O
    png_init_io(png_read_ptr, in_file);
    
    // Step 6: Read PNG info
    png_read_info(png_read_ptr, read_info_ptr);
    
    // Step 7: Process data skip
    png_process_data_skip(png_read_ptr);
    
    // Step 8: Get image info
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;
    png_get_IHDR(png_read_ptr, read_info_ptr, &width, &height,
                 &bit_depth, &color_type, &interlace_type,
                 nullptr, nullptr);
    
    // Ensure we have valid dimensions
    if (width == 0 || height == 0 || width > 10000 || height > 10000) {
        goto cleanup;
    }
    
    // Step 9: Create write structure
    png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_write_ptr) goto cleanup;
    
    // Step 10: Create info structure for writing
    write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) goto cleanup;
    
    // Step 11: Set up write error handling
    if (setjmp(png_jmpbuf(png_write_ptr))) {
	FUZZ_FILE_CLOSE();
        goto cleanup;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Step 12: Create output file
    out_file = fopen("output_file", "wb");
    if (!out_file) goto cleanup;
    
    // Step 13: Set up write I/O
    png_set_write_fn(png_write_ptr, out_file,  id_000042_user_write_data,  id_000042_user_flush_data);
    
    // Step 14: Set chunk malloc max
    png_set_chunk_malloc_max(png_write_ptr, 1024 * 1024);  // 1MB max
    
    // Step 15: Set filter
    png_set_filter(png_write_ptr, 0, PNG_FILTER_NONE);
    
    // Step 16: Set offsets
    png_set_oFFs(png_write_ptr, write_info_ptr, 100, 100, PNG_OFFSET_PIXEL);
    
    // Step 17: Create and set unknown chunks
    png_unknown_chunk unknown_chunks[1];
    memset(unknown_chunks, 0, sizeof(unknown_chunks));
    strncpy((char*)unknown_chunks[0].name, "tEST", 4);
    unknown_chunks[0].name[4] = '\0';
    unknown_chunks[0].data = (png_byte*)"test data";
    unknown_chunks[0].size = 9;
    unknown_chunks[0].location = PNG_HAVE_IHDR;
    png_set_unknown_chunks(png_write_ptr, write_info_ptr, unknown_chunks, 1);
    png_set_unknown_chunk_location(png_write_ptr, write_info_ptr, 0, PNG_HAVE_IHDR);
    
    // Step 18: Convert and set time
    png_time pngtime;
    time_t current_time;
    struct tm *timeinfo;
    
    time(&current_time);
    timeinfo = localtime(&current_time);
    png_convert_from_struct_tm(&pngtime, timeinfo);
    png_set_tIME(png_write_ptr, write_info_ptr, &pngtime);
    
    // Step 19: Set IHDR for output
    png_set_IHDR(png_write_ptr, write_info_ptr, width, height,
                 bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // Step 20: Write info
    png_write_info(png_write_ptr, write_info_ptr);
    
    // Step 21: Allocate and read row pointers
    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    if (row_pointers) {
        png_uint_32 rowbytes = png_get_rowbytes(png_read_ptr, read_info_ptr);
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_byte*)malloc(rowbytes);
            if (!row_pointers[y]) {
                for (png_uint_32 i = 0; i < y; i++) {
                    free(row_pointers[i]);
                }
                free(row_pointers);
                row_pointers = nullptr;
                break;
            }
        }
        
        if (row_pointers) {
            png_read_image(png_read_ptr, row_pointers);
            png_write_image(png_write_ptr, row_pointers);
            
            // Free row pointers
            for (png_uint_32 y = 0; y < height; y++) {
                free(row_pointers[y]);
            }
            free(row_pointers);
        }
    }
    
    // Step 22: Write end
    png_write_end(png_write_ptr, write_info_ptr);
    
cleanup:
    // Step 23: Cleanup
    if (row_pointers && height > 0) {
        for (png_uint_32 y = 0; y < height; y++) {
            if (row_pointers[y]) free(row_pointers[y]);
        }
        free(row_pointers);
    }
    
    if (write_info_ptr) png_destroy_info_struct(png_write_ptr, &write_info_ptr);
    if (png_write_ptr) png_destroy_write_struct(&png_write_ptr, nullptr);
    if (read_info_ptr) png_destroy_info_struct(png_read_ptr, &read_info_ptr);
    if (png_read_ptr) png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}