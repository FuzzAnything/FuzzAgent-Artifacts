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
//<ID> 816
//<Prompt> []
/*<Combination>: [
*/
//<score> 104, nr_unique_branch: 5
//<Quality> {"density":26,"unique_branches":{"png_get_mem_ptr":[[281,8,281,23,0,0,4,1]],"png_write_png":[[1391,8,1391,23,0,0,4,1],[1391,27,1391,43,0,0,4,1],[1394,8,1394,46,0,0,4,0]],"png_set_text_compression_level":[[1287,8,1287,23,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_cHRM","png_get_mem_ptr","png_get_y_offset_microns","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_get_IHDR","png_set_IHDR","png_set_text_compression_level","png_set_mDCV","png_set_mDCV","png_write_png","png_get_next_frame_delay_num","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_cHRM","png_get_mem_ptr","png_get_y_offset_microns","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_get_IHDR","png_set_IHDR","png_set_text_compression_level","png_set_mDCV","png_write_png","png_get_next_frame_delay_num","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to achieve a comprehensive PNG processing event:

*/


// Include necessary libpng headers

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for PNG processing
    
    // Create input FILE* from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	
    
    // Create output FILE* for writing
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize libpng structures
    png_struct *png_read_ptr = png_create_read_struct(fuzz_str_1, 
                                                      nullptr, nullptr, nullptr);
    if (!png_read_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *read_info_ptr = png_create_info_struct(png_read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_read_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up reading from FILE*
    png_init_io(png_read_ptr, in_file);
    
    // Read PNG info
    png_read_info(png_read_ptr, read_info_ptr);
    
    // Get cHRM chromaticities (if present)
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_uint_32 cHRM_result = png_get_cHRM(png_read_ptr, read_info_ptr, 
                                          &white_x, &white_y, &red_x, &red_y,
                                          &green_x, &green_y, &blue_x, &blue_y);
    
    // Get memory pointer
    void *mem_ptr = png_get_mem_ptr(png_read_ptr);
    
    // Get y-offset in microns
    png_int_32 y_offset_microns = png_get_y_offset_microns(png_read_ptr, read_info_ptr);
    
    // Create write structures
    png_struct *png_write_ptr = png_create_write_struct(fuzz_str_2,
                                                       nullptr, nullptr, nullptr);
    if (!png_write_ptr) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    png_info *write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up error handling for write
    if (setjmp(png_jmpbuf(png_write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set up writing to FILE*
    png_init_io(png_write_ptr, out_file);
    
    // Copy basic info from read to write
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    
    png_get_IHDR(png_read_ptr, read_info_ptr, &width, &height, &bit_depth,
                 &color_type, &interlace_type, &compression_type, &filter_type);
    
    png_set_IHDR(png_write_ptr, write_info_ptr, width, height, bit_depth,
                 color_type, interlace_type, compression_type, filter_type);
    
    // Set text compression level (using a value derived from input data)
    int compression_level = (data[0] % 10) + 1; // 1-10
    png_set_text_compression_level(png_write_ptr, compression_level);
    
    // Set mDCV (Mastering Display Color Volume) using cHRM values or defaults
    if (cHRM_result) {
        // Use the cHRM values we obtained earlier
        png_set_mDCV(png_write_ptr, write_info_ptr,
                    white_x, white_y, red_x, red_y,
                    green_x, green_y, blue_x, blue_y,
                    1000.0, 0.001); // Example luminance values
    } else {
        // Use default values for mDCV
        png_set_mDCV(png_write_ptr, write_info_ptr,
                    0.3127, 0.3290,  // D65 white point
                    0.64, 0.33,      // Red primary
                    0.30, 0.60,      // Green primary
                    0.15, 0.06,      // Blue primary
                    1000.0, 0.001);  // Max/min luminance
    }
    
    // Write the PNG with transformations
    png_write_png(png_write_ptr, write_info_ptr, 
                  fuzz_int32_t_3, nullptr);
    
    // Get next frame delay (for APNG)
    png_uint_16 next_frame_delay_num = png_get_next_frame_delay_num(png_read_ptr, read_info_ptr);
    
    // Cleanup
    png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}