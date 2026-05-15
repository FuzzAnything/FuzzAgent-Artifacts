// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_get_unknown_chunks at pngget.c:1249:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void dummy_read_fn(png_structp png_ptr, png_bytep out, png_size_t length) {
    // Dummy read function - doesn't actually read anything
    (void)png_ptr;
    (void)out;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    // Initialize libpng structures
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
    
    // Set up dummy read function
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);
    
    // Test png_set_chunk_cache_max
    if (Size >= sizeof(png_uint_32)) {
        png_uint_32 cache_max;
        memcpy(&cache_max, Data, sizeof(png_uint_32));
        png_set_chunk_cache_max(png_ptr, cache_max);
        Data += sizeof(png_uint_32);
        Size -= sizeof(png_uint_32);
    }
    
    // Test png_set_keep_unknown_chunks
    if (Size >= 1) {
        int keep = Data[0] % 3; // 0, 1, or 2
        Data++;
        Size--;
        
        // Prepare chunk list (5-byte entries: 4-byte name + 1-byte handling code)
        int num_chunks = 0;
        png_byte chunk_list[100]; // Max 20 chunks * 5 bytes
        size_t list_size = 0;
        
        while (Size >= 5 && list_size < 95) { // Leave room for at least one more 5-byte entry
            // Copy 4-byte chunk name
            memcpy(chunk_list + list_size, Data, 4);
            Data += 4;
            Size -= 4;
            
            // Add handling code (0-2)
            chunk_list[list_size + 4] = Data[0] % 3;
            Data++;
            Size--;
            
            list_size += 5;
            num_chunks++;
            
            if (num_chunks >= 20) break; // Limit to 20 chunks
        }
        
        if (num_chunks > 0) {
            png_set_keep_unknown_chunks(png_ptr, keep, chunk_list, num_chunks);
        }
    }
    
    // Test png_handle_as_unknown
    if (Size >= 4) {
        png_byte chunk_name[4];
        memcpy(chunk_name, Data, 4);
        png_handle_as_unknown(png_ptr, chunk_name);
        Data += 4;
        Size -= 4;
    }
    
    // Prepare unknown chunks for testing
    int num_unknowns = 0;
    png_unknown_chunk unknowns[10];
    
    // Initialize unknown chunks with fuzz data
    while (Size >= 9 && num_unknowns < 10) { // Need at least: 5-byte name + 4-byte size
        // Copy chunk name (5 bytes)
        memcpy(unknowns[num_unknowns].name, Data, 5);
        Data += 5;
        Size -= 5;
        
        // Get size (limit to remaining data)
        size_t chunk_size;
        if (Size >= sizeof(size_t)) {
            memcpy(&chunk_size, Data, sizeof(size_t));
            Data += sizeof(size_t);
            Size -= sizeof(size_t);
        } else {
            chunk_size = Size;
        }
        
        // Limit chunk size to prevent excessive memory allocation
        if (chunk_size > 1024) chunk_size = 1024;
        
        // Allocate and copy chunk data
        if (chunk_size > 0 && Size >= chunk_size) {
            unknowns[num_unknowns].data = (png_bytep)malloc(chunk_size);
            if (unknowns[num_unknowns].data) {
                memcpy(unknowns[num_unknowns].data, Data, chunk_size);
                unknowns[num_unknowns].size = chunk_size;
                Data += chunk_size;
                Size -= chunk_size;
            } else {
                unknowns[num_unknowns].size = 0;
            }
        } else {
            unknowns[num_unknowns].size = 0;
        }
        
        // Set location (0-2)
        if (Size >= 1) {
            unknowns[num_unknowns].location = Data[0] % 3;
            Data++;
            Size--;
        } else {
            unknowns[num_unknowns].location = 0;
        }
        
        num_unknowns++;
    }
    
    // Test png_set_unknown_chunk_location
    if (num_unknowns > 0 && Size >= 2) {
        int chunk_idx = Data[0] % num_unknowns;
        int location = Data[1] % 3;
        png_set_unknown_chunk_location(png_ptr, info_ptr, chunk_idx, location);
        Data += 2;
        Size -= 2;
    }
    
    // Test png_set_unknown_chunks
    if (num_unknowns > 0) {
        png_set_unknown_chunks(png_ptr, info_ptr, unknowns, num_unknowns);
    }
    
    // Test png_get_unknown_chunks
    png_unknown_chunkp entries;
    int entry_count = png_get_unknown_chunks(png_ptr, info_ptr, &entries);
    (void)entry_count; // Suppress unused warning
    
    // Clean up allocated memory
    for (int i = 0; i < num_unknowns; i++) {
        if (unknowns[i].data) {
            free(unknowns[i].data);
        }
    }
    
    // Clean up libpng structures
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}