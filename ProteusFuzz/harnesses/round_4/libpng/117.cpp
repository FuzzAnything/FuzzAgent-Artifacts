// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_sBIT at pngget.c:1140:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_set_shift at pngtrans.c:83:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void read_png_from_buffer(png_structp png_ptr, png_infop info_ptr, const uint8_t *data, size_t size) {
    if (png_ptr == NULL || info_ptr == NULL || data == NULL || size == 0) {
        return;
    }

    // Create a custom read callback
    struct buffer_info {
        const uint8_t *data;
        size_t size;
        size_t offset;
    } buf_info = { data, size, 0 };

    // Set custom read function
    png_set_read_fn(png_ptr, &buf_info, [](png_structp png_ptr, png_bytep out, size_t size) {
        struct buffer_info *info = static_cast<struct buffer_info*>(png_get_io_ptr(png_ptr));
        if (info->offset + size > info->size) {
            png_error(png_ptr, "Read beyond buffer");
        }
        memcpy(out, info->data + info->offset, size);
        info->offset += size;
    });

    // Read the PNG file
    png_read_info(png_ptr, info_ptr);
}

extern "C" int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Read PNG data from buffer
    read_png_from_buffer(png_ptr, info_ptr, Data, Size);

    // Fuzz png_get_sBIT
    png_color_8 sig_bit;
    png_color_8p sig_bit_ptr = &sig_bit;
    png_get_sBIT(png_ptr, info_ptr, &sig_bit_ptr);

    // Fuzz png_get_bit_depth
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    (void)bit_depth;

    // Fuzz png_set_shift
    png_color_8 true_bits = { 8, 8, 8 };
    png_set_shift(png_ptr, &true_bits);

    // Fuzz png_set_sBIT
    png_set_sBIT(png_ptr, info_ptr, &sig_bit);

    // Fuzz png_get_channels
    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels;

    // Fuzz png_read_png
    int transforms = PNG_TRANSFORM_IDENTITY;
    png_read_png(png_ptr, info_ptr, transforms, NULL);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}