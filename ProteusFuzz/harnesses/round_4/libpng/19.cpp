// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <new>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    FILE *fp = nullptr;

    // Initialize libpng structures
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Use a custom error handler to prevent abort on fuzzing errors
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Write dummy file to simulate output
    write_dummy_file(Data, Size);
    fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Set minimal required image parameters
    png_uint_32 width = 1, height = 1;
    int bit_depth = 8, color_type = PNG_COLOR_TYPE_RGB;
    int interlace_type = PNG_INTERLACE_NONE;
    int compression_type = PNG_COMPRESSION_TYPE_DEFAULT;
    int filter_type = PNG_FILTER_TYPE_DEFAULT;

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);

    // Fuzzing logic: Call target APIs in various combinations

    // 1. png_write_info_before_PLTE
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // 2. png_write_info
    png_write_info(png_ptr, info_ptr);

    // 3. png_set_rows
    if (Size > 16) {
        size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        if (row_bytes > 0) {
            png_byte **row_pointers = static_cast<png_byte **>(malloc(sizeof(png_byte *) * height));
            if (row_pointers) {
                for (png_uint_32 y = 0; y < height; ++y) {
                    row_pointers[y] = static_cast<png_byte *>(malloc(row_bytes));
                    if (row_pointers[y]) {
                        memset(row_pointers[y], 0, row_bytes);
                    }
                }
                png_set_rows(png_ptr, info_ptr, row_pointers);
                
                // Call png_set_invalid to mark rows as invalid after setting
                png_set_invalid(png_ptr, info_ptr, PNG_INFO_IDAT);

                // Clean up row pointers manually to test png_set_invalid behavior
                for (png_uint_32 y = 0; y < height; ++y) {
                    free(row_pointers[y]);
                }
                free(row_pointers);
            }
        }
    }

    // 4. png_write_end
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    fclose(fp);
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_write_struct(&png_ptr, nullptr);

    return 0;
}