// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_rows at pngget.c:50:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Fuzz driver for libpng API functions: png_get_rows, png_get_interlace_type,
// png_get_signature, png_get_channels, png_get_color_type, png_get_bit_depth.

static void custom_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Ignore errors during fuzzing to prevent aborts
    (void)png_ptr;
    (void)error_msg;
}

static void custom_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Ignore warnings during fuzzing
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file to hold the input data
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, custom_error_fn, custom_warning_fn);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling to prevent crashes on malformed input
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Open the file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read the PNG info
    png_read_info(png_ptr, info_ptr);

    // Try to read the full image to populate row_pointers for png_get_rows
    // This may fail on invalid data, which is expected in fuzzing
    png_byte **row_pointers = NULL;
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    if (width > 0 && height > 0 && row_bytes > 0 && width * height <= 10000000) {
        row_pointers = (png_byte **)malloc(height * sizeof(png_byte *));
        if (row_pointers) {
            for (png_uint_32 i = 0; i < height; ++i) {
                row_pointers[i] = (png_byte *)malloc(row_bytes);
                if (!row_pointers[i]) {
                    for (png_uint_32 j = 0; j < i; ++j) {
                        free(row_pointers[j]);
                    }
                    free(row_pointers);
                    row_pointers = NULL;
                    break;
                }
            }
            if (row_pointers) {
                png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, row_pointers);
            }
        }
    }

    // Invoke target API functions
    png_byte **rows = png_get_rows(png_ptr, info_ptr);
    (void)rows; // Use the result to avoid unused variable warnings

    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    (void)interlace_type;

    const png_byte *signature = png_get_signature(png_ptr, info_ptr);
    (void)signature;

    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels;

    png_byte color_type_result = png_get_color_type(png_ptr, info_ptr);
    (void)color_type_result;

    png_byte bit_depth_result = png_get_bit_depth(png_ptr, info_ptr);
    (void)bit_depth_result;

    // Cleanup
    if (row_pointers) {
        for (png_uint_32 i = 0; i < height; ++i) {
            free(row_pointers[i]);
        }
        free(row_pointers);
    }

    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    remove(filename);

    return 0;
}