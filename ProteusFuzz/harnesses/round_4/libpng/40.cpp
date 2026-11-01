// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_convert_from_time_t at pngwrite.c:531:1 in png.h
// png_convert_from_struct_tm at pngwrite.c:518:1 in png.h
// png_set_tIME at pngset.c:1118:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
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
#include <png.h>
#include <ctime>
#include <setjmp.h>

static jmp_buf jmp_buffer;
static png_structp global_png_ptr = NULL;

static void dummy_write(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush(png_structp png_ptr) {
    (void)png_ptr;
}

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    if (global_png_ptr) {
        longjmp(jmp_buffer, 1);
    }
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract some values from input to drive fuzzing logic
    uint32_t seed = 0;
    if (Size >= 4) {
        seed = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Set up error handling to prevent crashes
    png_set_error_fn(png_ptr, NULL, dummy_error, dummy_warning);
    global_png_ptr = png_ptr;

    // Set up dummy write and flush callbacks
    png_set_write_fn(png_ptr, NULL, dummy_write, dummy_flush);

    // Initialize png_time
    png_time mod_time;
    memset(&mod_time, 0, sizeof(mod_time));

    // Test png_convert_from_time_t
    time_t ttime = (time_t)(seed & 0x7FFFFFFF);
    png_convert_from_time_t(&mod_time, ttime);

    // Test png_convert_from_struct_tm
    struct tm tm_time;
    memset(&tm_time, 0, sizeof(tm_time));
    tm_time.tm_year = (seed >> 8) & 0x7FFF; // Adjust to 1900-based
    tm_time.tm_mon = (seed >> 16) & 0x0F;   // 0-11
    tm_time.tm_mday = (seed >> 20) & 0x1F;  // 1-31
    tm_time.tm_hour = (seed >> 24) & 0x17;  // 0-23
    tm_time.tm_min = (seed >> 28) & 0x3F;   // 0-59
    tm_time.tm_sec = (seed >> 4) & 0x3F;    // 0-59
    png_convert_from_struct_tm(&mod_time, &tm_time);

    // Test png_set_tIME
    png_set_tIME(png_ptr, info_ptr, &mod_time);

    // Test png_get_tIME
    png_time retrieved_time;
    memset(&retrieved_time, 0, sizeof(retrieved_time));
    png_timep mod_time_ptr = &retrieved_time;
    png_uint_32 flags = png_get_tIME(png_ptr, info_ptr, &mod_time_ptr);
    (void)flags;

    // Test png_convert_to_rfc1123_buffer
    char rfc_buffer[29];
    memset(rfc_buffer, 0, sizeof(rfc_buffer));
    int rfc_result = png_convert_to_rfc1123_buffer(rfc_buffer, &mod_time);
    (void)rfc_result;

    // Test png_write_end with and without info_ptr
    // Use setjmp to catch errors from png_write_end
    if (setjmp(jmp_buffer) == 0) {
        // First, try with info_ptr. This will likely fail because no IDATs were written.
        // The error handler will longjmp back here.
        png_write_end(png_ptr, info_ptr);
    }

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    global_png_ptr = NULL;

    return 0;
}