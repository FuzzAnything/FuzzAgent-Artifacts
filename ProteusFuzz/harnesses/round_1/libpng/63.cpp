// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_tIME at pngget.c:1184:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_convert_from_struct_tm at pngwrite.c:518:1 in png.h
// png_convert_from_time_t at pngwrite.c:531:1 in png.h
// png_convert_to_rfc1123_buffer at png.c:739:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_tIME at pngset.c:1118:1 in png.h
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

// Callback to write data to a dummy file
static void dummy_write(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

// Callback to write info
static void dummy_info_write(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

// Callback to write warning
static void dummy_warning(png_structp png_ptr, png_const_charp message) {
    (void)png_ptr;
    (void)message;
}

// Callback to write error
static void dummy_error(png_structp png_ptr, png_const_charp message) {
    (void)png_ptr;
    (void)message;
}

// Helper to create a minimal valid PNG structure for writing
static png_structp create_png_struct_for_write() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return nullptr;

    // Set custom callbacks to avoid file I/O and error printing during fuzzing
    png_set_write_fn(png_ptr, NULL, dummy_write, NULL);
    png_set_error_fn(png_ptr, NULL, dummy_error, dummy_warning);

    return png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize libpng structures
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_time time_struct;
    struct tm tm_struct;
    char rfc_buffer[29];
    png_time *time_ptr = nullptr;
    png_uint_32 result;

    // Initialize time_struct with zeros to avoid uninitialized memory issues
    memset(&time_struct, 0, sizeof(time_struct));
    memset(&tm_struct, 0, sizeof(tm_struct));
    memset(rfc_buffer, 0, sizeof(rfc_buffer));

    // Fuzzing Strategy 1: Test png_convert_from_struct_tm
    if (Size >= 10) {
        tm_struct.tm_year = (int16_t)Data[0] + (int16_t)Data[1] * 256; 
        tm_struct.tm_mon = (int8_t)Data[2];
        tm_struct.tm_mday = (int8_t)Data[3];
        tm_struct.tm_hour = (int8_t)Data[4];
        tm_struct.tm_min = (int8_t)Data[5];
        tm_struct.tm_sec = (int8_t)Data[6];
        tm_struct.tm_isdst = (int8_t)Data[7];
        tm_struct.tm_yday = (int16_t)Data[8] + (int16_t)Data[9] * 256;
        
        // Call conversion
        png_convert_from_struct_tm(&time_struct, &tm_struct);
    }

    // Fuzzing Strategy 2: Test png_convert_from_time_t
    if (Size >= 4) {
        time_t ttime = (time_t)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
        png_convert_from_time_t(&time_struct, ttime);
    }

    // Fuzzing Strategy 3: Test png_convert_to_rfc1123_buffer
    png_convert_to_rfc1123_buffer(rfc_buffer, &time_struct);

    // Fuzzing Strategy 4: Test png_set_tIME and png_get_tIME
    png_ptr = create_png_struct_for_write();
    if (png_ptr) {
        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr) {
            // Set tIME
            png_set_tIME(png_ptr, info_ptr, &time_struct);

            // Get tIME
            time_ptr = &time_struct; 
            result = png_get_tIME(png_ptr, info_ptr, &time_ptr);
            (void)result; // Suppress unused variable warning
        }
    }

    // Fuzzing Strategy 5: Test png_write_end
    if (png_ptr && info_ptr) {
        // Create a minimal IHDR
        png_uint_32 width = 1;
        png_uint_32 height = 1;
        int bit_depth = 8;
        int color_type = PNG_COLOR_TYPE_GRAY;
        int compression_type = PNG_COMPRESSION_TYPE_DEFAULT;
        int filter_type = PNG_FILTER_TYPE_DEFAULT;
        int interlace_type = PNG_INTERLACE_NONE;

        png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, 
                     interlace_type, compression_type, filter_type);

        // Write IHDR
        png_write_info(png_ptr, info_ptr);

        // Write a minimal row of data (1 byte for 1x1 gray image)
        // png_write_rows expects png_byte ** (array of pointers)
        png_byte row_data[1];
        row_data[0] = 0; // Black pixel
        png_byte *row_ptr = row_data;
        
        png_write_rows(png_ptr, &row_ptr, 1);

        // Now it is safe to call png_write_end
        png_write_end(png_ptr, info_ptr);
    }

    // Cleanup
    if (info_ptr) {
        png_destroy_info_struct(png_ptr, &info_ptr);
    }
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
    }

    return 0;
}