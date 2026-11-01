// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_process_data at pngpread.c:50:1 in png.h
// png_process_data_skip at pngpread.c:92:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstddef>

// Callbacks for progressive reading
static void
progressive_info_callback(png_structp png_ptr, png_infop info_ptr) {
    // Stub callback
    (void)png_ptr;
    (void)info_ptr;
}

static void
progressive_row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) {
    // Stub callback
    // We can use png_progressive_combine_row here if we had an old_row buffer
    (void)png_ptr;
    (void)new_row;
    (void)row_num;
    (void)pass;
}

static void
progressive_end_callback(png_structp png_ptr, png_infop info_ptr) {
    // Stub callback
    (void)png_ptr;
    (void)info_ptr;
}

// Fuzz driver entry point
extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling to avoid crashes
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Set progressive read callbacks
    // We use a dummy pointer for progressive_ptr
    void *progressive_ptr = const_cast<uint8_t*>(Data);
    png_set_progressive_read_fn(png_ptr, progressive_ptr,
                                (png_progressive_info_ptr)progressive_info_callback,
                                (png_progressive_row_ptr)progressive_row_callback,
                                (png_progressive_end_ptr)progressive_end_callback);

    // Retrieve the progressive pointer to ensure it was set
    void *retrieved_ptr = png_get_progressive_ptr(png_ptr);
    (void)retrieved_ptr; // Use to avoid unused variable warning

    // Process data incrementally
    // We simulate feeding data in chunks
    size_t offset = 0;
    size_t chunk_size = Size;
    
    // Try different chunk sizes to explore different states
    if (Size > 1024) {
        chunk_size = 1024;
    } else if (Size > 256) {
        chunk_size = 256;
    } else if (Size > 64) {
        chunk_size = 64;
    } else if (Size > 16) {
        chunk_size = 16;
    } else if (Size > 4) {
        chunk_size = 4;
    }

    while (offset < Size) {
        size_t remaining = Size - offset;
        size_t current_chunk = (remaining < chunk_size) ? remaining : chunk_size;
        
        // Call png_process_data
        png_process_data(png_ptr, info_ptr, (png_byte*)(Data + offset), current_chunk);
        
        // Try to pause processing (only valid inside png_process_data, but we call it after)
        // This might not be valid according to docs, but we test it anyway
        // Note: The docs say "only callable inside png_process_data", but we test boundary
        // We'll skip this to avoid undefined behavior as per the warning
        
        // Try the deprecated skip function (should return 0)
        png_uint_32 skip_bytes = png_process_data_skip(png_ptr);
        (void)skip_bytes; // Use to avoid unused variable warning

        offset += current_chunk;
        
        // Vary chunk size for next iteration to explore more states
        if (chunk_size > 1) {
            chunk_size = chunk_size / 2;
        }
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}