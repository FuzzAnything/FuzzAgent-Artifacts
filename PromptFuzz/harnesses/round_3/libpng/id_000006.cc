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
//<ID> 105
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"png_set_read_status_fn":[[945,8,945,23,0,0,4,1]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_read_status_fn","png_read_info","png_get_signature","png_get_num_plays","png_get_y_offset_pixels","png_get_next_frame_dispose_op","png_read_png","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_read_status_fn","png_read_info","png_get_signature","png_get_num_plays","png_get_y_offset_pixels","png_get_next_frame_dispose_op","png_read_png","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to read and process PNG data:

*/


// Custom read status callback function
static void  id_000006_read_status_callback(png_struct *png_ptr, png_uint_32 row, int pass) {
    // This callback can be used to track reading progress
    // For fuzzing, we don't need to do anything specific
    (void)png_ptr;
    (void)row;
    (void)pass;
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    if (size < 8) {
        return 0;  // Too small to be a PNG
    }

    // Check PNG signature
    if (png_sig_cmp((png_const_bytep)data, fuzz_uint64_t_1, 8) != 0) {
        return 0;  // Not a PNG file
    }

    // Create FILE* from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize PNG structures
    png_struct *png_ptr = png_create_read_struct(fuzz_str_2, 
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

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set up PNG reading
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, fuzz_int32_t_3);

    // Set read status function (API #1)
    png_set_read_status_fn(png_ptr,  id_000006_read_status_callback);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Get PNG signature (API #3)
    const png_byte *signature = png_get_signature(png_ptr, info_ptr);
    (void)signature;  // Use signature to avoid unused variable warning

    // Get number of plays for APNG (API #2)
    png_uint_32 num_plays = png_get_num_plays(png_ptr, info_ptr);
    (void)num_plays;

    // Get y-offset in pixels (API #4)
    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
    (void)y_offset_pixels;

    // Get next frame dispose op for APNG (API #5)
    png_byte dispose_op = png_get_next_frame_dispose_op(png_ptr, info_ptr);
    (void)dispose_op;

    // Read the entire PNG image (API #6)
    // Use PNG_TRANSFORM_IDENTITY (0) for no transformations
    png_read_png(png_ptr, info_ptr, fuzz_int32_t_4, nullptr);

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}