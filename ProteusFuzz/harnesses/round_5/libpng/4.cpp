// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_sCAL at pngset.c:664:1 in png.h
// png_set_mDCV at pngset.c:297:1 in png.h
// png_set_cLLI at pngset.c:194:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
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

static void fuzz_write_png(const uint8_t *data, size_t size) {
    // Create a dummy file for writing
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return;

    // Initialize libpng structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return;
    }

    // Set up error handling to prevent crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    // Set basic image parameters
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    // Parse fuzz data to set various metadata
    if (size >= 4) {
        int unit = (data[0] & 0x01); // 0 or 1 for sCAL unit
        double width = (double)(data[1] + 1) / 100.0;
        double height = (double)(data[2] + 1) / 100.0;
        png_set_sCAL(png_ptr, info_ptr, unit, width, height);
    }

    if (size >= 12) {
        double white_x = (double)data[3] / 255.0;
        double white_y = (double)data[4] / 255.0;
        double red_x = (double)data[5] / 255.0;
        double red_y = (double)data[6] / 255.0;
        double green_x = (double)data[7] / 255.0;
        double green_y = (double)data[8] / 255.0;
        double blue_x = (double)data[9] / 255.0;
        double blue_y = (double)data[10] / 255.0;
        double max_lum = (double)(data[11] * 10000);
        double min_lum = (double)(data[12] * 10000);
        png_set_mDCV(png_ptr, info_ptr, white_x, white_y, red_x, red_y,
                     green_x, green_y, blue_x, blue_y, max_lum, min_lum);
    }

    if (size >= 14) {
        double max_content = (double)(data[13] * 10000);
        double max_frame = (double)(data[14] * 10000);
        png_set_cLLI(png_ptr, info_ptr, max_content, max_frame);
    }

    if (size >= 15) {
        double gamma = (double)data[15] / 100.0;
        if (gamma > 0) {
            png_set_gAMA(png_ptr, info_ptr, gamma);
        }
    }

    // Write start
    png_write_info(png_ptr, info_ptr);

    // Write a single row of dummy image data
    png_byte row[3]; // 1x1 RGB pixel
    row[0] = data[size > 0 ? 0 : 0];
    row[1] = data[size > 1 ? 1 : 0];
    row[2] = data[size > 2 ? 2 : 0];
    png_write_row(png_ptr, row);

    // Write end
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz the PNG writing process with various metadata settings
    fuzz_write_png(Data, Size);

    return 0;
}