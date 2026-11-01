// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_write_png at pngwrite.c:1376:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_oFFs at pngset.c:465:1 in png.h
// png_set_sRGB at pngset.c:819:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
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
#include <vector>

static void write_png_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do not write to a real file or memory to avoid side effects during fuzzing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void error_callback(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void warning_callback(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create png_struct and png_info
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, error_callback, warning_callback);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize the PNG structure for writing
    png_init_io(png_ptr, nullptr); // No actual file, just for initialization

    // Set image parameters
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Create dummy image data
    std::vector<png_bytep> row_pointers(1);
    row_pointers[0] = (png_bytep)malloc(3); // 3 bytes for RGB
    if (!row_pointers[0]) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    row_pointers[0][0] = 255;
    row_pointers[0][1] = 0;
    row_pointers[0][2] = 0;

    // Extract data from fuzz input for various parameters
    int transforms = Data[0] % 256;
    int srgb_intent = Data[1] % 4;
    int unit_type = Data[2] % 4;

    // png_set_cHRM_XYZ_fixed
    png_fixed_point red_X = (png_fixed_point)(Data[3] * 100000);
    png_fixed_point red_Y = (png_fixed_point)(Data[4] * 100000);
    png_fixed_point red_Z = (png_fixed_point)(Data[5] * 100000);
    png_fixed_point green_X = (png_fixed_point)(Data[6] * 100000);
    png_fixed_point green_Y = (png_fixed_point)(Data[7] * 100000);
    png_fixed_point green_Z = (png_fixed_point)(Data[8] * 100000);
    png_fixed_point blue_X = (png_fixed_point)(Data[9] * 100000);
    png_fixed_point blue_Y = (png_fixed_point)(Data[10] * 100000);
    png_fixed_point blue_Z = (png_fixed_point)(Data[11] * 100000);

    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);

    // png_set_oFFs
    png_int_32 offset_x = (png_int_32)(Data[12] * 1000);
    png_int_32 offset_y = (png_int_32)(Data[13] * 1000);
    png_set_oFFs(png_ptr, info_ptr, offset_x, offset_y, unit_type);

    // png_set_sRGB
    png_set_sRGB(png_ptr, info_ptr, srgb_intent);

    // png_set_invalid
    int invalid_mask = Data[14] % 256;
    png_set_invalid(png_ptr, info_ptr, invalid_mask);

    // png_set_unknown_chunks
    if (Size > 15) {
        int num_unknowns = Data[15] % 5;
        if (num_unknowns > 0) {
            std::vector<png_unknown_chunk> unknowns(num_unknowns);
            for (int i = 0; i < num_unknowns; ++i) {
                memset(unknowns[i].name, 'A' + i, 4);
                unknowns[i].name[4] = '\0';
                unknowns[i].size = Data[16 + i] % 100;
                unknowns[i].data = (png_byte*)malloc(unknowns[i].size);
                if (unknowns[i].data) {
                    for (size_t j = 0; j < unknowns[i].size; ++j) {
                        unknowns[i].data[j] = Data[16 + num_unknowns + i * 100 + j] % 256;
                    }
                }
                unknowns[i].location = Data[16 + num_unknowns + i * 100 + unknowns[i].size] % 256;
            }
            png_set_unknown_chunks(png_ptr, info_ptr, unknowns.data(), num_unknowns);
            for (int i = 0; i < num_unknowns; ++i) {
                free(unknowns[i].data);
            }
        }
    }

    // png_write_png
    png_write_png(png_ptr, info_ptr, transforms, nullptr);

    // Cleanup
    for (size_t i = 0; i < row_pointers.size(); ++i) {
        free(row_pointers[i]);
    }
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}