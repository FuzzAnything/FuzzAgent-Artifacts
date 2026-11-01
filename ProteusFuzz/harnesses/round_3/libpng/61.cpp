// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_sCAL at pngget.c:1049:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
// png_get_cLLI at pngget.c:808:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
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
#include <png.h>

// Helper to safely read integers from fuzz data
static int read_int(const uint8_t *data, size_t size, size_t &offset) {
    if (offset + sizeof(int) > size) return 0;
    int val;
    memcpy(&val, data + offset, sizeof(int));
    offset += sizeof(int);
    return val;
}

// Helper to safely read doubles from fuzz data
static double read_double(const uint8_t *data, size_t size, size_t &offset) {
    if (offset + sizeof(double) > size) return 0.0;
    double val;
    memcpy(&val, data + offset, sizeof(double));
    offset += sizeof(double);
    return val;
}

// Helper to safely read png_uint_32
static png_uint_32 read_uint32(const uint8_t *data, size_t size, size_t &offset) {
    if (offset + sizeof(png_uint_32) > size) return 0;
    png_uint_32 val;
    memcpy(&val, data + offset, sizeof(png_uint_32));
    offset += sizeof(png_uint_32);
    return val;
}

// Helper to safely read png_fixed_point
static png_fixed_point read_fixed(const uint8_t *data, size_t size, size_t &offset) {
    if (offset + sizeof(png_fixed_point) > size) return 0;
    png_fixed_point val;
    memcpy(&val, data + offset, sizeof(png_fixed_point));
    offset += sizeof(png_fixed_point);
    return val;
}

// Helper to safely read png_int_32
static png_int_32 read_int32(const uint8_t *data, size_t size, size_t &offset) {
    if (offset + sizeof(png_int_32) > size) return 0;
    png_int_32 val;
    memcpy(&val, data + offset, sizeof(png_int_32));
    offset += sizeof(png_int_32);
    return val;
}

// Helper to safely read png_byte
static png_byte read_byte(const uint8_t *data, size_t size, size_t &offset) {
    if (offset + sizeof(png_byte) > size) return 0;
    png_byte val;
    memcpy(&val, data + offset, sizeof(png_byte));
    offset += sizeof(png_byte);
    return val;
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // We do not open a file for reading. Instead, we manually populate the info structure
    // to test the GET functions without relying on a valid PNG file on disk.
    // This avoids the double-free issue and file I/O overhead.

    size_t offset = 0;
    
    // Populate IHDR data manually
    png_uint_32 width = read_uint32(Data, Size, offset);
    png_uint_32 height = read_uint32(Data, Size, offset);
    int bit_depth = read_int(Data, Size, offset);
    int color_type = read_int(Data, Size, offset);
    int interlace = read_int(Data, Size, offset);
    int compression = read_int(Data, Size, offset);
    int filter = read_int(Data, Size, offset);

    // Set IHDR
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, interlace, compression, filter);

    // 1. png_get_valid
    png_uint_32 flag = read_uint32(Data, Size, offset);
    png_get_valid(png_ptr, info_ptr, flag);

    // 2. png_get_IHDR
    png_uint_32 w, h;
    int bd, ct, im, cm, fm;
    png_get_IHDR(png_ptr, info_ptr, &w, &h, &bd, &ct, &im, &cm, &fm);

    // 3. png_get_sCAL
    int unit;
    double sw, sh;
    png_get_sCAL(png_ptr, info_ptr, &unit, &sw, &sh);
    
    // png_get_sCAL_fixed
    png_fixed_point fw, fh;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &fw, &fh);

    // png_get_sCAL_s
    char *sw_str, *sh_str;
    png_get_sCAL_s(png_ptr, info_ptr, &unit, &sw_str, &sh_str);

    // 4. png_get_cLLI
    double max_content, max_frame;
    png_get_cLLI(png_ptr, info_ptr, &max_content, &max_frame);

    // 5. png_get_cHRM
    double wx, wy, rx, ry, gx, gy, bx, by;
    png_get_cHRM(png_ptr, info_ptr, &wx, &wy, &rx, &ry, &gx, &gy, &bx, &by);

    // png_get_cHRM_fixed
    png_fixed_point iwx, iwy, irx, iry, igx, igy, ibx, iby;
    png_get_cHRM_fixed(png_ptr, info_ptr, &iwx, &iwy, &irx, &iry, &igx, &igy, &ibx, &iby);

    // 6. png_get_mDCV_fixed
    png_fixed_point m_wx, m_wy, m_rx, m_ry, m_gx, m_gy, m_bx, m_by;
    png_uint_32 max_lum, min_lum;
    png_get_mDCV_fixed(png_ptr, info_ptr, &m_wx, &m_wy, &m_rx, &m_ry, &m_gx, &m_gy, &m_bx, &m_by, &max_lum, &min_lum);

    // 7. png_get_oFFs
    png_int_32 off_x, off_y;
    int off_unit;
    png_get_oFFs(png_ptr, info_ptr, &off_x, &off_y, &off_unit);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}