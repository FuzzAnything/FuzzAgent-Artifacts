// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_png at pngread.c:957:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_get_y_offset_pixels at pngget.c:304:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_get_y_offset_microns at pngget.c:264:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
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
#include <cstddef>
#include <cstring>
#include <cstdio>

static void
fuzzer_cleanup(png_structp png_ptr, png_infop info_ptr, png_bytep *row_pointers) {
    if (row_pointers) {
        if (png_ptr && info_ptr) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        } else if (png_ptr) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
        } else if (info_ptr) {
            png_destroy_info_struct(NULL, &info_ptr);
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for libpng to read from
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize libpng read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Open the dummy file for reading
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the PNG header and info
    png_read_info(png_ptr, info_ptr);

    // Allocate row pointers if needed for png_read_png
    png_bytep *row_pointers = NULL;
    png_uint_32 width, height;
    int bit_depth, color_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

    if (width > 0 && height > 0) {
        row_pointers = (png_bytep *)png_malloc(png_ptr, height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
        }
    }

    // Fuzz the target API functions
    // 1. png_get_y_offset_microns
    png_int_32 y_offset_microns = png_get_y_offset_microns(png_ptr, info_ptr);
    (void)y_offset_microns;

    // 2. png_get_x_offset_pixels
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)x_offset_pixels;

    // 3. png_get_x_offset_microns
    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr, info_ptr);
    (void)x_offset_microns;

    // 4. png_read_png
    // Only call if row_pointers are allocated to avoid segmentation fault
    if (row_pointers) {
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    }

    // 5. png_create_info_struct (already called above, but we can try again to test robustness)
    png_infop info_ptr2 = png_create_info_struct(png_ptr);
    if (info_ptr2) {
        png_destroy_info_struct(png_ptr, &info_ptr2);
    }

    // 6. png_get_y_offset_pixels
    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
    (void)y_offset_pixels;

    // Cleanup
    fclose(fp);
    fuzzer_cleanup(png_ptr, info_ptr, row_pointers);

    return 0;
}