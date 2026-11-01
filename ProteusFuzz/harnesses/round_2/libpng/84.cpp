// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_tIME at pngset.c:1118:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_convert_to_rfc1123_buffer at png.c:739:1 in png.h
// png_convert_from_struct_tm at pngwrite.c:518:1 in png.h
// png_convert_from_time_t at pngwrite.c:531:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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

// Custom error handler to prevent libpng from aborting
static void dummy_error_handler(png_structp png_ptr, const char *error_message) {
    (void)png_ptr;
    (void)error_message;
    // Do nothing, just return
}

static void dummy_warning_handler(png_structp png_ptr, const char *warning_message) {
    (void)png_ptr;
    (void)warning_message;
    // Do nothing, just return
}

static void fuzz_png_convert_from_struct_tm(const uint8_t *data, size_t size) {
    if (size < sizeof(struct tm)) return;
    
    struct tm ttime;
    memset(&ttime, 0, sizeof(ttime));
    
    // Parse data to fill struct tm
    if (size >= 1) ttime.tm_sec = data[0];
    if (size >= 2) ttime.tm_min = data[1];
    if (size >= 3) ttime.tm_hour = data[2];
    if (size >= 4) ttime.tm_mday = data[3];
    if (size >= 5) ttime.tm_mon = data[4];
    if (size >= 6) ttime.tm_year = data[5];
    if (size >= 7) ttime.tm_yday = data[6];
    if (size >= 8) ttime.tm_wday = data[7];
    if (size >= 9) ttime.tm_isdst = data[8];
    
    png_time ptime;
    memset(&ptime, 0, sizeof(ptime));
    
    png_convert_from_struct_tm(&ptime, &ttime);
}

static void fuzz_png_convert_from_time_t(const uint8_t *data, size_t size) {
    if (size < sizeof(time_t)) return;
    
    time_t ttime;
    memcpy(&ttime, data, sizeof(time_t));
    
    png_time ptime;
    memset(&ptime, 0, sizeof(ptime));
    
    png_convert_from_time_t(&ptime, ttime);
}

static void fuzz_png_get_tIME(const uint8_t *data, size_t size) {
    // Create minimal PNG structure for testing
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return;
    
    // Set custom error handler to prevent abort
    png_set_error_fn(png_ptr, NULL, dummy_error_handler, dummy_warning_handler);
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return;
    }
    
    // Create a minimal valid PNG file in memory
    unsigned char png_header[] = {
        0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A,
        0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52,
        0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
        0x08, 0x02, 0x00, 0x00, 0x00, 0x90, 0x77, 0x53, 0xDE,
        0x00, 0x00, 0x00, 0x0C, 0x49, 0x44, 0x41, 0x54,
        0x08, 0xD7, 0x63, 0xF8, 0xFF, 0xFF, 0x3F, 0x00,
        0x05, 0xFE, 0x02, 0xFE, 0xDC, 0xCC, 0x59, 0xE7,
        0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44,
        0xAE, 0x42, 0x60, 0x82
    };
    
    // Write to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return;
    }
    
    fwrite(png_header, 1, sizeof(png_header), fp);
    fclose(fp);
    
    // Reopen for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return;
    }
    
    png_init_io(png_ptr, fp);
    
    // Read PNG header and info
    png_read_info(png_ptr, info_ptr);
    
    png_time *mod_time = NULL;
    png_uint_32 result = png_get_tIME(png_ptr, info_ptr, &mod_time);
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}

static void fuzz_png_set_tIME(const uint8_t *data, size_t size) {
    // Create PNG structure for writing
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return;
    
    // Set custom error handler to prevent abort
    png_set_error_fn(png_ptr, NULL, dummy_error_handler, dummy_warning_handler);
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return;
    }
    
    // Create dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }
    
    png_init_io(png_ptr, fp);
    
    // Create png_time from fuzz data
    png_time mod_time;
    memset(&mod_time, 0, sizeof(mod_time));
    
    if (size >= 2) mod_time.year = data[0] | (data[1] << 8);
    if (size >= 3) mod_time.month = data[2];
    if (size >= 4) mod_time.day = data[3];
    if (size >= 5) mod_time.hour = data[4];
    if (size >= 6) mod_time.minute = data[5];
    if (size >= 7) mod_time.second = data[6];
    
    png_set_tIME(png_ptr, info_ptr, &mod_time);
    
    // Write minimal PNG to avoid errors
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, 
                 PNG_FILTER_TYPE_DEFAULT);
    
    png_byte row_data[3];
    row_data[0] = 255; row_data[1] = 0; row_data[2] = 0;
    
    png_byte *row_ptr = row_data;
    
    png_write_info(png_ptr, info_ptr);
    png_write_rows(png_ptr, &row_ptr, 1);
    png_write_end(png_ptr, info_ptr);
    
    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
}

static void fuzz_png_write_end(const uint8_t *data, size_t size) {
    // Create PNG structure for writing
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return;
    
    // Set custom error handler to prevent abort
    png_set_error_fn(png_ptr, NULL, dummy_error_handler, dummy_warning_handler);
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return;
    }
    
    // Create dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }
    
    png_init_io(png_ptr, fp);
    
    // Write minimal PNG structure
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, 
                 PNG_FILTER_TYPE_DEFAULT);
    
    png_byte row_data[3];
    row_data[0] = 255; row_data[1] = 0; row_data[2] = 0;
    
    png_byte *row_ptr = row_data;
    
    png_write_info(png_ptr, info_ptr);
    png_write_rows(png_ptr, &row_ptr, 1);
    
    // Call target function
    png_write_end(png_ptr, info_ptr);
    
    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
}

static void fuzz_png_convert_to_rfc1123_buffer(const uint8_t *data, size_t size) {
    // Create png_time from fuzz data
    png_time ptime;
    memset(&ptime, 0, sizeof(ptime));
    
    if (size >= 2) ptime.year = data[0] | (data[1] << 8);
    if (size >= 3) ptime.month = data[2];
    if (size >= 4) ptime.day = data[3];
    if (size >= 5) ptime.hour = data[4];
    if (size >= 6) ptime.minute = data[5];
    if (size >= 7) ptime.second = data[6];
    
    char out[29];
    memset(out, 0, sizeof(out));
    
    int result = png_convert_to_rfc1123_buffer(out, &ptime);
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz each target function
    fuzz_png_convert_from_struct_tm(Data, Size);
    fuzz_png_convert_from_time_t(Data, Size);
    fuzz_png_get_tIME(Data, Size);
    fuzz_png_set_tIME(Data, Size);
    fuzz_png_write_end(Data, Size);
    fuzz_png_convert_to_rfc1123_buffer(Data, Size);
    
    return 0;
}