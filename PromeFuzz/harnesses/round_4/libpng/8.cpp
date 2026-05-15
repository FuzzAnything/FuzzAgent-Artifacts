// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_convert_from_struct_tm at pngwrite.c:518:1 in png.h
// png_convert_from_time_t at pngwrite.c:531:1 in png.h
// png_set_tIME at pngset.c:1118:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_convert_to_rfc1123_buffer at png.c:739:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
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

#include "code/png.h"

static void initialize_png_structures(png_structp* png_ptr, png_infop* info_ptr) {
    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!*png_ptr) return;
    
    *info_ptr = png_create_info_struct(*png_ptr);
    if (!*info_ptr) {
        png_destroy_read_struct(png_ptr, nullptr, nullptr);
        *png_ptr = nullptr;
    }
}

static void cleanup_png_structures(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr && info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (png_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Initialize PNG structures
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    initialize_png_structures(&png_ptr, &info_ptr);
    if (!png_ptr || !info_ptr) return 0;
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png_structures(png_ptr, info_ptr);
        return 0;
    }
    
    // Test png_convert_from_struct_tm
    if (Size >= sizeof(struct tm)) {
        struct tm tm_time;
        memcpy(&tm_time, Data, sizeof(struct tm));
        png_time ptime1;
        png_convert_from_struct_tm(&ptime1, &tm_time);
    }
    
    // Test png_convert_from_time_t
    if (Size >= sizeof(time_t)) {
        time_t ttime;
        memcpy(&ttime, Data, sizeof(time_t));
        png_time ptime2;
        png_convert_from_time_t(&ptime2, ttime);
    }
    
    // Test png_set_tIME and png_get_tIME
    if (Size >= sizeof(png_time)) {
        png_time mod_time;
        memcpy(&mod_time, Data, sizeof(png_time));
        
        // Set time
        png_set_tIME(png_ptr, info_ptr, &mod_time);
        
        // Get time
        png_timep retrieved_time = nullptr;
        png_uint_32 time_result = png_get_tIME(png_ptr, info_ptr, &retrieved_time);
        
        // Test png_convert_to_rfc1123_buffer
        if (retrieved_time) {
            char buffer[29];
            png_convert_to_rfc1123_buffer(buffer, retrieved_time);
        }
    }
    
    // Test png_get_IHDR
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0, interlace_method = 0;
    int compression_method = 0, filter_method = 0;
    
    // First set some dummy IHDR values
    png_set_IHDR(png_ptr, info_ptr, 100, 100, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    
    // Then get them
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
                 &color_type, &interlace_method, &compression_method,
                 &filter_method);
    
    // Test with NULL pointers
    png_get_IHDR(png_ptr, info_ptr, nullptr, nullptr, nullptr,
                 nullptr, nullptr, nullptr, nullptr);
    
    cleanup_png_structures(png_ptr, info_ptr);
    return 0;
}