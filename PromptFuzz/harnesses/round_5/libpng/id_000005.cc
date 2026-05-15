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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"png_write_info_before_PLTE":[[201,14,201,52,0,0,4,0]],"png_write_iCCP":[[1156,8,1156,23,0,0,4,1],[1159,8,1159,25,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_mem_ptr","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_write_struct","png_init_io","png_get_IHDR","png_set_IHDR","png_set_iCCP","png_write_info","png_chunk_benign_error","png_destroy_write_struct","png_get_x_offset_pixels","png_get_sCAL","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_mem_ptr","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_IHDR","png_set_IHDR","png_set_iCCP","png_write_info","png_chunk_benign_error","png_destroy_write_struct","png_get_x_offset_pixels","png_get_sCAL","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimal data for PNG operations
    
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
    
    // Initialize structures
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, in_file);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Use png_get_mem_ptr
    void *mem_ptr = png_get_mem_ptr(png_ptr);
    (void)mem_ptr; // Use variable to avoid unused warning
    
    // Create write structure for setting iCCP
    png_struct *write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_info *write_info = png_create_info_struct(write_ptr);
    
    if (write_ptr && write_info) {
        if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
            png_destroy_write_struct(&write_ptr, &write_info);
        } else {
            png_init_io(write_ptr, out_file);
            
            // Copy basic info from read structure
            png_uint_32 width, height;
            int bit_depth, color_type, interlace_method, compression_method, filter_method;
            png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                        &interlace_method, &compression_method, &filter_method);
            png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                        interlace_method, compression_method, filter_method);
            
            // Use png_set_iCCP with dummy profile data
            const char *icc_name = "test_profile";
            // Use a small portion of input data as fake ICC profile
            size_t profile_len = size > 200 ? 100 : size / 2;
            png_byte *profile_data = new png_byte[profile_len];
            memcpy(profile_data, data, profile_len);
            png_set_iCCP(write_ptr, write_info, icc_name, 0, profile_data, profile_len);
            delete[] profile_data;
            
            // Write header
            png_write_info(write_ptr, write_info);
            
            // Use png_chunk_benign_error (simulate benign error condition)
            if (size < 200) {
                png_chunk_benign_error(png_ptr, "Input size small but continuing");
            }
            
            png_destroy_write_struct(&write_ptr, &write_info);
        }
    }
    
    // Use png_get_x_offset_pixels
    png_int_32 x_offset = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)x_offset; // Use variable to avoid unused warning
    
    // Use png_get_sCAL
    int unit;
    double scal_width, scal_height;
    png_uint_32 scal_result = png_get_sCAL(png_ptr, info_ptr, &unit, &scal_width, &scal_height);
    (void)scal_result; // Use variable to avoid unused warning
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}