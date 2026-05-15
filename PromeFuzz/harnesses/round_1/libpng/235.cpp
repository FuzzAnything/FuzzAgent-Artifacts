// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_unknown_chunks at pngget.c:1249:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
// png_handle_as_unknown at png.c:903:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "code/png.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>

// User chunk callback function
static int user_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    // Return 0 to save the chunk, positive to discard, negative for error
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_235(const uint8_t *Data, size_t Size) {
    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
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
    
    // Parse fuzz input for parameters
    size_t offset = 0;
    
    // Get user_chunk_cache_max (4 bytes)
    png_uint_32 user_chunk_cache_max = 0;
    if (offset + sizeof(png_uint_32) <= Size) {
        memcpy(&user_chunk_cache_max, Data + offset, sizeof(png_uint_32));
        offset += sizeof(png_uint_32);
    }
    
    // Get keep parameter (1 byte)
    int keep = 0;
    if (offset + 1 <= Size) {
        keep = Data[offset];
        offset += 1;
    }
    
    // Get num_chunks (1 byte, limited to reasonable value)
    int num_chunks = 0;
    if (offset + 1 <= Size) {
        num_chunks = Data[offset] & 0x1F; // Limit to 31 chunks max
        offset += 1;
    }
    
    // Prepare chunk_list for png_set_keep_unknown_chunks
    // Each chunk needs 5 bytes: 4-byte name + 1-byte handling code
    png_byte* chunk_list = nullptr;
    if (num_chunks > 0 && offset + num_chunks * 5 <= Size) {
        chunk_list = new png_byte[num_chunks * 5];
        memcpy(chunk_list, Data + offset, num_chunks * 5);
        offset += num_chunks * 5;
    }
    
    // Get chunk and location for png_set_unknown_chunk_location (2 bytes)
    int chunk = 0;
    int location = 0;
    if (offset + 2 <= Size) {
        chunk = Data[offset];
        location = Data[offset + 1];
        offset += 2;
    }
    
    // Get chunk_name for png_handle_as_unknown (4 bytes)
    png_byte chunk_name[5] = {0};
    if (offset + 4 <= Size) {
        memcpy(chunk_name, Data + offset, 4);
        offset += 4;
    }
    
    // Call target API functions
    png_set_chunk_cache_max(png_ptr, user_chunk_cache_max);
    
    if (chunk_list) {
        png_set_keep_unknown_chunks(png_ptr, keep, chunk_list, num_chunks);
        delete[] chunk_list;
    }
    
    png_set_unknown_chunk_location(png_ptr, info_ptr, chunk, location);
    
    png_set_read_user_chunk_fn(png_ptr, nullptr, user_chunk_callback);
    
    png_handle_as_unknown(png_ptr, chunk_name);
    
    // Test png_get_unknown_chunks - correct parameter types
    png_unknown_chunkp entries = nullptr;
    int count = png_get_unknown_chunks(png_ptr, info_ptr, &entries);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}