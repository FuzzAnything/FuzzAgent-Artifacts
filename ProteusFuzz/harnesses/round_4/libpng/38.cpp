// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_tIME at pngset.c:1118:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_bKGD at pngset.c:24:1 in png.h
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

// Static helper to safely write data to a dummy file
static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

// Custom error handler to prevent libpng from calling exit()
static void
fuzz_png_error(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    // Just return to let the fuzzer continue
    return;
}

static void
fuzz_png_warning(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
    return;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a dummy file for writing
    write_dummy_file(Data, Size);
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, NULL, fuzz_png_error, fuzz_png_warning);
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

    // Set up error handling to prevent exit()
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Set basic image parameters to allow writing
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    // Prepare a dummy row for writing
    png_byte row_data[3]; // 1 pixel, RGB
    row_data[0] = Data[0];
    row_data[1] = Data[0] + 1;
    row_data[2] = Data[0] + 2;
    png_bytep row_pointer = row_data;

    // Test png_set_bKGD
    png_color_16 background;
    background.index = 0;
    background.red = Data[0];
    background.green = Data[0] + 1;
    background.blue = Data[0] + 2;
    background.gray = 0;
    png_set_bKGD(png_ptr, info_ptr, &background);

    // Test png_set_tIME
    png_time mod_time;
    mod_time.year = 2000 + (Data[0] % 100);
    mod_time.month = (Data[0] % 12) + 1;
    mod_time.day = (Data[0] % 28) + 1;
    mod_time.hour = Data[0] % 24;
    mod_time.minute = Data[0] % 60;
    mod_time.second = Data[0] % 60;
    png_set_tIME(png_ptr, info_ptr, &mod_time);

    // Test png_write_info_before_PLTE
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // Test png_write_info
    png_write_info(png_ptr, info_ptr);

    // Write a single row of image data
    png_write_row(png_ptr, row_pointer);

    // Test png_write_end
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_write_struct(&png_ptr, NULL);
    fclose(fp);

    return 0;
}