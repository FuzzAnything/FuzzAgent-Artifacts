// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_set_tIME at pngset.c:1118:1 in png.h
// png_set_tIME at pngset.c:1118:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Initialize structures
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

    // Set up error handling to prevent aborts during fuzzing
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Set minimal valid IHDR
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Fuzzing logic: invoke target APIs with varied inputs

    // 1. png_set_sBIT
    png_color_8 sig_bit;
    if (Size >= 3) {
        sig_bit.red = Data[0];
        sig_bit.green = Data[1];
        sig_bit.blue = Data[2];
        png_set_sBIT(png_ptr, info_ptr, &sig_bit);
    } else {
        // Fallback with default values
        sig_bit.red = 8; sig_bit.green = 8; sig_bit.blue = 8;
        png_set_sBIT(png_ptr, info_ptr, &sig_bit);
    }

    // 2. png_set_tIME
    png_time mod_time;
    mod_time.year = 2023;
    mod_time.month = 1;
    mod_time.day = 1;
    mod_time.hour = 0;
    mod_time.minute = 0;
    mod_time.second = 0;

    if (Size >= 6) {
        mod_time.year = (png_uint_16)(Data[0] * 256 + Data[1]);
        mod_time.month = (png_byte)(Data[2] % 12 + 1);
        mod_time.day = (png_byte)(Data[3] % 31 + 1);
        mod_time.hour = (png_byte)(Data[4] % 24);
        mod_time.minute = (png_byte)(Data[5] % 60);
        if (Size > 6) {
            mod_time.second = (png_byte)(Data[6] % 61);
        }
        png_set_tIME(png_ptr, info_ptr, &mod_time);
    } else {
        png_set_tIME(png_ptr, info_ptr, &mod_time);
    }

    // 3. png_write_info_before_PLTE
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // 4. png_write_info
    png_write_info(png_ptr, info_ptr);

    // Write dummy image row (required before png_write_end)
    png_byte row[3];
    row[0] = Data[0]; 
    row[1] = (Size > 1) ? Data[1] : 0; 
    row[2] = (Size > 2) ? Data[2] : 0;
    png_write_row(png_ptr, row);

    // 5. png_write_end
    png_write_end(png_ptr, info_ptr);

    // 6. png_destroy_info_struct
    png_destroy_info_struct(png_ptr, &info_ptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, nullptr);
    fclose(fp);

    return 0;
}