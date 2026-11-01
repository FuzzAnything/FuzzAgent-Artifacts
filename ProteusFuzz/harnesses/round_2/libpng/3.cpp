// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_bKGD at pngset.c:24:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_tIME at pngset.c:1118:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_bKGD at pngset.c:24:1 in png.h
// png_set_tIME at pngset.c:1118:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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

static void write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    if (size > 0) {
        fwrite(data, 1, size, f);
    } else {
        // Write a minimal valid PNG header if input is empty
        uint8_t header[] = {
            0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A
        };
        fwrite(header, 1, 8, f);
    }
    fclose(f);
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for read operations if needed
    write_dummy_png(Data, Size);

    // Test 1: png_create_info_struct and png_destroy_info_struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr) {
        png_destroy_info_struct(png_ptr, &info_ptr);
    }

    // Test 2: png_set_bKGD
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr) {
        png_color_16 background = {0, 0, 0, 0, 0};
        png_set_bKGD(png_ptr, info_ptr, &background);
        png_destroy_info_struct(png_ptr, &info_ptr);
    }

    // Test 3: png_set_tIME
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr) {
        png_time mod_time;
        // Fuzz time values, but ensure they are within valid ranges to avoid immediate errors
        mod_time.year = (Data[0] % 200) + 1900;
        mod_time.month = (Data[1] % 12) + 1;
        mod_time.day = (Data[2] % 28) + 1;
        mod_time.hour = Data[3] % 24;
        mod_time.minute = Data[4] % 60;
        mod_time.second = Data[5] % 60;
        png_set_tIME(png_ptr, info_ptr, &mod_time);
        png_destroy_info_struct(png_ptr, &info_ptr);
    }

    // Test 4: png_destroy_read_struct
    png_infop end_info_ptr = NULL;
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info_ptr);

    // Test 5: png_destroy_write_struct
    png_structp write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (write_png_ptr) {
        png_infop write_info_ptr = png_create_info_struct(write_png_ptr);
        if (write_info_ptr) {
            png_color_16 bg = {0, 0, 0, 0, 0};
            png_set_bKGD(write_png_ptr, write_info_ptr, &bg);
            png_time t;
            t.year = 2023; t.month = 1; t.day = 1; t.hour = 0; t.minute = 0; t.second = 0;
            png_set_tIME(write_png_ptr, write_info_ptr, &t);
            png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        } else {
            png_destroy_write_struct(&write_png_ptr, NULL);
        }
    }

    return 0;
}