// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_sCAL_s at pngset.c:598:1 in png.h
// png_set_mDCV_fixed at pngset.c:225:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_pHYs at pngset.c:723:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <png.h>

static void safe_png_destroy(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Use setjmp to handle PNG errors gracefully
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        safe_png_destroy(png_ptr, info_ptr);
        return 0;
    }

    // Extract data from fuzz input
    const uint8_t *ptr = Data;
    size_t remaining = Size;

    // Helper to read bytes safely
    auto read_byte = [&]() -> uint8_t {
        if (remaining < 1) return 0;
        uint8_t val = *ptr++;
        remaining--;
        return val;
    };

    auto read_int32 = [&]() -> int32_t {
        if (remaining < 4) return 0;
        int32_t val = 0;
        val |= (int32_t)ptr[0] << 24;
        val |= (int32_t)ptr[1] << 16;
        val |= (int32_t)ptr[2] << 8;
        val |= (int32_t)ptr[3];
        ptr += 4;
        remaining -= 4;
        return val;
    };

    auto read_uint32 = [&]() -> uint32_t {
        if (remaining < 4) return 0;
        uint32_t val = 0;
        val |= (uint32_t)ptr[0] << 24;
        val |= (uint32_t)ptr[1] << 16;
        val |= (uint32_t)ptr[2] << 8;
        val |= (uint32_t)ptr[3];
        ptr += 4;
        remaining -= 4;
        return val;
    };

    // Fuzz png_set_sCAL_s
    // We need non-empty strings without leading '-'
    char swidth[32] = {0};
    char sheight[32] = {0};
    
    // Ensure we have enough bytes for width and height strings (at least 4 bytes)
    if (remaining >= 4) {
        // Ensure no leading '-' and non-empty
        uint8_t w1 = ptr[0] & 0x7F; 
        uint8_t w2 = ptr[1] & 0x7F;
        if (w1 == 0) w1 = '1';
        if (w2 == 0) w2 = '0';
        swidth[0] = (char)w1;
        swidth[1] = (char)w2;
        swidth[2] = '\0';

        uint8_t h1 = ptr[2] & 0x7F;
        uint8_t h2 = ptr[3] & 0x7F;
        if (h1 == 0) h1 = '1';
        if (h2 == 0) h2 = '0';
        sheight[0] = (char)h1;
        sheight[1] = (char)h2;
        sheight[2] = '\0';
        
        ptr += 4;
        remaining -= 4;

        // Read unit if available, otherwise default to 1
        int unit = 1; 
        if (remaining >= 1) {
            unit = read_byte() % 3; 
            if (unit == 0) unit = 1; 
        }
        
        png_set_sCAL_s(png_ptr, info_ptr, unit, swidth, sheight);
    }

    // Fuzz png_set_mDCV_fixed
    // Requires 8 fixed point values (4 bytes each) + 2 uint32 (4 bytes each) = 40 bytes
    if (remaining >= 40) {
        png_fixed_point wx = read_int32();
        png_fixed_point wy = read_int32();
        png_fixed_point rx = read_int32();
        png_fixed_point ry = read_int32();
        png_fixed_point gx = read_int32();
        png_fixed_point gy = read_int32();
        png_fixed_point bx = read_int32();
        png_fixed_point by = read_int32();
        png_uint_32 max_lum = read_uint32();
        png_uint_32 min_lum = read_uint32();

        auto clamp_fixed = [](png_fixed_point v) -> png_fixed_point {
            if (v < 0) return 0;
            if (v > 130000) return 130000;
            return v;
        };

        auto clamp_lum = [](png_uint_32 v) -> png_uint_32 {
            if (v > 2000000000u) return 2000000000u;
            return v;
        };

        png_set_mDCV_fixed(png_ptr, info_ptr,
            clamp_fixed(wx), clamp_fixed(wy),
            clamp_fixed(rx), clamp_fixed(ry),
            clamp_fixed(gx), clamp_fixed(gy),
            clamp_fixed(bx), clamp_fixed(by),
            clamp_lum(max_lum), clamp_lum(min_lum));
    }

    // Fuzz png_set_pHYs
    // Requires 4 + 4 + 1 = 9 bytes
    if (remaining >= 9) {
        png_uint_32 res_x = read_uint32();
        png_uint_32 res_y = read_uint32();
        int unit_type = read_byte() % 2; 
        
        if (res_x == 0) res_x = 1;
        if (res_y == 0) res_y = 1;

        png_set_pHYs(png_ptr, info_ptr, res_x, res_y, unit_type);
    }

    // Fuzz png_get_sCAL_fixed
    int unit_out = 0;
    png_fixed_point width_out = 0;
    png_fixed_point height_out = 0;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit_out, &width_out, &height_out);

    // Fuzz png_get_pHYs
    png_uint_32 res_x_out = 0;
    png_uint_32 res_y_out = 0;
    int unit_type_out = 0;
    png_get_pHYs(png_ptr, info_ptr, &res_x_out, &res_y_out, &unit_type_out);

    // Fuzz png_get_pixel_aspect_ratio_fixed
    png_fixed_point aspect_ratio = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);

    // Fuzz png_get_x_offset_pixels
    png_int_32 offset_x = png_get_x_offset_pixels(png_ptr, info_ptr);

    // Cleanup
    safe_png_destroy(png_ptr, info_ptr);

    return 0;
}