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
//<ID> 407
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 10
//<Quality> {"density":8,"unique_branches":{"png_set_next_frame_fcTL":[[1388,8,1388,23,0,0,4,1],[1388,27,1388,43,0,0,4,1],[1402,8,1402,42,0,0,4,1]],"png_ensure_fcTL_is_valid":[[1434,8,1434,18,0,0,4,1],[1434,22,1434,45,0,0,4,1],[1436,8,1436,19,0,0,4,1],[1436,23,1436,47,0,0,4,1],[1438,8,1438,34,0,0,4,1],[1438,38,1438,64,0,0,4,1],[1440,8,1440,53,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_set_next_frame_fcTL","png_set_eXIf_1","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_write_struct","png_init_io","png_write_info","png_write_row","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_set_next_frame_fcTL","png_set_eXIf_1","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_write_info","png_write_row","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<127) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_3, fuzz_char_array_size_3, fdp);
	
		const png_byte * fuzzer_var_png_write_row_15_1 = fuzz_char_array_3;//fuzzer shim end}




    if (size < 50) {
	return 0;
	}
	 // Need minimum data for operations

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	

    // Create FILE* for output
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize structures
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    
    if (!png_ptr || !info_ptr) {
        if (png_ptr) png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Setup error handling with longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize reading
    png_init_io(png_ptr, in_file);
    png_read_info(png_ptr, info_ptr);

    // Use png_get_int_32 to extract integer from data
    if (size >= 4) {
        png_int_32 extracted_int = png_get_int_32(data);
        // Use the extracted value for something (e.g., as a parameter)
        (void)extracted_int; // Suppress unused warning
    }

    // Setup next frame fcTL (for APNG)
    if (size >= 30) {
        // Extract parameters from data
        png_uint_32 width = *reinterpret_cast<const png_uint_32*>(data);
        png_uint_32 height = *reinterpret_cast<const png_uint_32*>(data + 4);
        png_uint_32 x_offset = *reinterpret_cast<const png_uint_32*>(data + 8);
        png_uint_32 y_offset = *reinterpret_cast<const png_uint_32*>(data + 12);
        png_uint_16 delay_num = *reinterpret_cast<const png_uint_16*>(data + 16);
        png_uint_16 delay_den = *reinterpret_cast<const png_uint_16*>(data + 18);
        png_byte dispose_op = data[20];
        png_byte blend_op = data[21];

        png_set_next_frame_fcTL(png_ptr, info_ptr, 
                               width % 1000, height % 1000,  // Keep reasonable bounds
                               x_offset % 100, y_offset % 100,
                               delay_num, delay_den,
                               dispose_op % 4, blend_op % 2);
    }

    // Setup EXIF data
    if (size >= 40) {
        png_uint_32 num_exif = 10; // Small EXIF chunk
        png_byte* exif_data = new png_byte[num_exif];
        
        // Copy some data for EXIF (ensuring bounds)
        size_t copy_size = (size - 30 > num_exif) ? num_exif : size - 30;
        memcpy(exif_data, data + 30, copy_size);
        
        png_set_eXIf_1(png_ptr, info_ptr, num_exif, exif_data);
        
        delete[] exif_data;
    }

    // Create write structures for output
    png_structp write_ptr = png_create_write_struct(fuzz_str_2, nullptr, nullptr, nullptr);
    png_infop write_info = png_create_info_struct(write_ptr);
    
    if (write_ptr && write_info) {
        if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
            png_destroy_write_struct(&write_ptr, &write_info);
        } else {
            png_init_io(write_ptr, out_file);
            
            // Copy info from read to write
            png_write_info(write_ptr, write_info);
            
            // Write a simple row
            png_byte row[100] = {0};
            if (size > 0) {
                size_t row_size = (size < 100) ? size : 100;
                memcpy(row, data, row_size);
            }
            png_write_row(write_ptr, fuzzer_var_png_write_row_15_1);
            
            png_write_end(write_ptr, nullptr);
            png_destroy_write_struct(&write_ptr, &write_info);
        }
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}