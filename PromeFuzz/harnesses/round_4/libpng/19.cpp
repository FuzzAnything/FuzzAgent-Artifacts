// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_handle_as_unknown at png.c:903:1 in png.h
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
// png_get_unknown_chunks at pngget.c:1249:1 in png.h
// png_get_io_chunk_type at pngget.c:1340:1 in png.h
// png_get_user_chunk_ptr at pngget.c:1272:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
// png_get_user_chunk_ptr at pngget.c:1272:1 in png.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "png.h"

static int user_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    // Simple callback that always returns 0 (chunk not handled, will be saved)
    (void)png_ptr;
    (void)chunk;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Test png_handle_as_unknown with various chunk names
    if (Size >= 4) {
        png_byte chunk_name[4];
        memcpy(chunk_name, Data, 4);
        png_handle_as_unknown(png_ptr, chunk_name);
    }
    
    // Test png_set_keep_unknown_chunks
    if (Size >= 5) {
        // Create chunk list with 5-byte entries (4-byte name + 1-byte handling code)
        size_t max_chunks = Size / 5;
        if (max_chunks > 0) {
            png_byte* chunk_list = (png_byte*)Data;
            int num_chunks = static_cast<int>(max_chunks);
            int keep = (Size % 3); // 0, 1, or 2
            png_set_keep_unknown_chunks(png_ptr, keep, chunk_list, num_chunks);
        }
    }
    
    // Test png_get_unknown_chunks
    png_unknown_chunkpp entries = nullptr;
    int chunk_count = png_get_unknown_chunks(png_ptr, info_ptr, entries);
    (void)chunk_count; // Use the result to avoid unused variable warning
    
    // Test png_get_io_chunk_type
    png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr);
    (void)chunk_type;
    
    // Test png_get_user_chunk_ptr
    void* user_ptr = png_get_user_chunk_ptr(png_ptr);
    (void)user_ptr;
    
    // Test png_set_read_user_chunk_fn
    static int dummy_data = 42;
    png_set_read_user_chunk_fn(png_ptr, &dummy_data, user_chunk_callback);
    
    // Test png_get_user_chunk_ptr again after setting it
    user_ptr = png_get_user_chunk_ptr(png_ptr);
    (void)user_ptr;
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}