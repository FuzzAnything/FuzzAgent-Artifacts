// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
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
#include <png.h>
#include <cstring>

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error, dummy_warning);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_byte buf[4];
    png_uint_32 val32;
    png_int_32 val32s;
    unsigned int val16;

    // Test png_get_uint_32
    if (Size >= 4) {
        memcpy(buf, Data, 4);
        val32 = png_get_uint_32(buf);
        (void)val32;
    }

    // Test png_get_int_32
    if (Size >= 4) {
        memcpy(buf, Data, 4);
        val32s = png_get_int_32(buf);
        (void)val32s;
    }

    // Test png_save_uint_32
    if (Size >= 4) {
        val32 = png_get_uint_32(Data);
        png_save_uint_32(buf, val32);
    }

    // Test png_save_int_32
    if (Size >= 4) {
        val32s = png_get_int_32(Data);
        png_save_int_32(buf, val32s);
    }

    // Test png_save_uint_16
    if (Size >= 2) {
        val16 = (unsigned int)Data[0] | ((unsigned int)Data[1] << 8);
        png_save_uint_16(buf, val16);
    }

    // Test png_get_uint_31
    if (Size >= 4) {
        memcpy(buf, Data, 4);
        // This may trigger png_error if value > PNG_UINT_31_MAX
        // We wrap in setjmp to catch the longjmp
        if (setjmp(png_jmpbuf(png_ptr)) == 0) {
            png_get_uint_31(png_ptr, buf);
        }
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 0;
}