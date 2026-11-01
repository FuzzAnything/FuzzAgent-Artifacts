// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
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
#include <cstdio>

static void safe_png_destroy(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Use a dummy file for writing context if needed, though many set functions don't require it immediately
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        safe_png_destroy(png_ptr, info_ptr);
        return 0;
    }

    // Initialize error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(fp);
        safe_png_destroy(png_ptr, info_ptr);
        return 0;
    }

    // Initialize PNG write context
    png_init_io(png_ptr, fp);

    // Helper to extract fixed point values safely
    auto get_fixed_point = [&](size_t offset) -> png_fixed_point {
        if (offset + sizeof(png_fixed_point) > Size) return 100000; // Default 1.0
        png_fixed_point val;
        memcpy(&val, Data + offset, sizeof(png_fixed_point));
        return val;
    };

    auto get_byte = [&](size_t offset) -> png_byte {
        if (offset >= Size) return 0;
        return Data[offset];
    };

    auto get_int = [&](size_t offset) -> int {
        if (offset + sizeof(int) > Size) return 0;
        int val;
        memcpy(&val, Data + offset, sizeof(int));
        return val;
    };

    // Fuzzing logic based on available data size
    int action = Data[0] % 7;

    switch (action) {
        case 0: {
            // png_set_gAMA_fixed
            png_fixed_point gamma = get_fixed_point(1);
            if (gamma <= 0) gamma = 100000; // Ensure valid
            png_set_gAMA_fixed(png_ptr, info_ptr, gamma);
            break;
        }
        case 1: {
            // png_set_gamma_fixed
            png_fixed_point screen_gamma = get_fixed_point(1);
            png_fixed_point file_gamma = get_fixed_point(1 + sizeof(png_fixed_point));
            if (screen_gamma <= 0) screen_gamma = 100000;
            if (file_gamma <= 0) file_gamma = 100000;
            png_set_gamma_fixed(png_ptr, screen_gamma, file_gamma);
            break;
        }
        case 2: {
            // png_set_cHRM_fixed
            png_fixed_point wx = get_fixed_point(1);
            png_fixed_point wy = get_fixed_point(1 + sizeof(png_fixed_point));
            png_fixed_point rx = get_fixed_point(1 + 2 * sizeof(png_fixed_point));
            png_fixed_point ry = get_fixed_point(1 + 3 * sizeof(png_fixed_point));
            png_fixed_point gx = get_fixed_point(1 + 4 * sizeof(png_fixed_point));
            png_fixed_point gy = get_fixed_point(1 + 5 * sizeof(png_fixed_point));
            png_fixed_point bx = get_fixed_point(1 + 6 * sizeof(png_fixed_point));
            png_fixed_point by = get_fixed_point(1 + 7 * sizeof(png_fixed_point));
            png_set_cHRM_fixed(png_ptr, info_ptr, wx, wy, rx, ry, gx, gy, bx, by);
            break;
        }
        case 3: {
            // png_set_cHRM_XYZ_fixed
            png_fixed_point rX = get_fixed_point(1);
            png_fixed_point rY = get_fixed_point(1 + sizeof(png_fixed_point));
            png_fixed_point rZ = get_fixed_point(1 + 2 * sizeof(png_fixed_point));
            png_fixed_point gX = get_fixed_point(1 + 3 * sizeof(png_fixed_point));
            png_fixed_point gY = get_fixed_point(1 + 4 * sizeof(png_fixed_point));
            png_fixed_point gZ = get_fixed_point(1 + 5 * sizeof(png_fixed_point));
            png_fixed_point bX = get_fixed_point(1 + 6 * sizeof(png_fixed_point));
            png_fixed_point bY = get_fixed_point(1 + 7 * sizeof(png_fixed_point));
            png_fixed_point bZ = get_fixed_point(1 + 8 * sizeof(png_fixed_point));
            png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, rX, rY, rZ, gX, gY, gZ, bX, bY, bZ);
            break;
        }
        case 4: {
            // png_set_sRGB_gAMA_and_cHRM
            int intent = get_int(1) % 4;
            png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, intent);
            break;
        }
        case 5: {
            // png_get_gAMA_fixed (requires info to be populated, but safe to call)
            png_fixed_point gamma_val;
            png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_val);
            break;
        }
        case 6: {
            // png_get_hIST
            png_uint_16 *hist;
            png_get_hIST(png_ptr, info_ptr, &hist);
            break;
        }
    }

    // Cleanup
    fclose(fp);
    safe_png_destroy(png_ptr, info_ptr);
    return 0;
}