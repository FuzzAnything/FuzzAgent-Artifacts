// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_read_user_transform_fn at pngrtran.c:1099:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_set_read_status_fn at pngread.c:945:1 in png.h
// png_set_shift at pngtrans.c:83:1 in png.h
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

// Dummy callback functions for progressive reading
// Note: The signatures must match the typedefs in png.h exactly.
// png_progressive_info_ptr: void (*)(png_structp, png_infop)
static void progressive_info_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

// png_progressive_row_ptr: void (*)(png_structp, png_bytep, png_uint_32, int)
static void progressive_row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) {
    (void)png_ptr;
    (void)new_row;
    (void)row_num;
    (void)pass;
}

// png_progressive_end_ptr: void (*)(png_structp, png_infop)
static void progressive_end_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

// Dummy callback for read status
// png_read_status_ptr: void (*)(png_structp, png_uint_32, int)
static void read_status_callback(png_structp png_ptr, png_uint_32 row_number, int pass) {
    (void)png_ptr;
    (void)row_number;
    (void)pass;
}

// Dummy user transform callback
// png_user_transform_ptr: void (*)(png_structp, png_row_infop, png_bytep)
static void user_transform_callback(png_structp png_ptr, png_row_infop row_info, png_bytep row) {
    (void)png_ptr;
    (void)row_info;
    (void)row;
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Setup error handling to prevent crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Use the input data to configure the fuzzing scenario
    // We use a dummy file for the read operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initialize the dummy file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Fuzz target 1: png_set_progressive_read_fn
    // Fixed signature to match png_progressive_info_ptr (no 'pass' argument)
    png_set_progressive_read_fn(png_ptr, NULL, progressive_info_callback, progressive_row_callback, progressive_end_callback);

    // Fuzz target 2: png_set_read_status_fn
    png_set_read_status_fn(png_ptr, read_status_callback);

    // Fuzz target 3: png_set_shift
    png_color_8 shift_vals;
    // Use input data to vary shift values safely
    shift_vals.red = (png_byte)(Data[0] % 9);
    shift_vals.green = (png_byte)(Data[1 % Size] % 9);
    shift_vals.blue = (png_byte)(Data[2 % Size] % 9);
    png_set_shift(png_ptr, &shift_vals);

    // Fuzz target 4: png_set_read_user_transform_fn
    png_set_read_user_transform_fn(png_ptr, user_transform_callback);

    // Fuzz target 5: png_set_bgr
    png_set_bgr(png_ptr);

    // Fuzz target 6: png_start_read_image
    // Note: This requires proper initialization which might fail with dummy data,
    // but we wrap it in setjmp to handle errors gracefully.
    png_start_read_image(png_ptr);

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}