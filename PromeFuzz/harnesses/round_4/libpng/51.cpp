// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_next_frame_height at pngget.c:1439:1 in png.h
// png_get_num_plays at pngget.c:1389:1 in png.h
// png_get_num_frames at pngget.c:1379:1 in png.h
// png_get_next_frame_y_offset at pngget.c:1459:1 in png.h
// png_get_next_frame_width at pngget.c:1429:1 in png.h
// png_get_next_frame_x_offset at pngget.c:1449:1 in png.h
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

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Simple dummy read function that returns zeros
    memset(data, 0, length);
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Initialize png_struct and png_info
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Set up custom read function
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);
    
    // Read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Call target API functions
    (void)png_get_next_frame_height(png_ptr, info_ptr);
    (void)png_get_num_plays(png_ptr, info_ptr);
    (void)png_get_num_frames(png_ptr, info_ptr);
    (void)png_get_next_frame_y_offset(png_ptr, info_ptr);
    (void)png_get_next_frame_width(png_ptr, info_ptr);
    (void)png_get_next_frame_x_offset(png_ptr, info_ptr);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}