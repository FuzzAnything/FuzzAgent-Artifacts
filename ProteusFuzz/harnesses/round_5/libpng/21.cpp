// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_read_status_fn at pngread.c:945:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_write_status_fn at pngwrite.c:1348:1 in png.h
// png_set_write_user_transform_fn at pngwrite.c:1360:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <png.h>

static void dummy_read_status_fn(png_structp png_ptr, png_uint_32 row_number, int pass) {
    (void)png_ptr;
    (void)row_number;
    (void)pass;
}

static void dummy_write_status_fn(png_structp png_ptr, png_uint_32 row_number, int pass) {
    (void)png_ptr;
    (void)row_number;
    (void)pass;
}

static void dummy_write_user_transform_fn(png_structp png_ptr, png_row_infop row_info, png_bytep row) {
    (void)png_ptr;
    (void)row_info;
    (void)row;
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for I/O operations
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Reopen for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }

    // Test Read Path
    png_structp read_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (read_png_ptr) {
        png_infop read_info_ptr = png_create_info_struct(read_png_ptr);
        if (read_info_ptr) {
            png_init_io(read_png_ptr, fp);

            // Call target APIs for reading
            png_set_read_status_fn(read_png_ptr, dummy_read_status_fn);
            png_set_packing(read_png_ptr);

            // Test png_get_user_transform_ptr on read struct
            void *user_ptr = png_get_user_transform_ptr(read_png_ptr);
            (void)user_ptr;

            png_destroy_read_struct(&read_png_ptr, &read_info_ptr, NULL);
        } else {
            png_destroy_read_struct(&read_png_ptr, NULL, NULL);
        }
    }
    fclose(fp);

    // Test Write Path
    fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }

    png_structp write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (write_png_ptr) {
        png_infop write_info_ptr = png_create_info_struct(write_png_ptr);
        if (write_info_ptr) {
            png_init_io(write_png_ptr, fp);

            // Call target APIs for writing
            png_set_write_status_fn(write_png_ptr, dummy_write_status_fn);
            png_set_write_user_transform_fn(write_png_ptr, dummy_write_user_transform_fn);
            png_set_packing(write_png_ptr);

            // Test png_get_user_transform_ptr on write struct
            void *user_ptr = png_get_user_transform_ptr(write_png_ptr);
            (void)user_ptr;

            png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        } else {
            png_destroy_write_struct(&write_png_ptr, NULL);
        }
    }
    fclose(fp);

    // Cleanup dummy file
    remove(filename);

    return 0;
}