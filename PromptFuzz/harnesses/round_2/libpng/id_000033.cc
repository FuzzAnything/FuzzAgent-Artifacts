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
//<ID> 650
//<Prompt> []
/*<Combination>: [
*/
//<score> 180, nr_unique_branch: 14
//<Quality> {"density":30,"unique_branches":{"png_fp_add":[[1052,11,1052,42,0,0,4,1],[1055,13,1055,24,0,0,4,0],[1057,11,1057,43,0,0,4,0],[1057,11,1057,43,0,0,4,1]],"png_muldiv":[[2746,8,2746,20,0,0,4,1]],"png_xy_from_XYZ":[[1118,8,1118,48,0,0,4,0],[1121,8,1121,62,0,0,4,0],[1127,8,1127,52,0,0,4,0],[1130,8,1130,68,0,0,4,0],[1136,8,1136,50,0,0,4,0],[1139,8,1139,65,0,0,4,0],[1149,8,1149,38,0,0,4,0]],"png_safe_add":[[1099,8,1099,14,0,0,4,1]],"png_set_cHRM_XYZ_fixed":[[87,8,87,39,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_destroy_write_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_destroy_read_struct","png_destroy_write_struct","png_set_compression_method","png_set_gamma_fixed","png_set_cHRM_XYZ_fixed","png_set_iCCP","png_chunk_benign_error","png_convert_from_time_t","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_init_io","png_read_info","png_get_y_offset_inches","png_init_io","png_get_IHDR","png_set_IHDR","png_write_info","png_data_freer","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_set_longjmp_fn","png_set_compression_method","png_set_gamma_fixed","png_set_cHRM_XYZ_fixed","png_set_iCCP","png_chunk_benign_error","png_convert_from_time_t","png_init_io","png_read_info","png_get_y_offset_inches","png_init_io","png_get_IHDR","png_set_IHDR","png_write_info","png_data_freer","png_write_end","png_destroy_read_struct","png_destroy_write_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimal data for PNG operations
    
    // Create read structure
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                      nullptr, nullptr, nullptr);
    if (!png_ptr_read) {
	return 0;
	}
	
    
    // Create info structure for reading
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }
    
    // Create write structure
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                        nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    // Create info structure for writing
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        return 0;
    }
    
    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr_read)) || setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }
    
    // 1. Use png_get_uint_32 to extract values from input data
    png_uint_32 extracted_value = png_get_uint_32(data);
    
    // 2. Set compression method
    png_set_compression_method(png_ptr_write, extracted_value % 9); // 0-8 valid methods
    
    // 3. Set gamma correction
    png_set_gamma_fixed(png_ptr_write, 
                       static_cast<png_fixed_point>(65536 * 2.2), // screen gamma
                       static_cast<png_fixed_point>(65536 * 1.0)); // override file gamma
    
    // 4. Set cHRM XYZ values (using fixed-point from extracted values)
    png_set_cHRM_XYZ_fixed(png_ptr_write, info_ptr_write,
                          static_cast<png_fixed_point>(extracted_value * 100),
                          static_cast<png_fixed_point>(extracted_value * 100),
                          static_cast<png_fixed_point>(extracted_value * 100),
                          static_cast<png_fixed_point>((extracted_value + 100) * 100),
                          static_cast<png_fixed_point>((extracted_value + 100) * 100),
                          static_cast<png_fixed_point>((extracted_value + 100) * 100),
                          static_cast<png_fixed_point>((extracted_value + 200) * 100),
                          static_cast<png_fixed_point>((extracted_value + 200) * 100),
                          static_cast<png_fixed_point>((extracted_value + 200) * 100));
    
    // 5. Create a dummy ICC profile from input data
    const char* icc_name = "sRGB";
    png_uint_32 proflen = size > 2000 ? 2000 : static_cast<png_uint_32>(size);
    png_set_iCCP(png_ptr_write, info_ptr_write, icc_name, 
                PNG_COMPRESSION_TYPE_BASE, data, proflen);
    
    // 6. Report benign error for testing
    png_chunk_benign_error(png_ptr_write, "Test benign error during fuzzing");
    
    // 7. Convert time_t to png_time
    time_t current_time = time(nullptr);
    png_time png_time_struct;
    png_convert_from_time_t(&png_time_struct, current_time);
    
    // 8. Create FILE* for input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 9. Create FILE* for output
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 10. Setup PNG read from memory file
    png_init_io(png_ptr_read, in_file);
    png_read_info(png_ptr_read, info_ptr_read);
    
    // 11. Get y offset in inches (if available)
    float y_offset_inches = png_get_y_offset_inches(png_ptr_read, info_ptr_read);
    (void)y_offset_inches; // Prevent unused warning
    
    // 12. Setup PNG write to output file
    png_init_io(png_ptr_write, out_file);
    
    // Copy basic info from read to write
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    png_get_IHDR(png_ptr_read, info_ptr_read, &width, &height, &bit_depth,
                 &color_type, &interlace_type, &compression_type, &filter_type);
    
    png_set_IHDR(png_ptr_write, info_ptr_write, width, height, bit_depth,
                 color_type, interlace_type, compression_type, filter_type);
    
    // 13. Write the PNG
    png_write_info(png_ptr_write, info_ptr_write);
    
    // 14. Use png_data_freer to free data
    png_data_freer(png_ptr_write, info_ptr_write, PNG_DESTROY_WILL_FREE_DATA, 
                   PNG_FREE_ALL);
    
    // 15. Clean up
    png_write_end(png_ptr_write, info_ptr_write);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 16. Destroy structures
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}