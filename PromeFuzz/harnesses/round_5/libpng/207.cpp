// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cICP at pngset.c:136:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdio>

#include "code/png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {}
static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {}

extern "C" int LLVMFuzzerTestOneInput_207(const uint8_t *Data, size_t Size) {
    // Create dummy PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Parse fuzz data to extract parameters
    size_t offset = 0;
    
    // Extract doubles for cHRM functions (9 doubles for XYZ, 8 for regular)
    double params_double[18] = {0};
    size_t max_doubles = sizeof(params_double) / sizeof(params_double[0]);
    for (size_t i = 0; i < max_doubles && offset + sizeof(double) <= Size; i++) {
        memcpy(&params_double[i], Data + offset, sizeof(double));
        offset += sizeof(double);
    }
    
    // Extract fixed-point values
    png_fixed_point params_fixed[18] = {0};
    size_t max_fixed = sizeof(params_fixed) / sizeof(params_fixed[0]);
    for (size_t i = 0; i < max_fixed && offset + sizeof(png_fixed_point) <= Size; i++) {
        memcpy(&params_fixed[i], Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
    }
    
    // Extract bytes for cICP
    png_byte cICP_params[4] = {0};
    for (size_t i = 0; i < 4 && offset < Size; i++) {
        cICP_params[i] = Data[offset++];
    }
    
    // Try different API functions based on available data
    // png_set_cHRM_XYZ (floating point)
    if (max_doubles >= 9) {
        png_set_cHRM_XYZ(png_ptr, info_ptr,
                        params_double[0], params_double[1], params_double[2],
                        params_double[3], params_double[4], params_double[5],
                        params_double[6], params_double[7], params_double[8]);
    }
    
    // png_set_cHRM_XYZ_fixed
    if (max_fixed >= 9) {
        png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
                              params_fixed[0], params_fixed[1], params_fixed[2],
                              params_fixed[3], params_fixed[4], params_fixed[5],
                              params_fixed[6], params_fixed[7], params_fixed[8]);
    }
    
    // png_set_cHRM (floating point)
    if (max_doubles >= 8) {
        png_set_cHRM(png_ptr, info_ptr,
                    params_double[9], params_double[10],  // white_x, white_y
                    params_double[11], params_double[12], // red_x, red_y
                    params_double[13], params_double[14], // green_x, green_y
                    params_double[15], params_double[16]); // blue_x, blue_y
    }
    
    // png_set_cHRM_fixed
    if (max_fixed >= 8) {
        png_set_cHRM_fixed(png_ptr, info_ptr,
                          params_fixed[9], params_fixed[10],  // int_white_x, int_white_y
                          params_fixed[11], params_fixed[12], // int_red_x, int_red_y
                          params_fixed[13], params_fixed[14], // int_green_x, int_green_y
                          params_fixed[15], params_fixed[16]); // int_blue_x, int_blue_y
    }
    
    // png_set_cICP
    png_set_cICP(png_ptr, info_ptr,
                cICP_params[0], cICP_params[1],
                cICP_params[2], cICP_params[3]);
    
    // Now test getter functions
    double out_doubles[18] = {0};
    png_fixed_point out_fixed[18] = {0};
    png_byte out_bytes[4] = {0};
    png_uint_32 ret;
    
    // png_get_cHRM_XYZ
    if (max_doubles >= 9) {
        ret = png_get_cHRM_XYZ(png_ptr, info_ptr,
                              &out_doubles[0], &out_doubles[1], &out_doubles[2],
                              &out_doubles[3], &out_doubles[4], &out_doubles[5],
                              &out_doubles[6], &out_doubles[7], &out_doubles[8]);
    }
    
    // png_get_cHRM_XYZ_fixed
    if (max_fixed >= 9) {
        ret = png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                                    &out_fixed[0], &out_fixed[1], &out_fixed[2],
                                    &out_fixed[3], &out_fixed[4], &out_fixed[5],
                                    &out_fixed[6], &out_fixed[7], &out_fixed[8]);
    }
    
    // png_get_cHRM
    if (max_doubles >= 8) {
        ret = png_get_cHRM(png_ptr, info_ptr,
                          &out_doubles[9], &out_doubles[10],
                          &out_doubles[11], &out_doubles[12],
                          &out_doubles[13], &out_doubles[14],
                          &out_doubles[15], &out_doubles[16]);
    }
    
    // png_get_cHRM_fixed
    if (max_fixed >= 8) {
        ret = png_get_cHRM_fixed(png_ptr, info_ptr,
                                &out_fixed[9], &out_fixed[10],
                                &out_fixed[11], &out_fixed[12],
                                &out_fixed[13], &out_fixed[14],
                                &out_fixed[15], &out_fixed[16]);
    }
    
    // png_get_cICP
    ret = png_get_cICP(png_ptr, info_ptr,
                      &out_bytes[0], &out_bytes[1],
                      &out_bytes[2], &out_bytes[3]);
    
    // Test with file operations if we have enough data
    if (Size > 0) {
        FILE* dummy_file = fopen("./dummy_file", "wb");
        if (dummy_file) {
            // Write some PNG header data
            png_init_io(png_ptr, dummy_file);
            fwrite(Data, 1, Size > 100 ? 100 : Size, dummy_file);
            fclose(dummy_file);
            
            // Try to read it back
            dummy_file = fopen("./dummy_file", "rb");
            if (dummy_file) {
                fclose(dummy_file);
            }
        }
    }
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}