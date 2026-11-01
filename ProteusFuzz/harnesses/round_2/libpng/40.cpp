// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_eXIf_1 at pngset.c:319:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_cICP at pngset.c:136:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
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

static void write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Simple write callback that discards data
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void fuzz_write_info(const uint8_t *Data, size_t Size) {
    if (Size < 1) return;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return;
    }

    // Set a dummy file for output
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }

    png_set_write_fn(png_ptr, f, write_data, NULL);

    // Set basic image info
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Call png_write_info
    png_write_info(png_ptr, info_ptr);

    // Cleanup
    fclose(f);
    png_destroy_write_struct(&png_ptr, &info_ptr);
}

static void fuzz_write_image(const uint8_t *Data, size_t Size) {
    if (Size < 1) return;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return;
    }

    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }

    png_set_write_fn(png_ptr, f, write_data, NULL);

    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    // Allocate a single row
    png_byte row[3]; // RGB
    memset(row, 0, 3);
    png_byte *row_ptr = row;
    png_byte **image = &row_ptr;

    // Call png_write_image
    png_write_image(png_ptr, image);

    // Cleanup
    fclose(f);
    png_destroy_write_struct(&png_ptr, &info_ptr);
}

static void fuzz_write_info_before_PLTE(const uint8_t *Data, size_t Size) {
    if (Size < 1) return;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return;
    }

    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }

    png_set_write_fn(png_ptr, f, write_data, NULL);

    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Call png_write_info_before_PLTE
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // Cleanup
    fclose(f);
    png_destroy_write_struct(&png_ptr, &info_ptr);
}

static void fuzz_set_eXIf_1(const uint8_t *Data, size_t Size) {
    if (Size < 1) return;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return;
    }

    // Prepare EXIF data from input
    png_uint_32 num_exif = Size > 0 ? Size : 1;
    png_byte *exif = const_cast<png_byte*>(Data);

    // Call png_set_eXIf_1
    png_set_eXIf_1(png_ptr, info_ptr, num_exif, exif);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
}

static void fuzz_set_cICP(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return;
    }

    png_byte colour_primaries = Data[0];
    png_byte transfer_function = Data[1];
    png_byte matrix_coefficients = Data[2];
    png_byte video_full_range_flag = Data[3];

    // Call png_set_cICP
    png_set_cICP(png_ptr, info_ptr, colour_primaries, transfer_function,
                 matrix_coefficients, video_full_range_flag);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
}

static void fuzz_set_rows(const uint8_t *Data, size_t Size) {
    if (Size < 1) return;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return;
    }

    // Create a simple row pointer array
    png_byte row[3];
    memset(row, 0, 3);
    png_byte *row_ptr = row;
    png_byte **row_pointers = &row_ptr;

    // Call png_set_rows
    png_set_rows(png_ptr, info_ptr, row_pointers);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz different functions based on input
    switch (Data[0] % 6) {
        case 0:
            fuzz_write_info(Data, Size);
            break;
        case 1:
            fuzz_write_image(Data, Size);
            break;
        case 2:
            fuzz_write_info_before_PLTE(Data, Size);
            break;
        case 3:
            fuzz_set_eXIf_1(Data, Size);
            break;
        case 4:
            fuzz_set_cICP(Data, Size);
            break;
        case 5:
            fuzz_set_rows(Data, Size);
            break;
    }

    return 0;
}