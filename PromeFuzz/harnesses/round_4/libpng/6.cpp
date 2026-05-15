// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_y_pixels_per_meter at pngget.c:146:1 in png.h
// png_get_y_pixels_per_inch at pngget.c:369:1 in png.h
// png_get_x_pixels_per_inch at pngget.c:363:1 in png.h
// png_get_pixels_per_inch at pngget.c:357:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "png.h"

static void dummy_png_error(png_structp png_ptr, png_const_charp error_msg) {
    // Prevent libpng from exiting on error
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void dummy_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    // Ignore warnings
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open dummy file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_png_error, dummy_png_warning);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, fp);
    
    // Read PNG header and info
    png_read_info(png_ptr, info_ptr);
    
    // Call target API functions
    png_get_x_pixels_per_meter(png_ptr, info_ptr);
    png_get_y_pixels_per_meter(png_ptr, info_ptr);
    png_get_y_pixels_per_inch(png_ptr, info_ptr);
    png_get_x_pixels_per_inch(png_ptr, info_ptr);
    png_get_pixels_per_inch(png_ptr, info_ptr);
    png_get_pixels_per_meter(png_ptr, info_ptr);
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    
    return 0;
}