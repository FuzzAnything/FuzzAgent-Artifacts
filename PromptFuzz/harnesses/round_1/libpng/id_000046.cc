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
//<ID> 962
//<Prompt> []
/*<Combination>: [
*/
//<score> 87, nr_unique_branch: 13
//<Quality> {"density":29,"unique_branches":{"png_set_text_compression_strategy":[[1309,8,1309,23,0,0,4,1]],"png_get_cHRM_fixed":[[636,8,636,23,0,0,4,0],[636,27,636,43,0,0,4,0],[637,8,637,46,0,0,4,0],[637,8,637,46,0,0,4,1],[639,11,639,25,0,0,4,0],[640,11,640,25,0,0,4,0],[641,11,641,25,0,0,4,0],[642,11,642,25,0,0,4,0],[643,11,643,25,0,0,4,0],[644,11,644,25,0,0,4,0],[645,11,645,25,0,0,4,0],[646,11,646,25,0,0,4,0]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_cHRM_fixed","png_get_x_offset_inches_fixed","png_get_y_pixels_per_inch","png_create_write_struct_2","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_set_text_compression_strategy","png_set_oFFs","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_set_IHDR","png_write_info","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_cHRM_fixed","png_get_x_offset_inches_fixed","png_get_y_pixels_per_inch","png_create_write_struct_2","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_text_compression_strategy","png_set_oFFs","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_get_interlace_type","png_get_compression_type","png_set_IHDR","png_write_info","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for PNG operations

    // Create FILE* for reading input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	FUZZ_FILENAME_PUSH("output_file")
	

    // Create FILE* for writing output (if needed)
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG read structure
    png_struct* png_ptr_read = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_ptr_read) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG info structure
    png_info* info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr_read))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG read
    png_init_io(png_ptr_read, in_file);
    png_read_info(png_ptr_read, info_ptr_read);

    // Get colorimetric data (cHRM chunk) - REQUIRED API 1
    png_fixed_point int_white_x, int_white_y;
    png_fixed_point int_red_x, int_red_y;
    png_fixed_point int_green_x, int_green_y;
    png_fixed_point int_blue_x, int_blue_y;
    png_uint_32 chrm_result = png_get_cHRM_fixed(
        png_ptr_read, info_ptr_read,
        &int_white_x, &int_white_y,
        &int_red_x, &int_red_y,
        &int_green_x, &int_green_y,
        &int_blue_x, &int_blue_y
    );

    // Get X offset in inches (fixed point) - REQUIRED API 2
    png_fixed_point x_offset_inches = png_get_x_offset_inches_fixed(png_ptr_read, info_ptr_read);

    // Get Y pixels per inch - REQUIRED API 3
    png_uint_32 y_pixels_per_inch = png_get_y_pixels_per_inch(png_ptr_read, info_ptr_read);

    // Parse raw data using uint16 and uint32 extraction functions - REQUIRED APIs 4 & 5
    png_uint_16 sample_uint16 = png_get_uint_16(data);
    png_uint_32 sample_uint32 = png_get_uint_32(data);

    // Create write structure for setting parameters
    png_struct* png_ptr_write = png_create_write_struct_2(
        fuzz_str_2, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create info structure for writing
    png_info* info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, nullptr);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set error handling for write
    if (setjmp(png_jmpbuf(png_ptr_write))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG write
    png_init_io(png_ptr_write, out_file);

    // Set text compression strategy - REQUIRED API 6
    // Use sample_uint16 mod 4 to get valid strategy values (0-3)
    int strategy = sample_uint16 % 4;
    png_set_text_compression_strategy(png_ptr_write, strategy);

    // Set image offsets (oFFs chunk) - REQUIRED API 7
    // Use sample values derived from input data
    png_int_32 offset_x = static_cast<png_int_32>(sample_uint32 % 1000);
    png_int_32 offset_y = static_cast<png_int_32>((sample_uint32 >> 16) % 1000);
    int unit_type = 0;  // 0 = pixels
    png_set_oFFs(png_ptr_write, info_ptr_write, offset_x, offset_y, fuzz_int32_t_3);

    // Copy basic image parameters from read to write
    png_uint_32 width = png_get_image_width(png_ptr_read, info_ptr_read);
    png_uint_32 height = png_get_image_height(png_ptr_read, info_ptr_read);
    int bit_depth = png_get_bit_depth(png_ptr_read, info_ptr_read);
    int color_type = png_get_color_type(png_ptr_read, info_ptr_read);
    int interlace_type = png_get_interlace_type(png_ptr_read, info_ptr_read);
    int compression_type = png_get_compression_type(png_ptr_read, info_ptr_read);
    int filter_type = 0;  // Default filter method

    png_set_IHDR(png_ptr_write, info_ptr_write, width, height, bit_depth,
                 color_type, interlace_type, compression_type, fuzz_int32_t_4);

    // Write PNG info
    png_write_info(png_ptr_write, info_ptr_write);

    // Clean up resources
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file to avoid accumulating files
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}