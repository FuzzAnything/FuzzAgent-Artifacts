// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_mDCV at pngset.c:297:1 in png.h
// png_set_sCAL at pngset.c:664:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_cLLI at pngset.c:194:1 in png.h
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
#include <limits>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

static void safe_png_error(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing to prevent abort during fuzzing
}

static void safe_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing to prevent stderr output during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file to simulate file I/O if needed
    write_dummy_file(Data, Size);

    // Initialize PNG write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, safe_png_error, safe_png_warning);
    if (!png_ptr) return 0;

    // Initialize PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Use setjmp to catch libpng errors
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize a simple image buffer for writing
    int width = 1;
    int height = 1;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    
    // Ensure dimensions are within reasonable bounds based on input
    if (Size > 0) {
        width = Data[0] % 1024 + 1;
        height = (Size > 1 ? Data[1] : 0) % 1024 + 1;
    }

    // Initialize the write process
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Set IHDR
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Fuzz png_set_gAMA
    if (Size > 2) {
        double gamma = (double)(Data[2]) / 256.0;
        if (gamma > 0.001) {
            png_set_gAMA(png_ptr, info_ptr, gamma);
        }
    }

    // Fuzz png_set_cLLI
    if (Size > 4) {
        double max_content = (double)(Data[3]) * 100.0;
        double max_frame = (double)(Data[4]) * 100.0;
        png_set_cLLI(png_ptr, info_ptr, max_content, max_frame);
    }

    // Fuzz png_set_mDCV
    if (Size > 14) {
        double wx = (double)Data[5] / 256.0;
        double wy = (double)Data[6] / 256.0;
        double rx = (double)Data[7] / 256.0;
        double ry = (double)Data[8] / 256.0;
        double gx = (double)Data[9] / 256.0;
        double gy = (double)Data[10] / 256.0;
        double bx = (double)Data[11] / 256.0;
        double by = (double)Data[12] / 256.0;
        double max_lum = (double)Data[13] * 1000.0;
        double min_lum = (double)Data[14] * 1000.0;
        png_set_mDCV(png_ptr, info_ptr, wx, wy, rx, ry, gx, gy, bx, by, max_lum, min_lum);
    }

    // Fuzz png_set_sCAL
    if (Size > 16) {
        int unit = Data[15] % 2; // 0 or 1
        double w = (double)Data[16] / 256.0;
        double h = (Size > 17 ? (double)Data[17] : 1.0) / 256.0;
        if (w > 0 && h > 0) {
            png_set_sCAL(png_ptr, info_ptr, unit, w, h);
        }
    }

    // Write info chunks
    png_write_info(png_ptr, info_ptr);

    // Calculate row bytes manually since png_get_rowbytes takes only 2 args
    // For RGB (3 bytes per pixel)
    size_t row_bytes = width * 3;
    std::vector<png_byte> row(row_bytes, 0);

    // Write a single row of dummy data
    for (int y = 0; y < height; ++y) {
        png_write_row(png_ptr, row.data());
    }

    // Write end
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}