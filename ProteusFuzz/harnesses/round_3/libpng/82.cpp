// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_eXIf_1 at pngset.c:319:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_set_cICP at pngset.c:136:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
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
#include <stdlib.h>
#include <string.h>
#include <cstddef>

static void write_to_file(const uint8_t *data, size_t size, FILE *fp) {
    if (fp && data && size > 0) {
        fwrite(data, 1, size, fp);
    }
}

static void free_image(png_byte **row_pointers, png_uint_32 height) {
    if (row_pointers) {
        for (png_uint_32 i = 0; i < height; ++i) {
            if (row_pointers[i]) {
                free(row_pointers[i]);
                row_pointers[i] = nullptr;
            }
        }
        free(row_pointers);
    }
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    FILE *fp = nullptr;
    int fuzz_result = 0;

    // Create a dummy file for writing
    fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;

    // Initialize PNG structures
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Parse input data to determine image parameters
    // We use a simple heuristic: first 4 bytes for width, height, bit_depth, color_type
    // If size is too small, use defaults
    png_uint_32 width = 1;
    png_uint_32 height = 1;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;

    if (Size >= 8) {
        width = Data[0] + 1;
        height = Data[1] + 1;
        bit_depth = (Data[2] % 8) + 1; // 1 to 8
        if (bit_depth == 2) bit_depth = 4; // PNG doesn't support 2-bit depth directly in all contexts, adjust to 4
        if (bit_depth > 8) bit_depth = 8;
        
        color_type = Data[3] % 10; // 0-9, valid color types are 0, 2, 3, 4, 6
        if (color_type == 1 || color_type == 5 || color_type == 7 || color_type == 8 || color_type == 9) {
            color_type = PNG_COLOR_TYPE_RGB; // Fallback
        }
    }

    // Set image parameters
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Allocate image data
    png_byte **row_pointers = nullptr;
    png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    
    if (row_bytes > 0 && height > 0) {
        row_pointers = (png_byte **)malloc(sizeof(png_byte *) * height);
        if (!row_pointers) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            fclose(fp);
            return 0;
        }
        for (png_uint_32 i = 0; i < height; ++i) {
            row_pointers[i] = (png_byte *)malloc(row_bytes);
            if (!row_pointers[i]) {
                free_image(row_pointers, i); // Clean up partial allocation
                free(row_pointers);
                row_pointers = nullptr;
                png_destroy_write_struct(&png_ptr, &info_ptr);
                fclose(fp);
                return 0;
            }
            // Fill with fuzz data
            size_t data_offset = 4 + (i * row_bytes);
            if (data_offset + row_bytes <= Size) {
                memcpy(row_pointers[i], Data + data_offset, row_bytes);
            } else {
                memset(row_pointers[i], 0, row_bytes);
            }
        }
    }

    // --- Fuzzing Target API Functions ---

    // 1. png_set_eXIf_1
    // Prepare EXIF data from input if available
    if (Size > 10) {
        png_uint_32 num_exif = (Data[4] % 100) + 1;
        png_byte *exif_data = nullptr;
        if (num_exif > 0 && num_exif < Size - 10) {
            exif_data = (png_byte *)(Data + 10);
        } else {
            exif_data = (png_byte *)malloc(10);
            if (exif_data) {
                memset(exif_data, 0, 10);
                num_exif = 10;
            }
        }
        if (exif_data) {
            png_set_eXIf_1(png_ptr, info_ptr, num_exif, exif_data);
            if (Size <= 10) free(exif_data);
        }
    }

    // 2. png_write_info_before_PLTE
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // 6. png_set_cICP
    // Extract parameters from input
    if (Size > 14) {
        png_byte colour_primaries = Data[10] % 256;
        png_byte transfer_function = Data[11] % 256;
        png_byte matrix_coefficients = Data[12] % 256;
        png_byte video_full_range_flag = Data[13] % 2;
        png_set_cICP(png_ptr, info_ptr, colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag);
    }

    // 5. png_set_rows
    if (row_pointers) {
        png_set_rows(png_ptr, info_ptr, row_pointers);
    }

    // 4. png_write_info
    png_write_info(png_ptr, info_ptr);

    // 3. png_write_image
    if (row_pointers) {
        png_write_image(png_ptr, row_pointers);
    }

    // Cleanup
    if (row_pointers) {
        free_image(row_pointers, height);
    }
    
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    return 0;
}