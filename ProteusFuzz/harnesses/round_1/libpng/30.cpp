// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_invert_mono at pngtrans.c:249:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_invert_mono at pngtrans.c:249:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_invert_mono at pngtrans.c:249:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <setjmp.h>
#include <png.h>

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a dummy file for write operations
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) return 0;
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);

    // Test with NULL pointers (safe by design)
    png_write_rows(NULL, NULL, 0);
    png_read_row(NULL, NULL, NULL);
    png_set_swap_alpha(NULL);
    png_set_invert_mono(NULL);
    png_set_swap(NULL);
    png_read_rows(NULL, NULL, NULL, 0);

    // Prepare buffers
    const size_t max_rows = 10;
    const size_t row_size = 1024;
    png_byte *rows[max_rows];
    png_byte *display_rows[max_rows];
    for (size_t i = 0; i < max_rows; ++i) {
        rows[i] = (png_byte *)malloc(row_size);
        display_rows[i] = (png_byte *)malloc(row_size);
        if (!rows[i] || !display_rows[i]) {
            for (size_t j = 0; j < i; ++j) {
                free(rows[j]);
                free(display_rows[j]);
            }
            return 0;
        }
        memset(rows[i], 0, row_size);
        memset(display_rows[i], 0, row_size);
    }

    // Test write path
    png_structp write_png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (write_png) {
        // Set up error handling
        if (setjmp(png_jmpbuf(write_png))) {
            png_destroy_write_struct(&write_png, NULL);
            // Cleanup buffers
            for (size_t i = 0; i < max_rows; ++i) {
                free(rows[i]);
                free(display_rows[i]);
            }
            remove(dummy_file);
            return 0;
        }

        png_infop write_info = png_create_info_struct(write_png);
        if (write_info) {
            png_set_write_fn(write_png, NULL, dummy_write_fn, NULL);
            png_set_IHDR(write_png, write_info, 10, 10, 8, PNG_COLOR_TYPE_RGB,
                         PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                         PNG_FILTER_TYPE_DEFAULT);

            // Apply transformations
            png_set_swap_alpha(write_png);
            png_set_invert_mono(write_png);
            png_set_swap(write_png);

            png_write_info(write_png, write_info);

            // Write rows
            png_uint_32 num_rows_write = (Size % max_rows) + 1;
            png_write_rows(write_png, rows, num_rows_write);

            png_write_end(write_png, write_info);
            png_destroy_info_struct(write_png, &write_info);
        }
        png_destroy_write_struct(&write_png, NULL);
    }

    // Test read path
    fp = fopen(dummy_file, "rb");
    if (fp) {
        png_structp read_png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (read_png) {
            // Set up error handling
            if (setjmp(png_jmpbuf(read_png))) {
                png_destroy_read_struct(&read_png, NULL, NULL);
                fclose(fp);
                // Cleanup buffers
                for (size_t i = 0; i < max_rows; ++i) {
                    free(rows[i]);
                    free(display_rows[i]);
                }
                remove(dummy_file);
                return 0;
            }

            png_infop read_info = png_create_info_struct(read_png);
            if (read_info) {
                png_set_read_fn(read_png, fp, dummy_read_fn);

                // Apply transformations
                png_set_swap_alpha(read_png);
                png_set_invert_mono(read_png);
                png_set_swap(read_png);

                png_read_info(read_png, read_info);

                // Read rows
                png_uint_32 num_rows_read = (Size % max_rows) + 1;
                png_read_rows(read_png, rows, display_rows, num_rows_read);

                // Also test single row read
                png_read_row(read_png, rows[0], display_rows[0]);

                png_read_end(read_png, read_info);
                png_destroy_info_struct(read_png, &read_info);
            }
            png_destroy_read_struct(&read_png, NULL, NULL);
        }
        fclose(fp);
    }

    // Cleanup
    for (size_t i = 0; i < max_rows; ++i) {
        free(rows[i]);
        free(display_rows[i]);
    }

    // Remove dummy file
    remove(dummy_file);

    return 0;
}