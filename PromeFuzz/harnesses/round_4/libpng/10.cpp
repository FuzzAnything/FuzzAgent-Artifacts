// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "png.h"

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Create read structures
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                      nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr_read) {
        remove("./dummy_file");
        return 0;
    }
    
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        remove("./dummy_file");
        return 0;
    }
    
    // Set up error handling for read structure
    if (setjmp(png_jmpbuf(png_ptr_read))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        remove("./dummy_file");
        return 0;
    }
    
    // Create write structures
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                        nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        remove("./dummy_file");
        return 0;
    }
    
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, nullptr);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        remove("./dummy_file");
        return 0;
    }
    
    // Set up error handling for write structure
    if (setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        remove("./dummy_file");
        return 0;
    }
    
    // Try to read the dummy file
    fp = fopen("./dummy_file", "rb");
    if (fp) {
        png_init_io(png_ptr_read, fp);
        png_read_info(png_ptr_read, info_ptr_read);
        fclose(fp);
    }
    
    // Test png_get_cHRM_XYZ_fixed
    png_fixed_point redX, redY, redZ, greenX, greenY, greenZ, blueX, blueY, blueZ;
    png_uint_32 result = png_get_cHRM_XYZ_fixed(png_ptr_read, info_ptr_read,
                                                &redX, &redY, &redZ,
                                                &greenX, &greenY, &greenZ,
                                                &blueX, &blueY, &blueZ);
    
    // Test with NULL pointers
    png_get_cHRM_XYZ_fixed(png_ptr_read, info_ptr_read,
                           nullptr, nullptr, nullptr,
                           nullptr, nullptr, nullptr,
                           nullptr, nullptr, nullptr);
    
    // Test png_set_sRGB_gAMA_and_cHRM
    if (Size > 0) {
        int intent = Data[0] % 4; // PNG sRGB intent values are 0-3
        png_set_sRGB_gAMA_and_cHRM(png_ptr_write, info_ptr_write, intent);
    }
    
    // Test png_set_cHRM_fixed
    if (Size >= 32) {
        png_fixed_point values[8];
        memcpy(values, Data, sizeof(png_fixed_point) * 8);
        png_set_cHRM_fixed(png_ptr_write, info_ptr_write,
                           values[0], values[1], values[2], values[3],
                           values[4], values[5], values[6], values[7]);
    }
    
    // Test png_get_cICP
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    result = png_get_cICP(png_ptr_read, info_ptr_read,
                          &colour_primaries, &transfer_function,
                          &matrix_coefficients, &video_full_range_flag);
    
    // Test png_get_cHRM_fixed
    png_fixed_point white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    result = png_get_cHRM_fixed(png_ptr_read, info_ptr_read,
                                &white_x, &white_y, &red_x, &red_y,
                                &green_x, &green_y, &blue_x, &blue_y);
    
    // Test with NULL pointers
    png_get_cHRM_fixed(png_ptr_read, info_ptr_read,
                       nullptr, nullptr, nullptr, nullptr,
                       nullptr, nullptr, nullptr, nullptr);
    
    // Test png_set_cHRM_XYZ_fixed
    if (Size >= 36) {
        png_fixed_point xyz_values[9];
        memcpy(xyz_values, Data, sizeof(png_fixed_point) * 9);
        png_set_cHRM_XYZ_fixed(png_ptr_write, info_ptr_write,
                               xyz_values[0], xyz_values[1], xyz_values[2],
                               xyz_values[3], xyz_values[4], xyz_values[5],
                               xyz_values[6], xyz_values[7], xyz_values[8]);
    }
    
    // Cleanup
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    remove("./dummy_file");
    
    return 0;
}