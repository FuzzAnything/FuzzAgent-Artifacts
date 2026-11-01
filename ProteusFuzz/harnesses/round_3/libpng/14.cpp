// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_iCCP at pngset.c:859:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_pCAL at pngset.c:482:1 in png.h
// png_set_sCAL_s at pngset.c:598:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
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
#include <setjmp.h>

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Use setjmp to catch libpng errors
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Parse input data to drive fuzzing
    const uint8_t *ptr = Data;
    const uint8_t *end = Data + Size;

    // Helper to read values safely
    auto read_u32 = [&]() -> png_uint_32 {
        if (ptr + 4 > end) return 0;
        png_uint_32 val = *ptr++ | (*ptr++ << 8) | (*ptr++ << 16) | (*ptr++ << 24);
        return val;
    };
    auto read_u16 = [&]() -> png_uint_32 {
        if (ptr + 2 > end) return 0;
        png_uint_32 val = *ptr++ | (*ptr++ << 8);
        return val;
    };
    auto read_i32 = [&]() -> png_int_32 {
        if (ptr + 4 > end) return 0;
        png_int_32 val = *ptr++ | (*ptr++ << 8) | (*ptr++ << 16) | (*ptr++ << 24);
        return val;
    };
    auto read_byte = [&]() -> png_byte {
        if (ptr >= end) return 0;
        return *ptr++;
    };

    // 1. png_set_IHDR
    png_uint_32 width = read_u32();
    png_uint_32 height = read_u32();
    int bit_depth = read_byte();
    int color_type = read_byte();
    int interlace_method = read_byte();
    int compression_method = read_byte();
    int filter_method = read_byte();

    // Limit dimensions to avoid excessive memory allocation
    if (width > 10000) width = 10000;
    if (height > 10000) height = 10000;

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);

    // 2. png_set_gAMA_fixed
    png_fixed_point gamma = read_i32();
    png_set_gAMA_fixed(png_ptr, info_ptr, gamma);

    // 3. png_set_cHRM_XYZ_fixed
    png_fixed_point red_X = read_i32();
    png_fixed_point red_Y = read_i32();
    png_fixed_point red_Z = read_i32();
    png_fixed_point green_X = read_i32();
    png_fixed_point green_Y = read_i32();
    png_fixed_point green_Z = read_i32();
    png_fixed_point blue_X = read_i32();
    png_fixed_point blue_Y = read_i32();
    png_fixed_point blue_Z = read_i32();

    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, red_X, red_Y, red_Z,
                           green_X, green_Y, green_Z,
                           blue_X, blue_Y, blue_Z);

    // 4. png_set_iCCP
    // Extract name and profile from remaining data
    size_t name_len = 0;
    if (ptr < end) name_len = *ptr++;
    if (name_len > 100) name_len = 100; // Limit name length
    if (ptr + name_len > end) name_len = end - ptr;
    std::vector<char> name_vec(name_len + 1, 0);
    if (name_len > 0) {
        memcpy(name_vec.data(), ptr, name_len);
        ptr += name_len;
    }

    int compression_type = read_byte();
    png_uint_32 proflen = read_u32();
    if (proflen > 100000) proflen = 100000; // Limit profile length
    if (ptr + proflen > end) proflen = end - ptr;
    std::vector<png_byte> profile_vec(proflen, 0);
    if (proflen > 0) {
        memcpy(profile_vec.data(), ptr, proflen);
        ptr += proflen;
    }

    png_set_iCCP(png_ptr, info_ptr, name_vec.data(), compression_type,
                 profile_vec.data(), proflen);

    // 5. png_set_pCAL
    size_t purpose_len = 0;
    if (ptr < end) purpose_len = *ptr++;
    if (purpose_len > 100) purpose_len = 100;
    if (ptr + purpose_len > end) purpose_len = end - ptr;
    std::vector<char> purpose_vec(purpose_len + 1, 0);
    if (purpose_len > 0) {
        memcpy(purpose_vec.data(), ptr, purpose_len);
        ptr += purpose_len;
    }

    png_int_32 X0 = read_i32();
    png_int_32 X1 = read_i32();
    int type = read_byte();
    int nparams = read_byte();
    if (nparams > 10) nparams = 10; // Limit params

    size_t units_len = 0;
    if (ptr < end) units_len = *ptr++;
    if (units_len > 50) units_len = 50;
    if (ptr + units_len > end) units_len = end - ptr;
    std::vector<char> units_vec(units_len + 1, 0);
    if (units_len > 0) {
        memcpy(units_vec.data(), ptr, units_len);
        ptr += units_len;
    }

    // Allocate params
    std::vector<char*> params(nparams, nullptr);
    std::vector<std::vector<char>> param_buffers;
    for (int i = 0; i < nparams; ++i) {
        size_t plen = 0;
        if (ptr < end) plen = *ptr++;
        if (plen > 100) plen = 100;
        if (ptr + plen > end) plen = end - ptr;
        param_buffers.emplace_back(plen + 1, 0);
        if (plen > 0) {
            memcpy(param_buffers.back().data(), ptr, plen);
            ptr += plen;
        }
        params[i] = param_buffers.back().data();
    }

    png_set_pCAL(png_ptr, info_ptr, purpose_vec.data(), X0, X1, type, nparams,
                 units_vec.data(), params.data());

    // 6. png_set_sCAL_s
    int unit = read_byte();
    size_t swidth_len = 0;
    if (ptr < end) swidth_len = *ptr++;
    if (swidth_len > 50) swidth_len = 50;
    if (ptr + swidth_len > end) swidth_len = end - ptr;
    std::vector<char> swidth_vec(swidth_len + 1, 0);
    if (swidth_len > 0) {
        memcpy(swidth_vec.data(), ptr, swidth_len);
        ptr += swidth_len;
    }

    size_t sheight_len = 0;
    if (ptr < end) sheight_len = *ptr++;
    if (sheight_len > 50) sheight_len = 50;
    if (ptr + sheight_len > end) sheight_len = end - ptr;
    std::vector<char> sheight_vec(sheight_len + 1, 0);
    if (sheight_len > 0) {
        memcpy(sheight_vec.data(), ptr, sheight_len);
        ptr += sheight_len;
    }

    png_set_sCAL_s(png_ptr, info_ptr, unit, swidth_vec.data(), sheight_vec.data());

    // Cleanup
    cleanup_png(png_ptr, info_ptr);
    return 0;
}