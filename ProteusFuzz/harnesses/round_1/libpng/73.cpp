// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_process_data_pause at pngpread.c:65:1 in png.h
// png_process_data_pause at pngpread.c:65:1 in png.h
// png_process_data_skip at pngpread.c:92:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <setjmp.h>
#include <cstdint>
#include <cstddef>

// Static callback functions for progressive reading
static void
dummy_info_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

static void
dummy_row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) {
    (void)png_ptr;
    (void)new_row;
    (void)row_num;
    (void)pass;
}

static void
dummy_end_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

static void
dummy_read_callback(png_structp png_ptr, png_bytep data, size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling to prevent crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Configure progressive read function
    png_set_progressive_read_fn(png_ptr, nullptr, dummy_info_callback, dummy_row_callback, dummy_end_callback);

    // Set a dummy read function to avoid issues with internal calls
    png_set_read_fn(png_ptr, nullptr, dummy_read_callback);

    // Allocate a buffer for the input data
    png_byte *buffer = const_cast<png_byte*>(reinterpret_cast<const png_byte*>(Data));

    // Fuzz png_process_data
    png_process_data(png_ptr, info_ptr, buffer, Size);

    // Fuzz png_process_data_pause
    png_process_data_pause(png_ptr, 1); // Save pending data
    png_process_data_pause(png_ptr, 0); // Do not save pending data

    // Fuzz png_process_data_skip (deprecated, but included for completeness)
    png_uint_32 skipped = png_process_data_skip(png_ptr);
    (void)skipped;

    // Fuzz png_get_progressive_ptr
    void *progressive_ptr = png_get_progressive_ptr(png_ptr);
    (void)progressive_ptr;

    // Fuzz png_progressive_combine_row
    png_byte old_row[256] = {0};
    png_byte new_row[256] = {0};
    png_progressive_combine_row(png_ptr, old_row, new_row);
    png_progressive_combine_row(png_ptr, old_row, nullptr);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}