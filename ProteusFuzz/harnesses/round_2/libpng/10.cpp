// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_sRGB at pngset.c:819:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_write_png at pngwrite.c:1376:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

static void write_data_to_file(const uint8_t *data, size_t size, const char *filename) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void safe_png_destroy(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    } else if (info_ptr) {
        png_destroy_info_struct(nullptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (!Data || Size == 0) return 0;

    // Limit size to avoid excessive memory usage
    if (Size > 1024 * 1024) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    FILE *fp = nullptr;
    png_bytep *row_pointers = nullptr;
    int height = 0;

    // Create PNG structure and open file
    fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(fp);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        // Cleanup on error
        if (row_pointers) {
            for (int i = 0; i < height; ++i) {
                if (row_pointers[i]) delete[] row_pointers[i];
            }
            delete[] row_pointers;
            row_pointers = nullptr;
        }
        safe_png_destroy(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Set image parameters
    int width = 1, h = 1;
    if (Size >= 2) {
        width = Data[0] + 1;
        h = Data[1] + 1;
    }
    if (width > 1000 || h > 1000) {
        width = 1000;
        h = 1000;
    }
    height = h;

    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Allocate row data
    row_pointers = new png_bytep[height];
    for (int i = 0; i < height; ++i) {
        row_pointers[i] = new png_byte[width * 3];
        memset(row_pointers[i], 0, width * 3);
    }

    // Fuzz various API calls
    if (Size > 2) {
        int srgb_intent = Data[2] % 4;
        png_set_sRGB(png_ptr, info_ptr, srgb_intent);
    }

    if (Size > 3) {
        png_fixed_point gamma = static_cast<png_fixed_point>(Data[3] * 100000);
        png_set_gAMA_fixed(png_ptr, info_ptr, gamma);
    }

    if (Size > 11) {
        png_fixed_point wx = static_cast<png_fixed_point>(Data[3] * 100000);
        png_fixed_point wy = static_cast<png_fixed_point>(Data[4] * 100000);
        png_fixed_point rx = static_cast<png_fixed_point>(Data[5] * 100000);
        png_fixed_point ry = static_cast<png_fixed_point>(Data[6] * 100000);
        png_fixed_point gx = static_cast<png_fixed_point>(Data[7] * 100000);
        png_fixed_point gy = static_cast<png_fixed_point>(Data[8] * 100000);
        png_fixed_point bx = static_cast<png_fixed_point>(Data[9] * 100000);
        png_fixed_point by = static_cast<png_fixed_point>(Data[10] * 100000);
        png_set_cHRM_fixed(png_ptr, info_ptr, wx, wy, rx, ry, gx, gy, bx, by);
    }

    if (Size > 19) {
        png_fixed_point rX = static_cast<png_fixed_point>(Data[3] * 100000);
        png_fixed_point rY = static_cast<png_fixed_point>(Data[4] * 100000);
        png_fixed_point rZ = static_cast<png_fixed_point>(Data[5] * 100000);
        png_fixed_point gX = static_cast<png_fixed_point>(Data[6] * 100000);
        png_fixed_point gY = static_cast<png_fixed_point>(Data[7] * 100000);
        png_fixed_point gZ = static_cast<png_fixed_point>(Data[8] * 100000);
        png_fixed_point bX = static_cast<png_fixed_point>(Data[9] * 100000);
        png_fixed_point bY = static_cast<png_fixed_point>(Data[10] * 100000);
        png_fixed_point bZ = static_cast<png_fixed_point>(Data[11] * 100000);
        png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, rX, rY, rZ, gX, gY, gZ, bX, bY, bZ);
    }

    if (Size > 12) {
        int srgb_intent = Data[12] % 4;
        png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);
    }

    // Write PNG
    int transforms = 0;
    if (Size > 13) {
        transforms = Data[13] % 8;
    }

    png_write_png(png_ptr, info_ptr, transforms, nullptr);

    // Cleanup
    if (row_pointers) {
        for (int i = 0; i < height; ++i) {
            if (row_pointers[i]) delete[] row_pointers[i];
        }
        delete[] row_pointers;
        row_pointers = nullptr;
    }

    safe_png_destroy(png_ptr, info_ptr);
    fclose(fp);

    return 0;
}