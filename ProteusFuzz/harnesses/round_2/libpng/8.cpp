// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_shift at pngtrans.c:83:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_sBIT at pngget.c:1140:1 in png.h
// png_get_sBIT at pngget.c:1140:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
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

static void dummy_read(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_write(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush(png_structp png_ptr) {
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_color_8 sig_bit = {0, 0, 0, 0, 0};
    png_color_8 true_bits = {0, 0, 0, 0, 0};
    png_color_8 *sig_bit_ptr = nullptr;

    // Initialize libpng structures
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set error handling to avoid aborts
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Initialize dummy I/O callbacks
    png_set_read_fn(png_ptr, nullptr, dummy_read);
    png_set_write_fn(png_ptr, nullptr, dummy_write, dummy_flush);

    // Fuzz data interpretation for sBIT and shift
    if (Size >= sizeof(png_color_8)) {
        memcpy(&sig_bit, Data, sizeof(png_color_8));
        memcpy(&true_bits, Data, sizeof(png_color_8));
    } else if (Size > 0) {
        // Partial data, just set some bytes
        sig_bit.red = Data[0];
        if (Size > 1) sig_bit.green = Data[1];
        if (Size > 2) sig_bit.blue = Data[2];
        if (Size > 3) sig_bit.alpha = Data[3];
        
        true_bits.red = Data[0];
        if (Size > 1) true_bits.green = Data[1];
        if (Size > 2) true_bits.blue = Data[2];
        if (Size > 3) true_bits.alpha = Data[3];
    }

    // Test png_set_sBIT with valid pointers
    png_set_sBIT(png_ptr, info_ptr, &sig_bit);
    
    // Test png_set_sBIT with NULL sig_bit (should be safe)
    png_set_sBIT(png_ptr, info_ptr, nullptr);

    // Test png_get_bit_depth
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    (void)bit_depth;

    // Test png_get_sBIT
    sig_bit_ptr = &sig_bit;
    png_uint_32 sbit_result = png_get_sBIT(png_ptr, info_ptr, &sig_bit_ptr);
    (void)sbit_result;

    // Test png_get_sBIT with NULL pointer
    png_uint_32 sbit_null_result = png_get_sBIT(png_ptr, info_ptr, nullptr);
    (void)sbit_null_result;

    // Test png_get_channels
    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels;

    // Test png_set_shift with valid pointer only
    // The library implementation may not handle NULL for true_bits safely in all versions
    png_set_shift(png_ptr, &true_bits);

    // Test png_get_color_type
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    (void)color_type;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}