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
//<ID> 657
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["png_image_begin_read_from_memory","png_image_free","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_read_info","png_get_iCCP","png_get_mDCV_fixed","png_get_mDCV","png_get_sRGB","png_get_progressive_ptr","png_get_tIME","png_convert_to_rfc1123_buffer","png_destroy_read_struct"],"critical_calls":["png_image_begin_read_from_memory","png_image_free","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_read_info","png_get_iCCP","png_get_mDCV_fixed","png_get_mDCV","png_get_sRGB","png_get_progressive_ptr","png_get_tIME","png_convert_to_rfc1123_buffer","png_destroy_read_struct"],"visited":0}
/**/


// Include libpng headers

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Step 1: Use png_image_begin_read_from_memory to start reading image from memory
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    int result = png_image_begin_read_from_memory(&image, data, size);
    
    // If image reading started successfully, we'll proceed with other APIs
    if (result != 0) {
        // Clean up the image structure
        png_image_free(&image);
        
        // Now let's use traditional libpng reading to access other metadata
        
        // Create FILE* from memory for traditional libpng reading
        FILE *in_file = fmemopen((void *)data, size, "rb");
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
        
        // Initialize I/O
        png_init_io(png_ptr, in_file);
        
        // Read PNG info
        png_read_info(png_ptr, info_ptr);
        
        // Now call the required APIs
        
        // 1. png_get_iCCP - get ICC profile
        char *name = nullptr;
        int compression_type = 0;
        png_byte *profile = nullptr;
        png_uint_32 proflen = 0;
        png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile, &proflen);
        
        // 2. png_get_mDCV_fixed - get mastering display color volume (fixed point)
        png_fixed_point int_white_x, int_white_y;
        png_fixed_point int_red_x, int_red_y;
        png_fixed_point int_green_x, int_green_y;
        png_fixed_point int_blue_x, int_blue_y;
        png_uint_32 max_lum_scaled, min_lum_scaled;
        
        png_get_mDCV_fixed(png_ptr, info_ptr, 
                          &int_white_x, &int_white_y,
                          &int_red_x, &int_red_y,
                          &int_green_x, &int_green_y,
                          &int_blue_x, &int_blue_y,
                          &max_lum_scaled, &min_lum_scaled);
        
        // 3. png_get_mDCV - get mastering display color volume (floating point)
        double white_x, white_y;
        double red_x, red_y;
        double green_x, green_y;
        double blue_x, blue_y;
        double max_lum, min_lum;
        
        png_get_mDCV(png_ptr, info_ptr,
                    &white_x, &white_y,
                    &red_x, &red_y,
                    &green_x, &green_y,
                    &blue_x, &blue_y,
                    &max_lum, &min_lum);
        
        // 4. png_get_sRGB - get sRGB intent
        int file_srgb_intent = 0;
        png_get_sRGB(png_ptr, info_ptr, &file_srgb_intent);
        
        // 5. png_get_progressive_ptr - get progressive reader pointer
        // Since we're not using progressive reading in this example, this will return nullptr
        void *progressive_ptr = png_get_progressive_ptr(png_ptr);
        (void)progressive_ptr; // Mark as used to avoid compiler warning
        
        // 6. png_convert_to_rfc1123_buffer - convert time to RFC1123 format
        // First try to get tIME chunk
        png_timep mod_time = nullptr;
        if (png_get_tIME(png_ptr, info_ptr, &mod_time)) {
            char time_buffer[29];
            png_convert_to_rfc1123_buffer(time_buffer, mod_time);
        }
        
        // Clean up libpng structures
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        
        // Free any allocated strings from png_get_iCCP
        if (name) {
            free(name);
        }
        if (profile) {
            free(profile);
        }
    }
    
    return 0;
}