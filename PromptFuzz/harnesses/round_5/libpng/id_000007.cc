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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"png_set_keep_unknown_chunks":[[1709,11,1709,29,0,0,4,0]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_keep_unknown_chunks","png_read_info","png_get_copyright","png_get_y_pixels_per_inch","png_set_sRGB","png_get_next_frame_height","png_get_cHRM_fixed","png_get_first_frame_is_hidden","png_process_data_pause","png_read_image","png_read_end","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_keep_unknown_chunks","png_read_info","png_get_copyright","png_get_y_pixels_per_inch","png_set_sRGB","png_get_next_frame_height","png_get_cHRM_fixed","png_get_first_frame_is_hidden","png_process_data_pause","png_read_image","png_read_end","png_destroy_read_struct"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to process PNG data:

*/


// Include necessary libpng headers

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	 // PNG signature requires at least 8 bytes

    // Check if input data has PNG signature
    if (png_sig_cmp((png_bytep)data, 0, 8) != 0) {
        return 0; // Not a PNG file
    }

    // Create FILE pointer from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
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

    // Initialize I/O
    png_init_io(png_ptr, in_file);
    
    // Set that we've already read the signature
    png_set_sig_bytes(png_ptr, 8);

    // Set up to keep unknown chunks (API #4)
    png_byte chunk_list[] = {0, 0, 0, 0}; // Empty list means keep all chunks
    png_set_keep_unknown_chunks(png_ptr, PNG_HANDLE_CHUNK_ALWAYS, 
                                chunk_list, 0);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Get and potentially use copyright information (API #1)
    const char *copyright = png_get_copyright(png_ptr);
    (void)copyright; // Use in some way to avoid compiler warning

    // Get Y pixels per inch (API #2)
    png_uint_32 y_pixels_per_inch = png_get_y_pixels_per_inch(png_ptr, info_ptr);
    (void)y_pixels_per_inch;

    // Set sRGB intent (API #3)
    png_set_sRGB(png_ptr, info_ptr, PNG_sRGB_INTENT_PERCEPTUAL);

    // Get next frame height (API #5) - for APNG support
    png_uint_32 next_frame_height = png_get_next_frame_height(png_ptr, info_ptr);
    (void)next_frame_height;

    // Get cHRM fixed values (API #6)
    png_fixed_point int_white_x, int_white_y;
    png_fixed_point int_red_x, int_red_y;
    png_fixed_point int_green_x, int_green_y;
    png_fixed_point int_blue_x, int_blue_y;
    
    png_get_cHRM_fixed(png_ptr, info_ptr,
                       &int_white_x, &int_white_y,
                       &int_red_x, &int_red_y,
                       &int_green_x, &int_green_y,
                       &int_blue_x, &int_blue_y);

    // Get first frame hidden status (API #7)
    png_byte first_frame_hidden = png_get_first_frame_is_hidden(png_ptr, info_ptr);
    (void)first_frame_hidden;

    // Process data with pause (API #8)
    size_t save = 0;
    size_t pause_result = png_process_data_pause(png_ptr, save);
    (void)pause_result;

    // Read the rest of the image data
    png_read_image(png_ptr, nullptr);
    png_read_end(png_ptr, info_ptr);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}