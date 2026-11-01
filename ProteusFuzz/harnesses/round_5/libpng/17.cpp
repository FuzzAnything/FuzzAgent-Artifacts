// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_process_data_pause at pngpread.c:65:1 in png.h
// png_process_data_pause at pngpread.c:65:1 in png.h
// png_process_data_skip at pngpread.c:92:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
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
    // We cannot safely get the info_ptr or rowbytes here without proper initialization
    // and without knowing if the image header has been parsed. 
    // To avoid crashes, we simply ignore the row data in this fuzz driver context
    // or perform a dummy combine if we had a valid buffer.
    // The target function png_progressive_combine_row is called below in the main loop
    // with controlled data to ensure safety.
    (void)png_ptr;
    (void)new_row;
    (void)row_num;
    (void)pass;
}

static void progressive_end_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr != NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    } else if (info_ptr != NULL) {
        png_destroy_read_struct(NULL, &info_ptr, NULL);
    }
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    int ret = 0;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        cleanup_png(png_ptr, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Set up progressive read callbacks
    png_set_progressive_read_fn(png_ptr, NULL, progressive_info_callback,
                                progressive_row_callback, progressive_end_callback);

    size_t offset = 0;
    size_t chunk_size = 1024;
    
    // Fuzz png_process_data
    while (offset < Size) {
        size_t current_chunk = (Size - offset < chunk_size) ? (Size - offset) : chunk_size;
        png_process_data(png_ptr, info_ptr, (png_byte*)(Data + offset), current_chunk);
        
        // Fuzz png_process_data_pause with save=true
        ret = png_process_data_pause(png_ptr, 1);
        (void)ret; // Expected to return 0 when save=1
        
        // Fuzz png_process_data_pause with save=false
        ret = png_process_data_pause(png_ptr, 0);
        (void)ret; // Ignore return value
        
        // Fuzz png_process_data_skip (deprecated, should return 0)
        png_uint_32 skip = png_process_data_skip(png_ptr);
        (void)skip;
        
        offset += current_chunk;
        // Randomly vary chunk size for more state exploration
        if (offset < Size && current_chunk > 1) {
            chunk_size = (chunk_size > 1) ? chunk_size / 2 : 1;
        }
    }

    // Fuzz png_get_progressive_ptr
    void *user_ptr = png_get_progressive_ptr(png_ptr);
    (void)user_ptr; // Should be NULL as we passed NULL in set_progressive_read_fn

    // Fuzz png_progressive_combine_row
    // We create a dummy buffer. Note: In a real scenario, old_row must be allocated 
    // based on the image width and bit depth. Here we use a small fixed buffer 
    // to trigger the function logic safely without crashing if the image isn't fully parsed.
    png_byte dummy_row[256] = {0};
    
    // Call with NULL new_row (should return immediately)
    png_progressive_combine_row(png_ptr, dummy_row, NULL);
    
    // Call with a non-NULL new_row (dummy data)
    png_byte dummy_new_row[256] = {0};
    png_progressive_combine_row(png_ptr, dummy_row, dummy_new_row);

    cleanup_png(png_ptr, info_ptr);
    return 0;
}