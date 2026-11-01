// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_progressive_frame_fn at pngpread.c:1125:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
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
#include <cstring>
#include <cstdio>
#include <cstdlib>

// Callback for progressive row data
// Signature: void (png_structp, png_bytep, png_uint_32, int)
static void dummy_progressive_row_fn(png_structp png_ptr, png_bytep new_row,
                                     png_uint_32 row_num, int pass) {
    (void)png_ptr;
    (void)new_row;
    (void)row_num;
    (void)pass;
}

// Callback for progressive info (header)
// Signature: void (png_structp, png_infop)
static void dummy_progressive_info_fn(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

// Callback for progressive end
// Signature: void (png_structp, png_infop)
static void dummy_progressive_end_fn(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

// Callback for APNG frame info: void (png_structp, png_uint_32)
static void dummy_frame_info_fn(png_structp png_ptr, png_uint_32 frame_num) {
    (void)png_ptr;
    (void)frame_num;
}

// Callback for APNG frame end: void (png_structp, png_uint_32)
static void dummy_frame_end_fn(png_structp png_ptr, png_uint_32 frame_num) {
    (void)png_ptr;
    (void)frame_num;
}

// Callback for read row status: void (png_structp, png_uint_32, int)
static void dummy_read_row_fn(png_structp png_ptr, png_uint_32 row_num,
                              int pass) {
    (void)png_ptr;
    (void)row_num;
    (void)pass;
}

static void dummy_error_fn(png_structp png_ptr, const char *error_message) {
    (void)png_ptr;
    (void)error_message;
}

static void dummy_warning_fn(png_structp png_ptr, const char *warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_set_error_fn(png_ptr, NULL, dummy_error_fn, dummy_warning_fn);

    // Test png_set_progressive_frame_fn
    // Signature: void (png_structp, png_uint_32)
    png_set_progressive_frame_fn(png_ptr, dummy_frame_info_fn, dummy_frame_end_fn);

    // Test png_get_user_transform_ptr (should be NULL initially)
    void *user_ptr = png_get_user_transform_ptr(png_ptr);
    (void)user_ptr;

    // Test png_get_progressive_ptr (should be NULL initially)
    void *prog_ptr = png_get_progressive_ptr(png_ptr);
    (void)prog_ptr;

    // Setup progressive read functions to allow png_process_data to work
    // Signature: png_set_progressive_read_fn(png_ptr, io_ptr, info_fn, row_fn, end_fn)
    // info_fn: void (png_structp, png_infop)
    // row_fn: void (png_structp, png_bytep, png_uint_32, int)
    // end_fn: void (png_structp, png_infop)
    png_set_progressive_read_fn(png_ptr, (void*)Data, 
                                dummy_progressive_info_fn, 
                                dummy_progressive_row_fn, 
                                dummy_progressive_end_fn);

    // Test png_process_data with input data
    if (Size > 0) {
        png_process_data(png_ptr, info_ptr, const_cast<png_byte*>(Data), Size);
    }

    // Attempt to start reading image (may fail if data is invalid, but tests the path)
    // This function is expected to be called after update_info, which might not happen
    // with random data, so we wrap in setjmp to catch errors.
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred, clean up and return
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Try to call png_start_read_image. This might trigger an error if not properly initialized.
    // We wrap it again to be safe, though the previous setjmp should catch it.
    png_start_read_image(png_ptr);

    // Test png_progressive_combine_row
    // We need some dummy row data. Allocate a small buffer.
    png_byte dummy_row[256];
    memset(dummy_row, 0, sizeof(dummy_row));
    png_progressive_combine_row(png_ptr, dummy_row, dummy_row);
    png_progressive_combine_row(png_ptr, dummy_row, NULL);

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}