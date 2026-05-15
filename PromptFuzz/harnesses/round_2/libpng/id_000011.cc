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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 27
//<Quality> {"density":12,"unique_branches":{"png_image_memory_read":[[1495,8,1495,23,0,0,4,0],[1498,11,1498,24,0,0,4,0],[1501,14,1501,24,0,0,4,0],[1506,17,1506,31,0,0,4,0],[1506,35,1506,47,0,0,4,0],[1506,35,1506,47,0,0,4,1]],"png_benign_error":[[310,8,310,59,0,0,4,0],[313,14,313,55,0,0,4,0],[314,13,314,37,0,0,4,1]],"png_set_background_fixed":[[131,8,131,37,0,0,4,1],[131,41,131,65,0,0,4,1],[134,8,134,61,0,0,4,1],[147,8,147,24,0,0,4,1]],"png_get_iCCP":[[724,8,724,23,0,0,4,0],[724,27,724,43,0,0,4,0],[725,8,725,46,0,0,4,1]],"png_image_read_header":[[1413,11,1413,29,0,0,4,0]],"png_image_begin_read_from_memory":[[1525,8,1525,21,0,0,4,0],[1525,25,1525,60,0,0,4,0],[1527,11,1527,25,0,0,4,0],[1527,29,1527,37,0,0,4,0],[1529,14,1529,45,0,0,4,0]],"png_set_invalid":[[1952,8,1952,23,0,0,4,0],[1952,27,1952,43,0,0,4,0]],"png_get_next_frame_dispose_op":[[1493,8,1493,23,0,0,4,0],[1493,27,1493,43,0,0,4,0]],"png_image_is_not_sRGB":[[104,4,104,61,7,0,4,0]]},"library_calls":["png_image_begin_read_from_memory","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_benign_error","png_get_next_frame_dispose_op","png_get_iCCP","png_set_cICP","png_set_background_fixed","png_save_uint_32","png_set_invalid","png_free","png_free","png_destroy_read_struct","png_create_read_struct","png_create_info_struct","png_get_next_frame_dispose_op","png_benign_error","png_set_cICP","png_set_background_fixed","png_save_uint_32","png_get_iCCP","png_free","png_free","png_set_invalid","png_destroy_read_struct","png_destroy_read_struct","png_image_free"],"critical_calls":["png_image_begin_read_from_memory","png_create_read_struct","png_create_info_struct","png_get_next_frame_dispose_op","png_benign_error","png_set_cICP","png_set_background_fixed","png_save_uint_32","png_get_iCCP","png_free","png_free","png_set_invalid","png_destroy_read_struct","png_image_free"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    // 1. Begin reading PNG from memory
    int result = png_image_begin_read_from_memory(&image, data, size);
    if (result == 0) {
        // If reading fails, we can still test other APIs with dummy structures
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!png_ptr) {
            return 0;
        }
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            return 0;
        }
        
        // 2. Call png_benign_error
        png_benign_error(png_ptr, "Fuzzing benign error");
        
        // 3. Try to get next frame dispose op (even though we might not have APNG)
        png_byte dispose_op = png_get_next_frame_dispose_op(png_ptr, info_ptr);
        
        // 4. Try to get ICCP profile (if any)
        char *name = nullptr;
        int compression_type = 0;
        png_byte *profile = nullptr;
        png_uint_32 proflen = 0;
        png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile, &proflen);
        
        // 5. Set CICP parameters
        png_set_cICP(png_ptr, info_ptr, 1, 2, 3, 0);
        
        // 6. Set background color (fixed point version)
        png_color_16 background_color;
        background_color.index = 0;
        background_color.red = 65535;
        background_color.green = 65535;
        background_color.blue = 65535;
        background_color.gray = 65535;
        png_set_background_fixed(png_ptr, &background_color, PNG_BACKGROUND_GAMMA_SCREEN, 0, 4545);
        
        // 7. Save uint32 to buffer
        png_byte buf[4];
        png_save_uint_32(buf, 0x12345678);
        
        // 8. Mark some info as invalid
        png_set_invalid(png_ptr, info_ptr, PNG_INFO_gAMA);
        
        // Clean up
        if (name) png_free(png_ptr, name);
        if (profile) png_free(png_ptr, profile);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        
        return 0;
    }
    
    // If we successfully started reading the image
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    
    // Try to get the underlying png_struct for more operations
    if (image.opaque) {
        // Create new read structures for additional testing
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (png_ptr) {
            info_ptr = png_create_info_struct(png_ptr);
            if (info_ptr) {
                // 1. Get next frame dispose op (for APNG)
                png_byte dispose_op = png_get_next_frame_dispose_op(png_ptr, info_ptr);
                
                // 2. Call benign error
                png_benign_error(png_ptr, "Fuzzing: processing PNG image");
                
                // 3. Set CICP color space
                png_set_cICP(png_ptr, info_ptr, 1, 2, 3, 0);
                
                // 4. Set background
                png_color_16 background;
                background.red = 32768;
                background.green = 32768;
                background.blue = 32768;
                background.gray = 32768;
                background.index = 0;
                png_set_background_fixed(png_ptr, &background, PNG_BACKGROUND_GAMMA_SCREEN, 0, 4545);
                
                // 5. Save uint32 to buffer
                png_byte buffer[4];
                png_save_uint_32(buffer, size & 0xFFFFFFFF);
                
                // 6. Try to get ICCP profile
                char *name = nullptr;
                int compression_type = 0;
                png_byte *profile = nullptr;
                png_uint_32 proflen = 0;
                png_get_iCCP(png_ptr, info_ptr, &name, &compression_type, &profile, &proflen);
                
                if (name) png_free(png_ptr, name);
                if (profile) png_free(png_ptr, profile);
                
                // 7. Mark some info as invalid
                png_set_invalid(png_ptr, info_ptr, PNG_INFO_tRNS);
                
                png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            } else {
                png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            }
        }
    }
    
    // Clean up image structure
    png_image_free(&image);
    
    return 0;
}