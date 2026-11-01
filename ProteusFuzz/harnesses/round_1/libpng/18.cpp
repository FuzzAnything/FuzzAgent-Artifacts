// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_set_invert_alpha at pngtrans.c:236:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_tRNS_to_alpha at pngrtran.c:969:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <vector>

static void png_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Suppress errors during fuzzing to prevent aborts
}

static void png_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Suppress warnings during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to a temporary file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize PNG read structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set custom error and warning handlers to prevent aborts
    png_set_error_fn(png_ptr, NULL, png_error_callback, png_warning_callback);

    // Use setjmp to handle PNG errors gracefully
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
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

    // Read the PNG signature and info
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    // Close file
    fclose(fp);

    // Re-open for transformation testing (since read_png consumes the stream)
    fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Re-initialize for transformation testing
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    png_set_error_fn(png_ptr, NULL, png_error_callback, png_warning_callback);

    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read info header
    png_read_info(png_ptr, info_ptr);

    // Apply the target transformation functions
    // 1. png_set_scale_16
    png_set_scale_16(png_ptr);

    // 2. png_set_gray_to_rgb
    png_set_gray_to_rgb(png_ptr);

    // 3. png_set_packing
    png_set_packing(png_ptr);

    // 4. png_set_invert_alpha
    png_set_invert_alpha(png_ptr);

    // 5. png_set_swap
    png_set_swap(png_ptr);

    // 6. png_set_tRNS_to_alpha
    png_set_tRNS_to_alpha(png_ptr);

    // Read the image with transformations
    png_read_image(png_ptr, NULL);

    // Read end info
    png_read_end(png_ptr, NULL);

    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}