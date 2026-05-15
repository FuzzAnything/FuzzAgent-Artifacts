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
//<ID> 216
//<Prompt> []
/*<Combination>: [
*/
//<score> 62, nr_unique_branch: 2
//<Quality> {"density":31,"unique_branches":{"png_set_sRGB_gAMA_and_cHRM":[[847,8,847,23,0,0,4,1],[847,27,847,43,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_read_info","png_get_num_frames","png_set_sRGB_gAMA_and_cHRM","png_get_next_frame_dispose_op","png_create_write_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_write_struct","png_destroy_read_struct","png_set_benign_errors","png_set_longjmp_fn","png_destroy_write_struct","png_destroy_read_struct","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_init_io","png_set_IHDR","png_set_invert_mono","png_write_info","png_write_frame_tail","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_num_frames","png_set_sRGB_gAMA_and_cHRM","png_get_next_frame_dispose_op","png_create_write_struct","png_create_info_struct","png_set_benign_errors","png_set_longjmp_fn","png_get_image_width","png_get_image_height","png_get_bit_depth","png_get_color_type","png_init_io","png_set_IHDR","png_set_invert_mono","png_write_info","png_write_frame_tail","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    if (size < 8) {
        return 0;
    }

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

    // Read structures
    png_structp png_read_ptr = png_create_read_struct(
        fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_read_ptr) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_infop read_info_ptr = png_create_info_struct(png_read_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up longjmp error handling
    jmp_buf *jmp_buf_ptr = png_set_longjmp_fn(
        png_read_ptr, longjmp, sizeof(jmp_buf));
    if (setjmp(*jmp_buf_ptr)) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize IO for reading
    png_init_io(png_read_ptr, in_file);
    png_set_sig_bytes(png_read_ptr, fuzz_int32_t_2);
    png_read_info(png_read_ptr, read_info_ptr);

    // Get image properties
    png_uint_32 num_frames = png_get_num_frames(png_read_ptr, read_info_ptr);
    if (num_frames > 0) {
        png_set_sRGB_gAMA_and_cHRM(png_read_ptr, read_info_ptr, 
                                   fuzz_int32_t_3);
        png_byte dispose_op = png_get_next_frame_dispose_op(
            png_read_ptr, read_info_ptr);
        (void)dispose_op; // Use variable to avoid unused warning
    }

    // Write structures
    png_structp png_write_ptr = png_create_write_struct(
        fuzz_str_4, nullptr, nullptr, nullptr);
    if (!png_write_ptr) {
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    png_infop write_info_ptr = png_create_info_struct(png_write_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&png_write_ptr, nullptr);
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up error handling for write struct
    png_set_benign_errors(png_write_ptr, fuzz_int32_t_5);
    
    if (setjmp(png_jmpbuf(png_write_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
        png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get image dimensions and format
    png_uint_32 width = png_get_image_width(png_read_ptr, read_info_ptr);
    png_uint_32 height = png_get_image_height(png_read_ptr, read_info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_read_ptr, read_info_ptr);
    png_byte color_type = png_get_color_type(png_read_ptr, read_info_ptr);

    // Set up output image with same properties
    png_init_io(png_write_ptr, out_file);
    png_set_IHDR(png_write_ptr, write_info_ptr, width, height, 
                 bit_depth, color_type, fuzz_int32_t_6, 
                 fuzz_int32_t_7, fuzz_int32_t_8);

    // Apply transformation
    png_set_invert_mono(png_write_ptr);

    // Write image data
    png_write_info(png_write_ptr, write_info_ptr);

    // For APNG frames
    if (num_frames > 0) {
        png_write_frame_tail(png_write_ptr, write_info_ptr);
    }

    png_write_end(png_write_ptr, write_info_ptr);

    // Cleanup
    png_destroy_write_struct(&png_write_ptr, &write_info_ptr);
    png_destroy_read_struct(&png_read_ptr, &read_info_ptr, nullptr);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}