// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_rows at pngset.c:1845:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_sRGB at pngset.c:819:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_write_png at pngwrite.c:1376:1 in png.h
// png_write_png at pngwrite.c:1376:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
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

static void write_data_to_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static png_fixed_point extract_fixed_point(const uint8_t *data, size_t size, size_t &offset) {
    if (offset + sizeof(png_fixed_point) > size) {
        return 0;
    }
    png_fixed_point val;
    memcpy(&val, data + offset, sizeof(png_fixed_point));
    offset += sizeof(png_fixed_point);
    return val;
}

static png_byte extract_byte(const uint8_t *data, size_t size, size_t &offset) {
    if (offset >= size) {
        return 0;
    }
    return data[offset++];
}

static int extract_int(const uint8_t *data, size_t size, size_t &offset) {
    if (offset + sizeof(int) > size) {
        return 0;
    }
    int val;
    memcpy(&val, data + offset, sizeof(int));
    offset += sizeof(int);
    return val;
}

// Global storage for row pointers to prevent use-after-free
static std::vector<png_bytep> g_row_pointers;
static std::vector<uint8_t> g_image_data;

static void write_png_data(png_structp png_ptr, png_infop info_ptr, const uint8_t *data, size_t size) {
    if (!png_ptr || !info_ptr || size == 0) return;

    // Clear previous data
    g_row_pointers.clear();
    g_image_data.clear();

    // Extract basic image parameters from data if possible
    size_t offset = 0;
    png_uint_32 width = 1;
    png_uint_32 height = 1;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    int interlace_type = PNG_INTERLACE_NONE;

    if (offset + 4 <= size) {
        memcpy(&width, data + offset, 4);
        offset += 4;
    }
    if (offset + 4 <= size) {
        memcpy(&height, data + offset, 4);
        offset += 4;
    }
    if (offset + 4 <= size) {
        memcpy(&bit_depth, data + offset, 4);
        offset += 4;
    }
    if (offset + 4 <= size) {
        memcpy(&color_type, data + offset, 4);
        offset += 4;
    }

    // Clamp values to valid ranges
    if (width == 0 || width > 10000) width = 100;
    if (height == 0 || height > 10000) height = 100;
    if (bit_depth == 0 || bit_depth > 16) bit_depth = 8;
    if (color_type == 0 || color_type > PNG_COLOR_TYPE_PALETTE) color_type = PNG_COLOR_TYPE_RGB;

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_type,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Create dummy image data
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // Resize global vectors
    g_row_pointers.resize(height);
    g_image_data.resize(rowbytes * height);

    for (png_uint_32 y = 0; y < height; ++y) {
        g_row_pointers[y] = &g_image_data[y * rowbytes];
        // Fill with some pattern based on input data
        for (int x = 0; x < rowbytes; ++x) {
            g_image_data[y * rowbytes + x] = (data[(y * rowbytes + x) % size] ^ (y + x)) & 0xFF;
        }
    }

    // Use global row pointers directly
    png_set_rows(png_ptr, info_ptr, &g_row_pointers[0]);
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Write dummy file
    write_data_to_file(Data, Size);

    // Open file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    size_t offset = 0;

    // Fuzz png_set_cHRM_fixed
    if (offset + 8 * sizeof(png_fixed_point) <= Size) {
        png_fixed_point white_x = extract_fixed_point(Data, Size, offset);
        png_fixed_point white_y = extract_fixed_point(Data, Size, offset);
        png_fixed_point red_x = extract_fixed_point(Data, Size, offset);
        png_fixed_point red_y = extract_fixed_point(Data, Size, offset);
        png_fixed_point green_x = extract_fixed_point(Data, Size, offset);
        png_fixed_point green_y = extract_fixed_point(Data, Size, offset);
        png_fixed_point blue_x = extract_fixed_point(Data, Size, offset);
        png_fixed_point blue_y = extract_fixed_point(Data, Size, offset);
        png_set_cHRM_fixed(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);
    }

    // Fuzz png_set_cHRM_XYZ_fixed
    if (offset + 9 * sizeof(png_fixed_point) <= Size) {
        png_fixed_point red_X = extract_fixed_point(Data, Size, offset);
        png_fixed_point red_Y = extract_fixed_point(Data, Size, offset);
        png_fixed_point red_Z = extract_fixed_point(Data, Size, offset);
        png_fixed_point green_X = extract_fixed_point(Data, Size, offset);
        png_fixed_point green_Y = extract_fixed_point(Data, Size, offset);
        png_fixed_point green_Z = extract_fixed_point(Data, Size, offset);
        png_fixed_point blue_X = extract_fixed_point(Data, Size, offset);
        png_fixed_point blue_Y = extract_fixed_point(Data, Size, offset);
        png_fixed_point blue_Z = extract_fixed_point(Data, Size, offset);
        png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);
    }

    // Fuzz png_set_sRGB_gAMA_and_cHRM
    if (offset + sizeof(int) <= Size) {
        int srgb_intent = extract_int(Data, Size, offset);
        png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);
    }

    // Fuzz png_set_sRGB
    if (offset + sizeof(int) <= Size) {
        int srgb_intent = extract_int(Data, Size, offset);
        png_set_sRGB(png_ptr, info_ptr, srgb_intent);
    }

    // Fuzz png_set_gAMA_fixed
    if (offset + sizeof(png_fixed_point) <= Size) {
        png_fixed_point gamma = extract_fixed_point(Data, Size, offset);
        png_set_gAMA_fixed(png_ptr, info_ptr, gamma);
    }

    // Prepare image data
    write_png_data(png_ptr, info_ptr, Data, Size);

    // Fuzz png_write_png
    if (offset + sizeof(int) <= Size) {
        int transforms = extract_int(Data, Size, offset);
        png_write_png(png_ptr, info_ptr, transforms, NULL);
    } else {
        png_write_png(png_ptr, info_ptr, 0, NULL);
    }

    // Cleanup
    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}