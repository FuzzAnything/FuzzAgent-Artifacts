// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_set_sCAL_fixed at pngset.c:694:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
// png_set_oFFs at pngset.c:465:1 in png.h
// png_set_oFFs at pngset.c:465:1 in png.h
// png_set_sCAL_fixed at pngset.c:694:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
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
#include <setjmp.h>

// Suppress warnings for unused parameters in some cases
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"

static void dummy_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // In fuzzing, we typically don't want to abort on errors
}

static void dummy_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        // If png_ptr is null but info_ptr exists (shouldn't happen normally), 
        // we can't safely destroy it without the struct.
    }
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file to simulate reading a PNG
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    // Initialize PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set error handling to avoid aborting
    png_set_error_fn(png_ptr, nullptr, dummy_error_callback, dummy_warning_callback);

    // Use setjmp to handle errors gracefully
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Initialize I/O
    f = fopen(filename, "rb");
    if (!f) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, f);

    // Read the PNG header and info
    png_read_info(png_ptr, info_ptr);

    // Close file after reading info
    fclose(f);

    // --- Fuzzing Target API Functions ---

    // 1. png_get_pixel_aspect_ratio_fixed
    png_fixed_point aspect_ratio = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)aspect_ratio;

    // 2. png_get_oFFs
    png_int_32 offset_x, offset_y;
    int unit_type;
    png_uint_32 offs_result = png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);
    (void)offs_result;

    // 3. png_get_x_offset_inches_fixed
    png_fixed_point x_offset_inches = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_inches;

    // 4. png_set_sCAL_fixed
    // Use 1 for metre (PNG_sCAL_UNIT_METRE is typically 1)
    int unit = 1; 
    png_fixed_point width = 100000; // 1.0 meter in fixed point
    png_fixed_point height = 100000;
    png_set_sCAL_fixed(png_ptr, info_ptr, unit, width, height);

    // 5. png_get_y_offset_inches_fixed
    png_fixed_point y_offset_inches = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)y_offset_inches;

    // 6. png_set_oFFs
    // Use 0 for pixel (PNG_OFFSET_PIXEL is typically 0)
    png_set_oFFs(png_ptr, info_ptr, 10, 20, 0);

    // Additional exploration: Try with different unit types and values from input
    if (Size >= 9) {
        int32_t val1 = *(const int32_t*)(Data);
        int32_t val2 = *(const int32_t*)(Data + 4);
        int unit_val = Data[8] % 3; // 0, 1, or 2

        png_set_oFFs(png_ptr, info_ptr, val1, val2, unit_val);
        
        // Try setting sCAL with values from input (ensure positive)
        if (val1 > 0 && val2 > 0) {
            png_set_sCAL_fixed(png_ptr, info_ptr, unit_val, val1, val2);
        }
    }

    // Clean up
    cleanup_png(png_ptr, info_ptr);

    // Remove dummy file
    remove(filename);

    return 0;
}

#pragma GCC diagnostic pop