// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_tIME at pngset.c:1118:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_convert_from_struct_tm at pngwrite.c:518:1 in png.h
// png_convert_from_time_t at pngwrite.c:531:1 in png.h
// png_convert_to_rfc1123_buffer at png.c:739:1 in png.h
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

#include "png.h"

static void initialize_png_structures(png_structp* png_ptr_ptr, png_infop* info_ptr_ptr) {
    *png_ptr_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!*png_ptr_ptr) return;
    
    *info_ptr_ptr = png_create_info_struct(*png_ptr_ptr);
    if (!*info_ptr_ptr) {
        png_destroy_read_struct(png_ptr_ptr, nullptr, nullptr);
        *png_ptr_ptr = nullptr;
    }
}

static void cleanup_png_structures(png_structp png_ptr, png_infop info_ptr) {
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;
    
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    initialize_png_structures(&png_ptr, &info_ptr);
    if (!png_ptr || !info_ptr) return 0;
    
    // Test png_set_tIME and png_get_tIME
    png_time mod_time;
    mod_time.year = *reinterpret_cast<const png_uint_16*>(Data);
    mod_time.month = Data[2];
    mod_time.day = Data[3];
    mod_time.hour = Data[4];
    mod_time.minute = Data[5];
    mod_time.second = Data[6];
    
    png_set_tIME(png_ptr, info_ptr, &mod_time);
    
    png_timep retrieved_time = nullptr;
    png_uint_32 time_info = png_get_tIME(png_ptr, info_ptr, &retrieved_time);
    if (time_info & PNG_INFO_tIME && retrieved_time) {
        // Verify retrieved values match what we set
    }
    
    // Test png_get_IHDR - first set some dummy IHDR data
    png_uint_32 width = *reinterpret_cast<const png_uint_32*>(Data + 7);
    png_uint_32 height = *reinterpret_cast<const png_uint_32*>(Data + 11);
    int bit_depth = Data[15];
    int color_type = Data[16];
    int interlace_method = Data[17];
    int compression_method = Data[18];
    int filter_method = Data[19];
    
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);
    
    png_uint_32 retrieved_width, retrieved_height;
    int retrieved_bit_depth, retrieved_color_type;
    int retrieved_interlace, retrieved_compression, retrieved_filter;
    png_get_IHDR(png_ptr, info_ptr, &retrieved_width, &retrieved_height,
                 &retrieved_bit_depth, &retrieved_color_type,
                 &retrieved_interlace, &retrieved_compression, &retrieved_filter);
    
    // Test png_convert_from_struct_tm
    struct tm tm_time;
    memset(&tm_time, 0, sizeof(struct tm));
    tm_time.tm_year = *reinterpret_cast<const int*>(Data) % 1000;
    tm_time.tm_mon = Data[2] % 12;
    tm_time.tm_mday = Data[3] % 31 + 1;
    tm_time.tm_hour = Data[4] % 24;
    tm_time.tm_min = Data[5] % 60;
    tm_time.tm_sec = Data[6] % 61;
    
    png_time png_time_from_tm;
    png_convert_from_struct_tm(&png_time_from_tm, &tm_time);
    
    // Test png_convert_from_time_t
    time_t raw_time = *reinterpret_cast<const time_t*>(Data);
    png_time png_time_from_time_t;
    png_convert_from_time_t(&png_time_from_time_t, raw_time);
    
    // Test png_convert_to_rfc1123_buffer
    char buffer[29];
    int result = png_convert_to_rfc1123_buffer(buffer, &mod_time);
    
    cleanup_png_structures(png_ptr, info_ptr);
    
    return 0;
}