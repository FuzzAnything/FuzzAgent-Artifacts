// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_get_chunk_cache_max at pngget.c:1318:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
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

#include "code/png.h"

// User chunk callback function
static int user_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    // Simple callback that returns a deterministic value based on chunk name
    if (chunk->name[0] % 2 == 0) {
        return 1;  // Chunk handled, libpng will ignore/discard it
    } else {
        return 0;  // Chunk not handled, will be saved
    }
}

extern "C" int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    // Create PNG structures
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
    
    // Test png_set_read_user_chunk_fn
    int user_data = 42;
    png_set_read_user_chunk_fn(png_ptr, &user_data, user_chunk_callback);
    
    // Test png_set_unknown_chunks if we have enough data
    if (Size >= sizeof(png_unknown_chunk)) {
        png_unknown_chunk unknown;
        
        // Fill chunk name (4 bytes + null terminator)
        size_t name_len = (Size < 4) ? Size : 4;
        for (size_t i = 0; i < name_len; i++) {
            unknown.name[i] = Data[i];
        }
        for (size_t i = name_len; i < 5; i++) {
            unknown.name[i] = 0;
        }
        
        // Set data pointer and size
        size_t data_offset = (name_len < Size) ? name_len : 0;
        size_t data_size = Size - data_offset;
        if (data_size > 0) {
            unknown.data = (png_byte*)(Data + data_offset);
            unknown.size = data_size;
        } else {
            unknown.data = nullptr;
            unknown.size = 0;
        }
        
        unknown.location = 0;
        
        png_set_unknown_chunks(png_ptr, info_ptr, &unknown, 1);
    }
    
    // Test png_set_keep_unknown_chunks
    if (Size >= 5) {
        // Create a chunk list with 5-byte entries (4-byte name + 1-byte handling code)
        png_byte chunk_list[5];
        for (int i = 0; i < 4 && i < (int)Size; i++) {
            chunk_list[i] = Data[i % Size];
        }
        // Set handling code (0-2)
        chunk_list[4] = (Size > 0) ? (Data[0] % 3) : 0;
        
        png_set_keep_unknown_chunks(png_ptr, PNG_HANDLE_CHUNK_AS_DEFAULT, chunk_list, 1);
    }
    
    // Test png_set_chunk_malloc_max
    png_alloc_size_t malloc_max = 0;
    if (Size >= sizeof(png_alloc_size_t)) {
        // Use first sizeof(png_alloc_size_t) bytes as the max value
        memcpy(&malloc_max, Data, sizeof(png_alloc_size_t));
    } else if (Size > 0) {
        // Use available bytes
        malloc_max = 0;
        for (size_t i = 0; i < Size; i++) {
            malloc_max = (malloc_max << 8) | Data[i];
        }
    }
    png_set_chunk_malloc_max(png_ptr, malloc_max);
    
    // Test png_set_chunk_cache_max
    png_uint_32 cache_max = 0;
    if (Size >= sizeof(png_uint_32)) {
        // Use first sizeof(png_uint_32) bytes as the cache max value
        memcpy(&cache_max, Data, sizeof(png_uint_32));
    } else if (Size > 0) {
        // Use available bytes
        cache_max = 0;
        for (size_t i = 0; i < Size && i < sizeof(png_uint_32); i++) {
            cache_max = (cache_max << 8) | Data[i];
        }
    }
    png_set_chunk_cache_max(png_ptr, cache_max);
    
    // Test png_get_chunk_cache_max
    png_uint_32 retrieved_cache_max = png_get_chunk_cache_max(png_ptr);
    (void)retrieved_cache_max;  // Prevent unused variable warning
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}