// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
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
#include <vector>

static void write_png_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    if (fwrite(data, 1, length, stdout) != length) {
        png_error(png_ptr, "Write failed");
    }
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create dummy file for testing if needed, though we use stdout here
    // FILE *fp = fopen("./dummy_file", "wb");
    // if (fp) {
    //     fwrite(Data, 1, Size, fp);
    //     fclose(fp);
    // }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize output to stdout
    png_init_io(png_ptr, stdout);

    // Determine image parameters from input data
    int width = 1;
    int height = 1;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    int num_palette = 0;

    if (Size >= 4) {
        width = Data[0] + 1;
        height = Data[1] + 1;
        bit_depth = (Data[2] % 5) + 1; // 1, 2, 4, 8, 16
        int color_idx = Data[3] % 6;
        switch (color_idx) {
            case 0: color_type = PNG_COLOR_TYPE_GRAY; break;
            case 1: color_type = PNG_COLOR_TYPE_RGB; break;
            case 2: color_type = PNG_COLOR_TYPE_PALETTE; break;
            case 3: color_type = PNG_COLOR_TYPE_GRAY_ALPHA; break;
            case 4: color_type = PNG_COLOR_TYPE_RGB_ALPHA; break;
            case 5: color_type = PNG_COLOR_TYPE_GRAY; bit_depth = 1; break;
        }
    }

    // Set image parameters
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Handle palette if needed
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        num_palette = 1 << bit_depth;
        std::vector<png_color> palette(num_palette);
        for (int i = 0; i < num_palette; ++i) {
            palette[i].red = Data[(i * 3) % Size];
            palette[i].green = Data[(i * 3 + 1) % Size];
            palette[i].blue = Data[(i * 3 + 2) % Size];
        }
        png_set_PLTE(png_ptr, info_ptr, palette.data(), num_palette);
    }

    // Test png_set_unknown_chunks
    if (Size >= 10) {
        std::vector<png_unknown_chunk> unknowns(1);
        unknowns[0].name[0] = 'x';
        unknowns[0].name[1] = 'D';
        unknowns[0].name[2] = 'A';
        unknowns[0].name[3] = 'T';
        unknowns[0].name[4] = '\0';
        unknowns[0].size = 4;
        unknowns[0].data = new png_byte[4];
        memcpy(unknowns[0].data, Data + 4, 4);
        unknowns[0].location = PNG_HAVE_IHDR | PNG_HAVE_PLTE; // Example location

        png_set_unknown_chunks(png_ptr, info_ptr, unknowns.data(), 1);
        png_set_unknown_chunk_location(png_ptr, info_ptr, 0, PNG_HAVE_IHDR);

        // Cleanup allocated data
        delete[] unknowns[0].data;
    }

    // Test png_write_info_before_PLTE
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // Test png_write_info
    png_write_info(png_ptr, info_ptr);

    // Prepare image data
    int bytes_per_pixel = 0;
    switch (color_type) {
        case PNG_COLOR_TYPE_GRAY:
            bytes_per_pixel = bit_depth <= 8 ? 1 : 2;
            break;
        case PNG_COLOR_TYPE_RGB:
            bytes_per_pixel = 3;
            break;
        case PNG_COLOR_TYPE_PALETTE:
            bytes_per_pixel = 1;
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            bytes_per_pixel = bit_depth <= 8 ? 2 : 4;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            bytes_per_pixel = 4;
            break;
    }

    if (bytes_per_pixel == 0) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    size_t row_bytes = width * bytes_per_pixel;
    std::vector<png_byte> row(row_bytes);
    for (size_t i = 0; i < row_bytes; ++i) {
        row[i] = Data[i % Size];
    }

    std::vector<png_bytep> row_pointers(height);
    for (int y = 0; y < height; ++y) {
        row_pointers[y] = row.data();
    }

    // Test png_write_png (simplified write)
    // Note: png_write_png expects row_pointers to be passed via params or handled internally
    // Since we already called png_write_info, we will manually write image data
    png_write_image(png_ptr, row_pointers.data());

    // Write end info
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 0;
}