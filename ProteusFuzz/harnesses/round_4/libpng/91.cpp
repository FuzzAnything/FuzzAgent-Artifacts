// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_set_pHYs at pngset.c:723:1 in png.h
// png_set_sCAL_s at pngset.c:598:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_sCAL_fixed at pngset.c:694:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
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
#include <stdio.h>

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return NULL;
    }
    return png_ptr;
}

static png_infop create_png_info(png_structp png_ptr) {
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        return NULL;
    }
    return info_ptr;
}

// Custom error handler to prevent libpng from aborting during fuzzing
static void fuzz_error_handler(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    // Do nothing to prevent crash, just return
}

extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to avoid buffer overflows
    if (Size < 15) {
        return 0;
    }

    png_structp png_ptr = create_png_struct();
    if (!png_ptr) {
        return 0;
    }

    // Set custom error handler to prevent crashes
    png_set_error_fn(png_ptr, NULL, fuzz_error_handler, NULL);

    png_infop info_ptr = create_png_info(png_ptr);
    if (!info_ptr) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Set custom error handler for info_ptr as well
    png_set_error_fn(png_ptr, NULL, fuzz_error_handler, NULL);

    // Set up a dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Extract data for fuzzing parameters safely
    // Valid unit values for sCAL are 1 (meter) and 2 (other)
    // We'll use modulo to keep it in valid range
    int unit = (Data[0] % 2) + 1; // 1 or 2
    int srgb_intent = Data[1] % 4; // 0 to 3
    int unit_type = Data[2] % 2; // 0 or 1

    // Fixed-point values (avoiding extreme values that might cause issues)
    png_fixed_point width_fixed = (png_fixed_point)(Data[3] * 100000 + Data[4]);
    png_fixed_point height_fixed = (png_fixed_point)(Data[5] * 100000 + Data[6]);
    png_fixed_point gamma_fixed = (png_fixed_point)(Data[7] * 100000 + Data[8]);

    // Resolutions
    png_uint_32 res_x = (png_uint_32)(Data[9] * 256 + Data[10]);
    png_uint_32 res_y = (png_uint_32)(Data[11] * 256 + Data[12]);

    // Fuzz png_set_sCAL_fixed with valid unit
    png_set_sCAL_fixed(png_ptr, info_ptr, unit, width_fixed, height_fixed);

    // Fuzz png_set_sRGB_gAMA_and_cHRM
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);

    // Fuzz png_set_gAMA_fixed
    png_set_gAMA_fixed(png_ptr, info_ptr, gamma_fixed);

    // Fuzz png_set_pHYs
    png_set_pHYs(png_ptr, info_ptr, res_x, res_y, unit_type);

    // Prepare string inputs for png_set_sCAL_s with valid format
    char swidth[16], sheight[16];
    snprintf(swidth, sizeof(swidth), "%.6f", (double)Data[13] / 10.0);
    snprintf(sheight, sizeof(sheight), "%.6f", (double)Data[14] / 10.0);
    
    // Call png_set_sCAL_s with valid unit (1 or 2)
    png_set_sCAL_s(png_ptr, info_ptr, unit, swidth, sheight);

    // Create minimal image data for png_write_png
    png_bytep row = (png_bytep)malloc(4);
    if (row) {
        memset(row, 0, 4);
        
        // Set IHDR with valid dimensions and color type
        png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, 
                     PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        
        // Create row pointers array
        png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep));
        if (row_pointers) {
            row_pointers[0] = row;
            
            // Write the image
            png_write_info(png_ptr, info_ptr);
            png_write_image(png_ptr, row_pointers);
            png_write_end(png_ptr, info_ptr);
            
            free(row_pointers);
        }
        free(row);
    }

    fclose(fp);
    cleanup_png(png_ptr, info_ptr);
    return 0;
}