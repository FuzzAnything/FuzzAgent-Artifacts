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
//<ID> 638
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 13
//<Quality> {"density":15,"unique_branches":{"png_get_cLLI":[[813,8,813,23,0,0,4,0],[813,27,813,43,0,0,4,0],[814,8,814,46,0,0,4,0],[814,8,814,46,0,0,4,1],[816,11,816,25,0,0,4,0],[817,11,817,26,0,0,4,0]],"png_get_text":[[1161,8,1161,23,0,0,4,0],[1161,27,1161,43,0,0,4,0],[1161,47,1161,69,0,0,4,0],[1161,47,1161,69,0,0,4,1],[1166,11,1166,27,0,0,4,0],[1169,11,1169,27,0,0,4,0],[1175,8,1175,24,0,0,4,0]]},"library_calls":["png_image_begin_read_from_stdio","png_create_read_struct","png_image_free","png_create_info_struct","png_destroy_read_struct","png_image_free","png_set_error_fn","png_set_longjmp_fn","png_destroy_read_struct","png_image_free","png_init_io","png_read_info","png_get_text","png_get_cLLI","png_destroy_read_struct","png_image_free"],"critical_calls":["png_image_begin_read_from_stdio","png_create_read_struct","png_create_info_struct","png_set_error_fn","png_set_longjmp_fn","png_init_io","png_read_info","png_get_text","png_get_cLLI","png_destroy_read_struct","png_image_free"],"visited":0}
/**/


extern "C" {
}

// Custom error handler to prevent crashes on invalid input
static void  id_000029_custom_error_fn(png_structp png_ptr, png_const_charp msg) {
    // Instead of exiting, we longjmp back to our error handler
    png_longjmp(png_ptr, 1);
}

static void  id_000029_custom_warning_fn(png_structp png_ptr, png_const_charp msg) {
    // Warnings are ignored during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size < 2) {
        return 0;  // Need at least 2 bytes for png_get_uint_16
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create and initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // Use png_image_begin_read_from_stdio
    int begin_result = png_image_begin_read_from_stdio(&image, in_file);
    
    if (begin_result) {
        // Create png_struct and png_info for traditional API usage
        png_structp png_ptr = png_create_read_struct(fuzz_str_1, 
                                                     nullptr, nullptr, nullptr);
        if (!png_ptr) {
            png_image_free(&image);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            png_image_free(&image);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }

        // Use png_set_error_fn
        png_set_error_fn(png_ptr, nullptr,  id_000029_custom_error_fn,  id_000029_custom_warning_fn);

        // Set up error handling with setjmp
        if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
            // Error occurred during PNG processing
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            png_image_free(&image);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }

        // Initialize I/O with our file
        png_init_io(png_ptr, in_file);
        
        // Read PNG info
        png_read_info(png_ptr, info_ptr);

        // Use png_get_text
        png_textp text_ptr = nullptr;
        int num_text = 0;
        if (png_get_text(png_ptr, info_ptr, &text_ptr, &num_text)) {
            // Text chunks were found, but we don't need to process them for fuzzing
        }

        // Use png_get_cLLI
        double max_content_light = 0.0;
        double max_frame_avg_light = 0.0;
        png_get_cLLI(png_ptr, info_ptr, &max_content_light, &max_frame_avg_light);

        // Clean up traditional structures
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        
        // Free the image structure
        png_image_free(&image);
    }

    // Use png_get_uint_16 on a portion of the input
    // We'll use the first 2 bytes of the input
    png_uint_16 uint16_value = png_get_uint_16(data);

    // Clean up
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}