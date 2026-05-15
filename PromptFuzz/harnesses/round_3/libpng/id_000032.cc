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
//<ID> 676
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 1
//<Quality> {"density":22,"unique_branches":{"png_get_valid":[[29,11,29,32,0,0,4,1]]},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_user_limits","png_init_io","png_read_info","png_get_image_width","png_get_image_height","png_get_rgb_to_gray_status","png_set_invert_alpha","png_get_valid","png_set_hIST","png_set_compression_window_bits","png_process_data","png_get_rows","png_get_valid","png_get_next_frame_fcTL","png_image_free","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_create_info_struct","png_set_longjmp_fn","png_set_user_limits","png_init_io","png_read_info","png_get_image_width","png_get_image_height","png_get_rgb_to_gray_status","png_set_invert_alpha","png_get_valid","png_set_hIST","png_set_compression_window_bits","png_process_data","png_get_rows","png_get_valid","png_get_next_frame_fcTL","png_image_free","png_destroy_read_struct"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<547) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegralArray(uint16_t, fuzz_uint16_t_array_5, fuzz_uint16_t_array_size_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
	
		const png_uint_16 * fuzzer_var_png_set_hIST_15_2 = fuzz_uint16_t_array_5;//fuzzer shim end}




    if (size < 8) {
        return 0;
    }

    // Create a FILE pointer to read from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize libpng structures
    png_struct *png_ptr = png_create_read_struct(
        fuzz_str_1,
        nullptr,
        nullptr,
        nullptr
    );
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

    // Set up error handling with longjmp
    jmp_buf *jmp_context = png_set_longjmp_fn(png_ptr, longjmp, sizeof(jmp_buf));
    if (setjmp(*jmp_context)) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set user limits for security
    png_set_user_limits(png_ptr, fuzz_uint32_t_2, fuzz_uint32_t_3);

    // Initialize I/O
    png_init_io(png_ptr, in_file);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);

    // Get RGB to gray status
    png_byte rgb_to_gray_status = png_get_rgb_to_gray_status(png_ptr);

    // Set various transformations
    png_set_invert_alpha(png_ptr);

    // Set dummy histogram (for demonstration)
    png_uint_16 hist[256] = {0};
    if (png_get_valid(png_ptr, info_ptr, fuzz_uint32_t_4)) {
        png_set_hIST(png_ptr, info_ptr, fuzzer_var_png_set_hIST_15_2);
    }

    // Set compression window bits
    png_set_compression_window_bits(png_ptr, fuzz_int32_t_6);

    // Process data progressively
    png_byte buffer[4096];
    size_t bytes_remaining = size;
    size_t bytes_processed = 0;
    
    while (bytes_processed < size) {
        size_t chunk_size = (bytes_remaining > sizeof(buffer)) ? 
                           sizeof(buffer) : bytes_remaining;
        memcpy(buffer, data + bytes_processed, chunk_size);
        png_process_data(png_ptr, info_ptr, buffer, chunk_size);
        bytes_processed += chunk_size;
        bytes_remaining -= chunk_size;
    }

    // Get row pointers (after image is read)
    png_byte **row_pointers = png_get_rows(png_ptr, info_ptr);

    // Try to get APNG frame control (if APNG)
    png_uint_32 frame_width, frame_height, x_offset, y_offset;
    png_uint_16 delay_num, delay_den;
    png_byte dispose_op, blend_op;
    
    if (png_get_valid(png_ptr, info_ptr, fuzz_uint32_t_7)) {
        png_get_next_frame_fcTL(
            png_ptr,
            info_ptr,
            &frame_width,
            &frame_height,
            &x_offset,
            &y_offset,
            &delay_num,
            &delay_den,
            &dispose_op,
            &blend_op
        );
    }

    // Create and free a png_image structure
    png_image image_struct;
    memset(&image_struct, 0, sizeof(png_image));
    image_struct.version = PNG_IMAGE_VERSION;
    
    // Clean up
    png_image_free(&image_struct);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}