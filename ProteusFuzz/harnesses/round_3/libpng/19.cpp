// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_convert_from_time_t at pngwrite.c:531:1 in png.h
// png_set_tIME at pngset.c:1118:1 in png.h
// png_convert_to_rfc1123_buffer at png.c:739:1 in png.h
// png_convert_from_struct_tm at pngwrite.c:518:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
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
#include <stdint.h>
#include <string.h>
#include <time.h>

static void write_data_to_file(const uint8_t *Data, size_t Size, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize libpng structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Prepare dummy file for writing
    const char *filename = "./dummy_file";
    write_data_to_file(Data, Size, filename);

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize PNG write
    png_init_io(png_ptr, fp);

    // Create a minimal PNG header (1x1 grayscale)
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Test png_convert_from_time_t
    time_t ttime = (Size > 0) ? (time_t)Data[0] : 0;
    png_time png_time_struct;
    png_convert_from_time_t(&png_time_struct, ttime);

    // Test png_set_tIME
    png_set_tIME(png_ptr, info_ptr, &png_time_struct);

    // Test png_convert_to_rfc1123_buffer
    char rfc_buffer[29];
    png_convert_to_rfc1123_buffer(rfc_buffer, &png_time_struct);

    // Test png_convert_from_struct_tm
    struct tm tm_time = {0};
    tm_time.tm_year = (Size > 1) ? Data[1] : 0;
    tm_time.tm_mon = (Size > 2) ? Data[2] : 0;
    tm_time.tm_mday = (Size > 3) ? Data[3] : 0;
    tm_time.tm_hour = (Size > 4) ? Data[4] : 0;
    tm_time.tm_min = (Size > 5) ? Data[5] : 0;
    tm_time.tm_sec = (Size > 6) ? Data[6] : 0;
    png_time png_time_struct2;
    png_convert_from_struct_tm(&png_time_struct2, &tm_time);

    // Write image data
    // png_write_image expects an array of pointers (png_byte**)
    png_byte row[1];
    row[0] = (Size > 7) ? Data[7] : 0;
    png_byte *row_ptr = row;
    png_write_image(png_ptr, &row_ptr);

    // Test png_get_tIME
    png_time *mod_time = NULL;
    png_uint_32 flag = png_get_tIME(png_ptr, info_ptr, &mod_time);
    (void)flag;

    // Finalize PNG writing
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}