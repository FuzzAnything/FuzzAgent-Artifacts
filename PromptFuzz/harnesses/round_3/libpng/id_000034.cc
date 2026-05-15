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
//<ID> 715
//<Prompt> []
/*<Combination>: [
*/
//<score> 150, nr_unique_branch: 28
//<Quality> {"density":15,"unique_branches":{"png_ensure_fcTL_is_valid":[[1434,8,1434,18,0,0,4,1],[1434,22,1434,45,0,0,4,1],[1436,8,1436,19,0,0,4,1],[1436,23,1436,47,0,0,4,1],[1438,8,1438,34,0,0,4,1],[1438,38,1438,64,0,0,4,1],[1440,8,1440,53,0,0,4,0]],"png_set_compression_strategy":[[1221,8,1221,23,0,0,4,1]],"png_get_bKGD":[[507,8,507,23,0,0,4,0],[507,27,507,43,0,0,4,0],[508,8,508,46,0,0,4,0],[508,8,508,46,0,0,4,1],[509,8,509,26,0,0,4,0]],"png_get_sCAL_fixed":[[1029,8,1029,23,0,0,4,0],[1029,27,1029,43,0,0,4,0],[1030,8,1030,46,0,0,4,0],[1030,8,1030,46,0,0,4,1]],"png_set_packswap":[[73,8,73,23,0,0,4,1],[76,8,76,30,0,0,4,0]],"png_get_user_chunk_ptr":[[1274,12,1274,19,0,0,4,0]],"png_fixed_error":[[520,8,520,20,0,0,4,0],[521,14,521,42,0,0,4,0],[521,46,521,60,0,0,4,0],[521,46,521,60,0,0,4,1]],"png_fixed":[[2702,8,2702,23,0,0,4,0]],"png_set_next_frame_fcTL":[[1388,8,1388,23,0,0,4,1],[1388,27,1388,43,0,0,4,1],[1402,8,1402,42,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_sCAL_fixed","png_get_pixel_aspect_ratio","png_get_bKGD","png_get_user_chunk_ptr","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_destroy_write_struct","png_destroy_read_struct","png_init_io","png_set_compression_strategy","png_set_alpha_mode","png_set_packswap","png_set_next_frame_fcTL","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_sCAL_fixed","png_get_pixel_aspect_ratio","png_get_bKGD","png_get_user_chunk_ptr","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_compression_strategy","png_set_alpha_mode","png_set_packswap","png_set_next_frame_fcTL","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/*Looking at the provided libpng APIs, I need to create a fuzzer that processes PNG data and uses the specified APIs. The event that could be achieved is reading a PNG file with APNG (Animated PNG) support, where we need to handle frame control, compression, scaling, alpha, and other properties.

Here's a fuzz driver that uses all the specified APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    if (size < 100) {
        return 0;  // Too small to be a valid PNG
    }

    // Create memory file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create PNG read structure
    png_struct *png_ptr = png_create_read_struct(fuzz_str_1, nullptr, 
                                                  nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create PNG info structure
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, in_file);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // 1. Get sCAL chunk (physical dimensions) - fixed point version
    int unit = 0;
    png_fixed_point width_scaled = 0, height_scaled = 0;
    png_uint_32 scal_result = png_get_sCAL_fixed(png_ptr, info_ptr, &unit, 
                                                &width_scaled, &height_scaled);

    // 2. Get pixel aspect ratio (floating point version)
    float aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);

    // 3. Get background color
    png_color_16 *background = nullptr;
    png_uint_32 bkgd_result = png_get_bKGD(png_ptr, info_ptr, &background);

    // 4. Get user chunk pointer (for custom chunks)
    void *user_chunk_ptr = png_get_user_chunk_ptr(png_ptr);

    // 5. Convert first 4 bytes of data to uint32 using png_get_uint_32
    png_uint_32 first_four_bytes = png_get_uint_32(data);

    // Now create a write structure to test write operations
    png_struct *write_ptr = png_create_write_struct(fuzz_str_2, 
                                                    nullptr, nullptr, nullptr);
    if (!write_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    png_info *write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (!out_file) {
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_init_io(write_ptr, out_file);

    // 6. Set compression strategy (use data to determine strategy)
    int strategy = data[0] % 5;  // 0-4 for different compression strategies
    png_set_compression_strategy(write_ptr, strategy);

    // 7. Set alpha mode (use data to determine mode)
    int alpha_mode = data[1] % 4;  // 0-3 for different alpha modes
    double output_gamma = (data[2] / 255.0) * 2.0 + 0.5;  // 0.5-2.5 range
    png_set_alpha_mode(write_ptr, alpha_mode, output_gamma);

    // 8. Set packswap (byte ordering)
    png_set_packswap(write_ptr);

    // 9. Set next frame fcTL (for APNG animation)
    // Extract parameters from input data
    png_uint_32 width = (png_get_uint_32(data) % 1000) + 100;
    png_uint_32 height = (png_get_uint_32(data + 4) % 1000) + 100;
    png_uint_32 x_offset = png_get_uint_32(data + 8) % width;
    png_uint_32 y_offset = png_get_uint_32(data + 12) % height;
    png_uint_16 delay_num = (png_get_uint_16(data + 16) % 100) + 1;
    png_uint_16 delay_den = (png_get_uint_16(data + 18) % 100) + 1;
    png_byte dispose_op = data[20] % 4;
    png_byte blend_op = data[21] % 2;

    png_set_next_frame_fcTL(write_ptr, write_info, width, height, 
                           x_offset, y_offset, delay_num, delay_den, 
                           dispose_op, blend_op);

    // Clean up
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file if it was created
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}