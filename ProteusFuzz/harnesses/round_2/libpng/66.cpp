// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_mDCV at pngset.c:297:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_sCAL at pngset.c:664:1 in png.h
// png_set_cLLI at pngset.c:194:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
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
#include <cstdio>
#include <cstring>
#include <cstdlib>

static void fuzzer_error_fn(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    // Do not abort, just return to allow fuzzer to continue
}

static void fuzzer_warning_fn(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

static void write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
    // Dummy write callback
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Create a dummy file for writing
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;

    // Initialize PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, fuzzer_error_fn, fuzzer_warning_fn);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return 0;
    }

    // Set up error handling with longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);
    png_set_write_fn(png_ptr, NULL, write_data, NULL);

    // Extract some values from input to drive fuzzing
    double white_x = Data[0] / 255.0;
    double white_y = Data[1] / 255.0;
    double red_x = Data[2] / 255.0;
    double red_y = Data[3] / 255.0;
    double green_x = Data[4] / 255.0;
    double green_y = Data[5] / 255.0;
    double blue_x = Data[6] / 255.0;
    double blue_y = Data[7] / 255.0;
    double max_lum = Data[8] * 100.0;
    double min_lum = Data[9] * 100.0;

    double gamma = Data[0] / 255.0;
    int unit = Data[0] % 2;
    double width = Data[1] / 100.0;
    double height = Data[2] / 100.0;

    double max_content_light = Data[3] * 1000.0;
    double max_frame_avg_light = Data[4] * 1000.0;

    // Call png_set_mDCV
    png_set_mDCV(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y, max_lum, min_lum);

    // Call png_set_gAMA
    png_set_gAMA(png_ptr, info_ptr, gamma);

    // Call png_set_sCAL
    png_set_sCAL(png_ptr, info_ptr, unit, width, height);

    // Call png_set_cLLI
    png_set_cLLI(png_ptr, info_ptr, max_content_light, max_frame_avg_light);

    // Call png_write_info
    png_write_info(png_ptr, info_ptr);

    // Write dummy image data (1x1 pixel)
    png_byte row[3] = {0, 0, 0};
    png_bytep row_pointers[1];
    row_pointers[0] = row;
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_image(png_ptr, row_pointers);

    // Call png_write_end
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    return 0;
}