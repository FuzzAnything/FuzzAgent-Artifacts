// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_cICP at pngset.c:136:1 in png.h
// png_set_eXIf_1 at pngset.c:319:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
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
    // Do not abort, just return to let fuzzer continue
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 20) {
        return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    png_byte **row_pointers = nullptr;
    png_uint_32 width = 0;
    png_uint_32 height = 0;
    png_byte bit_depth = 8;
    png_byte color_type = PNG_COLOR_TYPE_RGB;
    png_byte interlace_method = PNG_INTERLACE_NONE;
    png_byte compression_method = PNG_COMPRESSION_TYPE_DEFAULT;
    png_byte filter_method = PNG_FILTER_TYPE_BASE;

    // Parse some basic parameters from input
    width = (Data[0] << 8) | Data[1];
    height = (Data[2] << 8) | Data[3];
    bit_depth = Data[4];
    color_type = Data[5];
    interlace_method = Data[6];
    compression_method = Data[7];
    filter_method = Data[8];

    // Sanity checks to avoid invalid IHDR data
    if (width == 0 || height == 0 || width > 10000 || height > 10000) {
        return 0;
    }
    if (bit_depth == 0 || bit_depth > 16) {
        bit_depth = 8;
    }
    // Validate color_type against valid PNG constants
    if (color_type > PNG_COLOR_TYPE_PALETTE) {
        color_type = PNG_COLOR_TYPE_RGB;
    }
    if (interlace_method >= 2) {
        interlace_method = PNG_INTERLACE_NONE;
    }

    // Initialize PNG structures
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set error handling to avoid aborts
    png_set_error_fn(png_ptr, nullptr, dummy_error, dummy_warning);

    // Initialize write callbacks
    png_set_write_fn(png_ptr, nullptr, dummy_write, dummy_flush);

    // Set IHDR parameters
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_method, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_BASE);

    // Fuzz png_set_cICP
    if (Size >= 12) {
        png_byte colour_primaries = Data[9];
        png_byte transfer_function = Data[10];
        png_byte matrix_coefficients = Data[11];
        png_byte video_full_range_flag = (Size > 12) ? Data[12] : 0;
        png_set_cICP(png_ptr, info_ptr, colour_primaries, transfer_function,
                     matrix_coefficients, video_full_range_flag);
    }

    // Fuzz png_set_eXIf_1
    if (Size >= 14) {
        png_uint_32 num_exif = (Data[13] << 24) | (Data[14] << 16) | (Data[15] << 8) | Data[16];
        if (num_exif > 0 && Size > 17 + num_exif) {
            png_byte *exif_data = new png_byte[num_exif];
            std::memcpy(exif_data, Data + 17, num_exif);
            png_set_eXIf_1(png_ptr, info_ptr, num_exif, exif_data);
            delete[] exif_data;
        }
    }

    // Allocate row pointers and image data
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    if (row_bytes == 0) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    row_pointers = new png_byte *[height];
    for (png_uint_32 y = 0; y < height; ++y) {
        row_pointers[y] = new png_byte[row_bytes];
        // Fill with fuzzed data safely
        if (Size > 18) {
            uint8_t fill_byte = Data[18];
            std::memset(row_pointers[y], fill_byte, row_bytes);
            
            if (Size > 19) {
                size_t fill_size = Size - 19;
                for (size_t i = 0; i < row_bytes && i < fill_size; ++i) {
                    row_pointers[y][i] = Data[19 + (i % fill_size)];
                }
            }
        } else {
            std::memset(row_pointers[y], 0, row_bytes);
        }
    }

    // Call png_set_rows
    png_set_rows(png_ptr, info_ptr, row_pointers);

    // Call png_write_info_before_PLTE
    png_write_info_before_PLTE(png_ptr, info_ptr);

    // Call png_write_info
    png_write_info(png_ptr, info_ptr);

    // Call png_write_image
    png_write_image(png_ptr, row_pointers);

    // Cleanup
    for (png_uint_32 y = 0; y < height; ++y) {
        delete[] row_pointers[y];
    }
    delete[] row_pointers;

    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}