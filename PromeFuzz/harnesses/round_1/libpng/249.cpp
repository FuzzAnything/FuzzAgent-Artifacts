// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_set_tIME at pngset.c:1118:1 in png.h
// png_set_tIME at pngset.c:1118:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_convert_to_rfc1123_buffer at png.c:739:1 in png.h
// png_convert_from_struct_tm at pngwrite.c:518:1 in png.h
// png_convert_from_time_t at pngwrite.c:531:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <fstream>

#include "code/png.h"

static void initialize_png_time(png_time* ptime, const uint8_t* data, size_t size) {
    if (size >= sizeof(png_time)) {
        memcpy(ptime, data, sizeof(png_time));
    } else {
        memset(ptime, 0, sizeof(png_time));
        if (size > 0) {
            memcpy(ptime, data, size);
        }
    }
    // Ensure reasonable values
    ptime->year = ptime->year % 5000 + 1900;
    ptime->month = (ptime->month % 12) + 1;
}

static void initialize_struct_tm(struct tm* ttime, const uint8_t* data, size_t size) {
    memset(ttime, 0, sizeof(struct tm));
    if (size >= sizeof(struct tm)) {
        memcpy(ttime, data, sizeof(struct tm));
    } else if (size > 0) {
        memcpy(ttime, data, size);
    }
    // Ensure reasonable values
    ttime->tm_year = ttime->tm_year % 200 + 100;
    ttime->tm_mon = ttime->tm_mon % 12;
    ttime->tm_mday = (ttime->tm_mday % 31) + 1;
    ttime->tm_hour = ttime->tm_hour % 24;
    ttime->tm_min = ttime->tm_min % 60;
    ttime->tm_sec = ttime->tm_sec % 61;
    ttime->tm_wday = ttime->tm_wday % 7;
    ttime->tm_yday = ttime->tm_yday % 366;
    ttime->tm_isdst = ttime->tm_isdst % 3 - 1;
}

extern "C" int LLVMFuzzerTestOneInput_249(const uint8_t *Data, size_t Size) {
    // Test png_convert_to_rfc1123_buffer
    {
        char out_buffer[29];
        png_time ptime;
        initialize_png_time(&ptime, Data, Size);
        png_convert_to_rfc1123_buffer(out_buffer, &ptime);
    }

    // Test png_convert_from_struct_tm
    {
        png_time ptime;
        struct tm ttime;
        initialize_struct_tm(&ttime, Data, Size);
        png_convert_from_struct_tm(&ptime, &ttime);
    }

    // Test png_convert_from_time_t
    {
        png_time ptime;
        time_t ttime = 0;
        if (Size >= sizeof(time_t)) {
            memcpy(&ttime, Data, sizeof(time_t));
        } else if (Size > 0) {
            memcpy(&ttime, Data, Size);
        }
        png_convert_from_time_t(&ptime, ttime);
    }

    // Test png_get_IHDR and related functions
    {
        // Write fuzzer data to file
        std::ofstream out_file("./dummy_file", std::ios::binary);
        if (out_file.is_open()) {
            out_file.write(reinterpret_cast<const char*>(Data), Size);
            out_file.close();
        }

        // Try to read as PNG
        FILE* fp = fopen("./dummy_file", "rb");
        if (fp) {
            png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            if (png_ptr) {
                png_infop info_ptr = png_create_info_struct(png_ptr);
                if (info_ptr) {
                    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
                        png_init_io(png_ptr, fp);
                        png_read_info(png_ptr, info_ptr);

                        png_uint_32 width, height;
                        int bit_depth, color_type;
                        int interlace_method, compression_method, filter_method;

                        // Test png_get_IHDR
                        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
                                     &color_type, &interlace_method, &compression_method,
                                     &filter_method);

                        // Test png_get_tIME and png_set_tIME
                        png_timep mod_time;
                        if (png_get_tIME(png_ptr, info_ptr, &mod_time) & PNG_INFO_tIME) {
                            // If tIME exists, set it again
                            png_set_tIME(png_ptr, info_ptr, mod_time);
                        } else {
                            // Otherwise create and set a new time
                            png_time new_time;
                            initialize_png_time(&new_time, Data, Size);
                            png_set_tIME(png_ptr, info_ptr, &new_time);
                        }
                    }
                }
                png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            }
            fclose(fp);
        }
    }

    return 0;
}