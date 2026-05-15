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
//<ID> 109
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_set_read_fn","png_set_read_status_fn","png_read_info","png_get_signature","png_sig_cmp","png_destroy_read_struct","png_read_png","png_get_num_plays","png_get_y_offset_pixels","png_get_next_frame_dispose_op","png_destroy_read_struct"],"critical_calls":["png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_read_fn","png_set_read_status_fn","png_read_info","png_get_signature","png_sig_cmp","png_read_png","png_get_num_plays","png_get_y_offset_pixels","png_get_next_frame_dispose_op","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to read and process PNG data:

*/


extern "C" {
}

// Custom read function for memory buffer
static void  id_000008_user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    FILE* fp = (FILE*)io_ptr;
    fread(data, 1, length, fp);
}

// Read status callback function
static void  id_000008_read_row_callback(png_structp png_ptr, png_uint_32 row, int pass) {
    // Callback for read status - can be empty for fuzzing
    (void)png_ptr;
    (void)row;
    (void)pass;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // Need at least PNG signature
    
    // Create FILE* from memory buffer
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, 
                                                 nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
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
    
    // Set up custom read function
    png_set_read_fn(png_ptr, (png_voidp)in_file,  id_000008_user_read_data);
    
    // Set read status callback (API #1)
    png_set_read_status_fn(png_ptr,  id_000008_read_row_callback);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Get PNG signature (API #3)
    const png_byte* signature = png_get_signature(png_ptr, info_ptr);
    if (signature) {
        // Verify it's a PNG file
        if (png_sig_cmp(signature, fuzz_uint64_t_2, 8) != 0) {
            // Not a valid PNG signature
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Read the entire PNG image (API #6)
    png_read_png(png_ptr, info_ptr, fuzz_int32_t_3, nullptr);
    
    // Get number of plays for APNG (API #2)
    png_uint_32 num_plays = png_get_num_plays(png_ptr, info_ptr);
    (void)num_plays; // Suppress unused warning
    
    // Get Y offset in pixels (API #4)
    png_int_32 y_offset = png_get_y_offset_pixels(png_ptr, info_ptr);
    (void)y_offset; // Suppress unused warning
    
    // Get next frame dispose op for APNG (API #5)
    png_byte dispose_op = png_get_next_frame_dispose_op(png_ptr, info_ptr);
    (void)dispose_op; // Suppress unused warning
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}