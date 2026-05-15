// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_get_chunk_cache_max at pngget.c:1318:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
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
#include "png.h"
#include <cstdint>
#include <cstring>
#include <cstdlib>

// User chunk callback function
static int user_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    // Simple callback that always returns 0 (chunk not handled, will be saved)
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Create PNG structures
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
    
    // Test png_set_chunk_cache_max
    if (Size >= sizeof(png_uint_32)) {
        png_uint_32 cache_max;
        memcpy(&cache_max, Data, sizeof(png_uint_32));
        png_set_chunk_cache_max(png_ptr, cache_max);
    }
    
    // Test png_get_chunk_cache_max
    png_uint_32 current_cache_max = png_get_chunk_cache_max(png_ptr);
    
    // Test png_set_chunk_malloc_max
    if (Size >= sizeof(png_alloc_size_t) + sizeof(png_uint_32)) {
        png_alloc_size_t malloc_max;
        memcpy(&malloc_max, Data + sizeof(png_uint_32), sizeof(png_alloc_size_t));
        png_set_chunk_malloc_max(png_ptr, malloc_max);
    }
    
    // Test png_set_keep_unknown_chunks
    // Need at least: keep (1 byte) + num_chunks (4 bytes) + chunk_list entries (5 bytes each)
    if (Size >= 5) {
        int keep = Data[0] % 3;  // 0, 1, or 2
        int num_chunks = (Data[1] % 10) + 1;  // 1-10 chunks
        
        // Calculate required size for chunk_list
        size_t chunk_list_size = num_chunks * 5;
        if (Size >= 2 + chunk_list_size) {
            png_byte* chunk_list = new png_byte[chunk_list_size];
            
            // Copy chunk names and handling codes from fuzzer input
            // Each entry: 4 bytes chunk name + 1 byte handling code (0-2)
            for (int i = 0; i < num_chunks; i++) {
                size_t offset = 2 + (i * 5);
                if (offset + 4 < Size) {
                    memcpy(&chunk_list[i*5], Data + offset, 4);
                    chunk_list[i*5 + 4] = Data[offset + 4] % 3;  // Handling code 0-2
                } else {
                    // Fill with default values if not enough data
                    memset(&chunk_list[i*5], 'A' + (i % 26), 4);
                    chunk_list[i*5 + 4] = i % 3;
                }
            }
            
            png_set_keep_unknown_chunks(png_ptr, keep, chunk_list, num_chunks);
            delete[] chunk_list;
        }
    }
    
    // Test png_set_unknown_chunks
    if (Size >= 10) {
        int num_unknowns = (Data[2] % 5) + 1;  // 1-5 unknown chunks
        
        png_unknown_chunk* unknowns = new png_unknown_chunk[num_unknowns];
        
        for (int i = 0; i < num_unknowns; i++) {
            // Set chunk name (5 bytes including null terminator)
            for (int j = 0; j < 4; j++) {
                size_t offset = 3 + (i * 10) + j;
                unknowns[i].name[j] = (offset < Size) ? Data[offset] : 'A' + j;
            }
            unknowns[i].name[4] = '\0';
            
            // Set chunk data
            size_t data_offset = 3 + (i * 10) + 4;
            size_t data_size = (Data[data_offset % Size] % 100) + 1;
            
            unknowns[i].data = new png_byte[data_size];
            unknowns[i].size = data_size;
            
            for (size_t j = 0; j < data_size; j++) {
                size_t src_offset = (data_offset + 1 + j) % Size;
                unknowns[i].data[j] = Data[src_offset];
            }
            
            // Set location
            unknowns[i].location = (i % 3) + 1;  // Valid location values
        }
        
        png_set_unknown_chunks(png_ptr, info_ptr, unknowns, num_unknowns);
        
        // Clean up
        for (int i = 0; i < num_unknowns; i++) {
            delete[] unknowns[i].data;
        }
        delete[] unknowns;
    }
    
    // Test png_set_read_user_chunk_fn
    void* user_chunk_ptr = nullptr;
    png_set_read_user_chunk_fn(png_ptr, user_chunk_ptr, user_chunk_callback);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}