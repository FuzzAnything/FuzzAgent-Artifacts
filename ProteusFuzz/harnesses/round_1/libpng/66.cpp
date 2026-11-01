// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <setjmp.h>

static jmp_buf jmp_env;

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(jmp_env, 1);
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    png_byte buf8[8] = {0};
    png_uint_32 val32 = 0;
    png_int_32 val32_signed = 0;
    unsigned int val16 = 0;

    // Initialize png_struct for functions requiring it
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }

    // Fuzz png_save_uint_16
    if (Size >= 2) {
        val16 = Data[0] | (Data[1] << 8);
        png_save_uint_16(buf8, val16);
        // Also test with max value and potential truncation
        png_save_uint_16(buf8, 65535);
        png_save_uint_16(buf8, 100000);
    }

    // Fuzz png_save_uint_32
    if (Size >= 4) {
        val32 = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
        png_save_uint_32(buf8, val32);
        // Test max uint32
        png_save_uint_32(buf8, 0xFFFFFFFF);
    }

    // Fuzz png_save_int_32
    if (Size >= 4) {
        val32_signed = (png_int_32)val32;
        png_save_int_32(buf8, val32_signed);
        // Test negative
        png_save_int_32(buf8, -1);
        png_save_int_32(buf8, -2147483648);
    }

    // Fuzz png_get_uint_32
    if (Size >= 4) {
        png_uint_32 result = png_get_uint_32(Data);
        (void)result;
    }

    // Fuzz png_get_int_32
    if (Size >= 4) {
        png_int_32 result = png_get_int_32(Data);
        (void)result;
    }

    // Fuzz png_get_uint_31
    // This function may call png_error if value > PNG_UINT_31_MAX.
    // We use setjmp/longjmp to handle the error gracefully.
    if (Size >= 4) {
        if (setjmp(jmp_env) == 0) {
            png_get_uint_31(png_ptr, Data);
        }
        // If longjmp occurs, we continue here safely
    }

    png_destroy_read_struct(&png_ptr, NULL, NULL);

    return 0;
}