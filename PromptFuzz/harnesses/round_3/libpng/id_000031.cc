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
//<ID> 659
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["png_image_begin_read_from_memory","png_create_read_struct","png_image_free","png_create_info_struct","png_destroy_read_struct","png_image_free","png_set_longjmp_fn","png_destroy_read_struct","png_image_free","png_set_read_fn","png_get_io_ptr","png_error","png_get_io_ptr","png_error","png_read_info","png_get_iCCP","png_get_mDCV_fixed","png_get_mDCV","png_get_sRGB","png_get_tIME","png_convert_to_rfc1123_buffer","png_set_progressive_read_fn","png_get_progressive_ptr","png_destroy_read_struct","png_image_free"],"critical_calls":["png_image_begin_read_from_memory","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_set_read_fn","png_error","png_get_io_ptr","png_error","png_get_io_ptr","png_read_info","png_get_iCCP","png_get_mDCV_fixed","png_get_mDCV","png_get_sRGB","png_get_tIME","png_convert_to_rfc1123_buffer","png_set_progressive_read_fn","png_get_progressive_ptr","png_destroy_read_struct","png_image_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size < 8) {
	return 0;
	}
	  // Need at least PNG signature
    
    // Initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    // 1. Use png_image_begin_read_from_memory to start reading image from memory
    if (png_image_begin_read_from_memory(&image, data, size) == 0) {
        return 0;  // Failed to begin read
    }
    
    // Create read structure for low-level access
    png_structp png_ptr = png_create_read_struct(fuzz_str_1, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        png_image_free(&image);
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        png_image_free(&image);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        png_image_free(&image);
        return 0;
    }
    
    // Set up custom read function to read from memory buffer
    struct mem_buffer {
        const uint8_t *data;
        size_t size;
        size_t offset;
    };
    
    mem_buffer buffer_info = {data, size, 0};
    
    png_set_read_fn(png_ptr, &buffer_info, [](png_structp png_ptr, png_bytep out, size_t length) {
        mem_buffer* buf = static_cast<mem_buffer*>(png_get_io_ptr(png_ptr));
        if (buf->offset + length > buf->size) {
            png_error(png_ptr, "Read beyond buffer");
        }
        memcpy(out, buf->data + buf->offset, length);
        buf->offset += length;
    });
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // 2. Extract ICC profile if present
    char* name = nullptr;
    int compression_type = 0;
    png_byte* profile = nullptr;
    png_uint_32 proflen = 0;
    if (png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile, &proflen)) {
        // ICC profile found, we can process it if needed
        // For fuzzing, just accessing is enough
    }
    
    // 3. Extract mastering display color volume (fixed-point version)
    png_fixed_point white_x_fixed, white_y_fixed;
    png_fixed_point red_x_fixed, red_y_fixed;
    png_fixed_point green_x_fixed, green_y_fixed;
    png_fixed_point blue_x_fixed, blue_y_fixed;
    png_uint_32 max_lum_scaled, min_lum_scaled;
    
    if (png_get_mDCV_fixed(png_ptr, info_ptr, &white_x_fixed, &white_y_fixed,
                          &red_x_fixed, &red_y_fixed,
                          &green_x_fixed, &green_y_fixed,
                          &blue_x_fixed, &blue_y_fixed,
                          &max_lum_scaled, &min_lum_scaled)) {
        // mDCV data found in fixed-point format
    }
    
    // 4. Extract mastering display color volume (double version)
    double white_x, white_y;
    double red_x, red_y;
    double green_x, green_y;
    double blue_x, blue_y;
    double max_lum, min_lum;
    
    if (png_get_mDCV(png_ptr, info_ptr, &white_x, &white_y,
                    &red_x, &red_y,
                    &green_x, &green_y,
                    &blue_x, &blue_y,
                    &max_lum, &min_lum)) {
        // mDCV data found in double format
    }
    
    // 5. Get sRGB intent
    int srgb_intent = 0;
    if (png_get_sRGB(png_ptr, info_ptr, &srgb_intent)) {
        // sRGB data found
    }
    
    // 6. Get time information if present and convert to RFC1123
    png_timep mod_time = nullptr;
    if (png_get_tIME(png_ptr, info_ptr, &mod_time)) {
        char rfc1123_buffer[29];
        if (png_convert_to_rfc1123_buffer(rfc1123_buffer, mod_time)) {
            // Successfully converted time to RFC1123 format
        }
    }
    
    // 7. Set up progressive reading and get progressive pointer
    // First, set progressive read callback with dummy functions
    png_set_progressive_read_fn(png_ptr, &image, 
        [](png_structp, png_infop) {},  // info_callback
        [](png_structp, png_bytep, png_uint_32, int) {},  // row_callback
        [](png_structp, png_infop) {}   // end_callback
    );
    
    // Get the progressive pointer back
    void* progressive_ptr = png_get_progressive_ptr(png_ptr);
    (void)progressive_ptr;  // Mark as used
    
    // Create a FILE* for reading if needed (though we're using memory reading)
    FILE* in_file = fmemopen((void*)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (in_file) {
        // We could use file descriptor if needed
        int fd = fuzz_fileno(in_file);
	FUZZ_FD_PUSH(fd)
        (void)fd;  // Mark as used
        assert_file_closed(&in_file);;
    }
	FUZZ_FILENAME_PUSH("output_file")
    
    // Create output file for writing if needed
    FILE* out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
	FUZZ_FD_PUSH(out_fd)
        (void)out_fd;  // Mark as used
        assert_file_closed(&out_file);;
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    png_image_free(&image);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}