// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_process_data_pause at pngpread.c:65:1 in png.h
// png_process_data_pause at pngpread.c:65:1 in png.h
// png_process_data_skip at pngpread.c:92:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
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

static void progressive_info_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

static void progressive_row_callback(png_structp png_ptr, png_bytep new_row,
                                     png_uint_32 row_num, int pass) {
    (void)png_ptr;
    (void)new_row;
    (void)row_num;
    (void)pass;
}

static void progressive_end_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

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

    // Set progressive read functions
    png_set_progressive_read_fn(png_ptr, (void*)Data,
                                progressive_info_callback,
                                progressive_row_callback,
                                progressive_end_callback);

    // Test png_get_progressive_ptr
    void *user_ptr = png_get_progressive_ptr(png_ptr);
    (void)user_ptr;

    // Feed data incrementally to png_process_data
    size_t offset = 0;
    size_t chunk_size = 1;

    while (offset < Size) {
        size_t remaining = Size - offset;
        size_t current_chunk = (chunk_size < remaining) ? chunk_size : remaining;
        
        // Call png_process_data
        png_process_data(png_ptr, info_ptr, (png_byte*)&Data[offset], current_chunk);

        // Test png_process_data_pause
        size_t paused_bytes = png_process_data_pause(png_ptr, 0);
        (void)paused_bytes;

        size_t paused_bytes_save = png_process_data_pause(png_ptr, 1);
        (void)paused_bytes_save;

        // Test png_process_data_skip (deprecated, returns 0)
        png_uint_32 skip_bytes = png_process_data_skip(png_ptr);
        (void)skip_bytes;

        // Test png_progressive_combine_row
        png_byte dummy_row[256];
        memset(dummy_row, 0, sizeof(dummy_row));
        
        const png_byte *new_row_data = (current_chunk > 0) ? (const png_byte*)&Data[offset] : nullptr;
        
        png_progressive_combine_row(png_ptr, dummy_row, new_row_data);

        offset += current_chunk;
        
        if (chunk_size < 1024) {
            chunk_size *= 2;
        } else {
            chunk_size = 1;
        }
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}