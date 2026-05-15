// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_convert_from_struct_tm at pngwrite.c:518:1 in png.h
// png_convert_from_time_t at pngwrite.c:531:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_tIME at pngset.c:1118:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_convert_to_rfc1123_buffer at png.c:739:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "code/png.h"

static void initialize_png_structures(png_structp* png_ptr_ptr, png_infop* info_ptr_ptr) {
    *png_ptr_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (*png_ptr_ptr) {
        *info_ptr_ptr = png_create_info_struct(*png_ptr_ptr);
    }
}

static void cleanup_png_structures(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, info_ptr ? &info_ptr : nullptr, nullptr);
    }
}

static void populate_png_time_from_data(png_timep mod_time, const uint8_t* data, size_t size) {
    if (!mod_time || size < 7) return;
    
    mod_time->year = (data[0] << 8) | data[1];
    mod_time->month = data[2];
    mod_time->day = data[3];
    mod_time->hour = data[4];
    mod_time->minute = data[5];
    mod_time->second = data[6];
}

static void populate_struct_tm_from_data(struct tm* ttime, const uint8_t* data, size_t size) {
    if (!ttime || size < 9) return;
    
    ttime->tm_year = (data[0] << 8) | data[1];
    ttime->tm_mon = data[2];
    ttime->tm_mday = data[3];
    ttime->tm_hour = data[4];
    ttime->tm_min = data[5];
    ttime->tm_sec = data[6];
    ttime->tm_wday = data[7];
    ttime->tm_yday = data[8];
    ttime->tm_isdst = data[9] & 1;
}

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t* Data, size_t Size) {
    if (Size < 20) return 0;
    
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    
    initialize_png_structures(&png_ptr, &info_ptr);
    if (!png_ptr || !info_ptr) {
        cleanup_png_structures(png_ptr, info_ptr);
        return 0;
    }
    
    // Set up minimal IHDR data for testing
    png_set_IHDR(png_ptr, info_ptr, 
                 (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3],
                 (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7],
                 Data[8] & 0x0F,
                 Data[9] & 0x07,
                 Data[10] & 0x03,
                 Data[11] & 0x03,
                 Data[12] & 0x03);
    
    // Test png_set_tIME
    png_time mod_time;
    populate_png_time_from_data(&mod_time, Data, Size);
    png_set_tIME(png_ptr, info_ptr, &mod_time);
    
    // Test png_get_IHDR
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);
    
    // Test png_convert_to_rfc1123_buffer
    char out_buffer[29];
    png_convert_to_rfc1123_buffer(out_buffer, &mod_time);
    
    // Test png_get_tIME
    png_timep retrieved_time = nullptr;
    png_get_tIME(png_ptr, info_ptr, &retrieved_time);
    
    // Test png_convert_from_struct_tm
    struct tm ttime;
    populate_struct_tm_from_data(&ttime, Data + 13, Size - 13);
    png_time ptime_from_tm;
    png_convert_from_struct_tm(&ptime_from_tm, &ttime);
    
    // Test png_convert_from_time_t
    png_time ptime_from_time_t;
    time_t raw_time = (time_t)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
    png_convert_from_time_t(&ptime_from_time_t, raw_time);
    
    // Write dummy PNG file to test with actual file data
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size > 1024 ? 1024 : Size, dummy_file);
        fclose(dummy_file);
    }
    
    cleanup_png_structures(png_ptr, info_ptr);
    return 0;
}