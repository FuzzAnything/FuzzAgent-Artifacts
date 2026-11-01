// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
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
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>

static void safe_read_png(const uint8_t *data, size_t size, png_structp *png_ptr_out, png_infop *info_ptr_out) {
    if (!data || size == 0) {
        *png_ptr_out = nullptr;
        *info_ptr_out = nullptr;
        return;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        *png_ptr_out = nullptr;
        *info_ptr_out = nullptr;
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        *png_ptr_out = nullptr;
        *info_ptr_out = nullptr;
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        *png_ptr_out = nullptr;
        *info_ptr_out = nullptr;
        return;
    }

    // Create a custom read callback that reads from the fuzzed data
    struct custom_io {
        const uint8_t *data;
        size_t size;
        size_t offset;
    };
    
    static custom_io io_ctx;
    io_ctx.data = data;
    io_ctx.size = size;
    io_ctx.offset = 0;

    png_set_read_fn(png_ptr, &io_ctx, [](png_structp png_ptr, png_bytep data, png_size_t length) {
        custom_io *ctx = (custom_io *)png_get_io_ptr(png_ptr);
        if (ctx->offset + length > ctx->size) {
            png_error(png_ptr, "Read beyond buffer");
        }
        memcpy(data, ctx->data + ctx->offset, length);
        ctx->offset += length;
    });

    // Read the header and info
    png_read_info(png_ptr, info_ptr);

    *png_ptr_out = png_ptr;
    *info_ptr_out = info_ptr;
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    safe_read_png(Data, Size, &png_ptr, &info_ptr);

    if (!png_ptr || !info_ptr) {
        return 0;
    }

    // Test png_get_gAMA_fixed
    png_fixed_point gamma_value = 0;
    png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_value);

    // Test png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);

    // Test png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);

    // Test png_get_uint_16 with a slice of the input data
    if (Size >= 2) {
        png_uint_16 val16 = png_get_uint_16(Data);
        (void)val16; // Use the value to suppress unused variable warning
    }

    // Test png_get_hIST
    png_uint_16 *hist = nullptr;
    png_get_hIST(png_ptr, info_ptr, &hist);
    if (hist) {
        // Access some elements to ensure the pointer is valid
        for (int i = 0; i < 10 && i < 256; ++i) {
            (void)hist[i];
        }
    }

    // Test png_get_tIME
    png_time mod_time;
    png_timep mod_time_ptr = &mod_time;
    png_get_tIME(png_ptr, info_ptr, &mod_time_ptr);

    // Cleanup
    cleanup_png(png_ptr, info_ptr);

    return 0;
}