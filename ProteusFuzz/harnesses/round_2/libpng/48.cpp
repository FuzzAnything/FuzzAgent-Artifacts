// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_PLTE at pngset.c:739:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_write_png at pngwrite.c:1376:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
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
#include <vector>
#include <cstdio>
#include <cstdlib>

static void write_dummy_png_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

static void my_png_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void my_png_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    fwrite(Data, 1, Size, f);
    fclose(f);

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_set_error_fn(png_ptr, nullptr, my_png_error_fn, my_png_warning_fn);

    f = fopen("./dummy_file", "rb");
    if (!f) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_init_io(png_ptr, f);

    // Initialize basic image info
    png_uint_32 width = 1, height = 1;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;

    // Fuzz parameters slightly based on input
    if (Size > 0) width = (Data[0] % 100) + 1;
    if (Size > 1) height = (Data[1] % 100) + 1;
    if (Size > 2) bit_depth = (Data[2] % 4) + 1; // 1, 2, 4, 8
    if (Size > 3) color_type = Data[3] % 6; // 0 to 5

    // Ensure valid combinations
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        if (bit_depth > 8) bit_depth = 8;
    } else if (color_type == PNG_COLOR_TYPE_GRAY) {
        if (bit_depth > 16) bit_depth = 16;
    } else if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        if (bit_depth > 8) bit_depth = 8;
    } else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
        if (bit_depth > 8) bit_depth = 8;
    }

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Test png_write_info_before_PLTE
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // Test png_set_PLTE if palette
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        int num_palette = 1 << bit_depth;
        std::vector<png_color> palette(num_palette);
        for (int i = 0; i < num_palette; ++i) {
            palette[i].red = Data[i % Size];
            palette[i].green = Data[(i + 1) % Size];
            palette[i].blue = Data[(i + 2) % Size];
        }
        png_set_PLTE(png_ptr, info_ptr, palette.data(), num_palette);
    }

    // Test unknown chunks
    if (Size >= 10) {
        std::vector<png_unknown_chunk> unknowns(1);
        unknowns[0].name[0] = 't';
        unknowns[0].name[1] = 'E';
        unknowns[0].name[2] = 'S';
        unknowns[0].name[3] = 'T';
        unknowns[0].name[4] = '\0';
        unknowns[0].size = 5;
        unknowns[0].data = (png_byte *)malloc(5);
        if (unknowns[0].data) {
            memcpy(unknowns[0].data, Data, 5);
            unknowns[0].location = PNG_HAVE_IHDR; // Valid location
            png_set_unknown_chunks(png_ptr, info_ptr, unknowns.data(), 1);
            png_set_unknown_chunk_location(png_ptr, info_ptr, 0, PNG_HAVE_IHDR);
            free(unknowns[0].data);
        }
    }

    // Write image data
    std::vector<png_byte> row(width * ((color_type == PNG_COLOR_TYPE_RGB ? 3 : (color_type == PNG_COLOR_TYPE_RGB_ALPHA ? 4 : 1))));
    for (png_uint_32 y = 0; y < height; ++y) {
        for (size_t i = 0; i < row.size(); ++i) {
            row[i] = Data[(y * row.size() + i) % Size];
        }
        png_write_row(png_ptr, row.data());
    }

    // Test png_write_png with transforms
    int transforms = 0;
    if (Size > 4) transforms = Data[4] % 10;

    // Re-open file for png_write_png test
    f = fopen("./dummy_file", "wb");
    if (f) {
        fclose(f);
        f = fopen("./dummy_file", "rb");
        if (f) {
            png_init_io(png_ptr, f);
            png_write_png(png_ptr, info_ptr, transforms, nullptr);
            fclose(f);
        }
    }

    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 0;
}