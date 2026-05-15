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
//<ID> 221
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 2
//<Quality> {"density":18,"unique_branches":{"png_malloc":[[178,8,178,23,0,0,4,0]],"png_calloc":[[56,8,56,19,0,0,4,1]]},"library_calls":["png_sig_cmp","png_image_begin_read_from_memory","png_calloc","png_image_free","png_create_read_struct","png_free","png_image_free","png_create_info_struct","png_destroy_read_struct","png_free","png_image_free","png_set_crc_action","png_set_rgb_to_gray_fixed","png_set_first_frame_is_hidden","png_set_longjmp_fn","png_destroy_read_struct","png_free","png_image_free","png_destroy_read_struct","png_free","png_image_free","png_init_io","png_set_sig_bytes","png_read_info","png_get_image_width","png_get_image_height","png_image_finish_read","png_destroy_read_struct","png_free","png_image_free","png_image_write_to_file","png_destroy_read_struct","png_free","png_image_free"],"critical_calls":["png_sig_cmp","png_image_begin_read_from_memory","png_calloc","png_create_read_struct","png_create_info_struct","png_set_crc_action","png_set_rgb_to_gray_fixed","png_set_first_frame_is_hidden","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_read_info","png_get_image_width","png_get_image_height","png_image_finish_read","png_image_write_to_file","png_destroy_read_struct","png_free","png_image_free"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libpng APIs to read a PNG from memory, process it, and write it to a file:

*/


// Include necessary libpng headers

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 8) {
        return 0; // Too small to be a valid PNG
    }

    // Check PNG signature
    if (png_sig_cmp((png_bytep)data, 0, 8) != 0) {
        return 0; // Not a PNG file
    }

    // Initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // 1. Begin reading PNG from memory
    if (png_image_begin_read_from_memory(&image, data, size) == 0) {
        return 0; // Failed to read
    }

    // Allocate buffer for image data using png_calloc
    png_alloc_size_t buffer_size = PNG_IMAGE_SIZE(image);
    png_byte *buffer = (png_byte *)png_calloc(NULL, buffer_size);
    if (!buffer) {
        png_image_free(&image);
        return 0;
    }

    // Create png_struct and png_info for additional processing
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        png_free(NULL, buffer);
        png_image_free(&image);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        png_free(NULL, buffer);
        png_image_free(&image);
        return 0;
    }

    // 2. Set CRC action
    png_set_crc_action(png_ptr, PNG_CRC_ERROR_QUIT, PNG_CRC_WARN_USE);

    // 3. Set RGB to gray conversion (fixed point)
    png_set_rgb_to_gray_fixed(png_ptr, 1, 30000, 30000); // Equal weights for RGB to gray

    // 4. Set first frame as hidden (for APNG)
    png_set_first_frame_is_hidden(png_ptr, info_ptr, 0);

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        png_free(NULL, buffer);
        png_image_free(&image);
        return 0;
    }

    // Create FILE* for reading (though we're using memory, we need it for some operations)
    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        png_free(NULL, buffer);
        png_image_free(&image);
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, in_file);
    png_set_sig_bytes(png_ptr, 8); // We already checked the signature

    // Read image info
    png_read_info(png_ptr, info_ptr);

    // Update image dimensions from png_struct
    image.width = png_get_image_width(png_ptr, info_ptr);
    image.height = png_get_image_height(png_ptr, info_ptr);

    // 5. Finish reading the image into buffer
    if (png_image_finish_read(&image, NULL, buffer, 0, NULL) == 0) {
        assert_file_closed(&in_file);;
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        png_free(NULL, buffer);
        png_image_free(&image);
        assert_file_closed(&in_file);
	return 0;
    }
	FUZZ_FILENAME_PUSH("output_file")

    // 6. Write the image to a file
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        // Note: png_image_write_to_stdio is not in the provided list, but we'll use
        // png_image_write_to_file instead (which is in the list)
        png_image_write_to_file(&image, "output_file", 0, buffer, 0, NULL);
        assert_file_closed(&out_file);;
    }

    // Cleanup
    assert_file_closed(&in_file);;
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    png_free(NULL, buffer);
    png_image_free(&image);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}