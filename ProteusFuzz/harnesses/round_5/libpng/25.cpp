// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_tIME at pngset.c:1118:1 in png.h
// png_convert_to_rfc1123_buffer at png.c:739:1 in png.h
// png_convert_from_time_t at pngwrite.c:531:1 in png.h
// png_convert_from_struct_tm at pngwrite.c:518:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
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
#include <ctime>
#include <fstream>

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    } else if (info_ptr) {
        png_destroy_info_struct(nullptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Need at least enough data to safely access indices used below
    if (Size < 7) return 0;

    // Create a dummy file for writing
    const char* dummy_file = "./dummy_file";
    
    // Initialize PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Open dummy file for writing
    FILE* fp = fopen(dummy_file, "wb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Set a minimal image signature to allow writing
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_GRAY, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, 
                 PNG_FILTER_TYPE_DEFAULT);

    // Extract data for time conversion fuzzing with bounds checking
    png_time time_struct;
    time_struct.year = (png_uint_16)(Data[0] + Data[1]);
    time_struct.month = Data[2 % Size];
    time_struct.day = Data[3 % Size];
    time_struct.hour = Data[4 % Size];
    time_struct.minute = Data[5 % Size];
    time_struct.second = Data[6 % Size];

    // Fuzz png_set_tIME
    png_set_tIME(png_ptr, info_ptr, &time_struct);

    // Fuzz png_convert_to_rfc1123_buffer
    char buffer[29];
    memset(buffer, 0, sizeof(buffer));
    png_convert_to_rfc1123_buffer(buffer, &time_struct);

    // Fuzz png_convert_from_time_t
    time_t ttime = (time_t)Data[0] * 1000 + Data[1];
    png_convert_from_time_t(&time_struct, ttime);

    // Fuzz png_convert_from_struct_tm with safe indexing
    struct tm tm_struct;
    tm_struct.tm_year = Data[0];
    tm_struct.tm_mon = Data[1 % Size];
    tm_struct.tm_mday = Data[2 % Size];
    tm_struct.tm_hour = Data[3 % Size];
    tm_struct.tm_min = Data[4 % Size];
    tm_struct.tm_sec = Data[5 % Size];
    png_convert_from_struct_tm(&time_struct, &tm_struct);

    // Fuzz png_get_tIME
    png_time* mod_time = nullptr;
    png_uint_32 ret = png_get_tIME(png_ptr, info_ptr, &mod_time);
    (void)ret; // Suppress unused warning

    // Write end to finalize PNG
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    fclose(fp);
    cleanup_png(png_ptr, info_ptr);

    return 0;
}