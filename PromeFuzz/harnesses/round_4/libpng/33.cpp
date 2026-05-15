// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_oFFs at pngset.c:465:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
// png_get_y_offset_microns at pngget.c:264:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
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
#include <cstdlib>
#include <cstring>

extern "C" {
#include "png.h"
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function that does nothing
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Dummy flush function that does nothing
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Dummy error handler
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Dummy warning handler
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 100) {
        return 0;
    }

    // Create dummy PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up dummy I/O
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);

    // Initialize some values from fuzz data
    png_int_32 offset_x = 0;
    png_int_32 offset_y = 0;
    int unit_type = 0;
    
    // Use fuzz data to set values
    if (Size >= sizeof(png_int_32) * 2 + sizeof(int)) {
        memcpy(&offset_x, Data, sizeof(png_int_32));
        memcpy(&offset_y, Data + sizeof(png_int_32), sizeof(png_int_32));
        memcpy(&unit_type, Data + sizeof(png_int_32) * 2, sizeof(int));
        
        // Ensure unit_type is valid (0 or 1)
        unit_type = unit_type & 1;
    }

    // Test png_set_oFFs
    png_set_oFFs(png_ptr, info_ptr, offset_x, offset_y, unit_type);

    // Test png_get_oFFs
    png_int_32 retrieved_offset_x = 0;
    png_int_32 retrieved_offset_y = 0;
    int retrieved_unit_type = 0;
    png_uint_32 info_result = png_get_oFFs(png_ptr, info_ptr, &retrieved_offset_x, &retrieved_offset_y, &retrieved_unit_type);

    // Test png_get_x_offset_pixels
    png_int_32 x_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);

    // Test png_get_x_offset_microns
    png_int_32 x_microns = png_get_x_offset_microns(png_ptr, info_ptr);

    // Test png_get_y_offset_microns
    png_int_32 y_microns = png_get_y_offset_microns(png_ptr, info_ptr);

#ifdef PNG_FIXED_POINT_SUPPORTED
    // Test png_get_x_offset_inches_fixed
    png_fixed_point x_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
#endif

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}