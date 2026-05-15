// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_y_pixels_per_inch at pngget.c:369:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
// png_get_x_pixels_per_inch at pngget.c:363:1 in png.h
// png_get_pixels_per_inch at pngget.c:357:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_y_pixels_per_meter at pngget.c:146:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
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
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "code/png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - suppress error output during fuzzing
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - suppress warning output during fuzzing
    (void)png_ptr;
    (void)warning_msg;
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return nullptr;
    }
    return png_ptr;
}

static png_infop create_png_info(png_structp png_ptr) {
    if (!png_ptr) {
        return nullptr;
    }
    return png_create_info_struct(png_ptr);
}

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Write fuzz data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the dummy file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return 0;
    }

    // Create PNG read structure
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    // Create PNG info structure
    png_infop info_ptr = create_png_info(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        // PNG error occurred - clean up and exit
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Initialize PNG I/O
    png_init_io(png_ptr, fp);

    // Read PNG header
    png_read_info(png_ptr, info_ptr);

    // Call all target API functions
    png_uint_32 result;
    
    result = png_get_y_pixels_per_meter(png_ptr, info_ptr);
    (void)result;
    
    result = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)result;
    
    result = png_get_y_pixels_per_inch(png_ptr, info_ptr);
    (void)result;
    
    result = png_get_pixels_per_meter(png_ptr, info_ptr);
    (void)result;
    
    result = png_get_x_pixels_per_inch(png_ptr, info_ptr);
    (void)result;
    
    result = png_get_pixels_per_inch(png_ptr, info_ptr);
    (void)result;

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);

    return 0;
}