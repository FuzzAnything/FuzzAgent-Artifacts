// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_handle_as_unknown at png.c:903:1 in png.h
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_get_unknown_chunks at pngget.c:1249:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
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
    
    // Test png_handle_as_unknown
    if (Size >= 4) {
        png_byte chunk_name[4];
        memcpy(chunk_name, Data, 4);
        png_handle_as_unknown(png_ptr, chunk_name);
    }
    
    // Test png_set_keep_unknown_chunks
    if (Size >= 5) {
        // Ensure we have at least one 5-byte entry (4-byte name + 1-byte handling code)
        int num_chunks = Size / 5;
        if (num_chunks > 0) {
            png_set_keep_unknown_chunks(png_ptr, 0, Data, num_chunks);
        }
    }
    
    // Test png_set_unknown_chunks
    if (Size >= sizeof(png_unknown_chunk)) {
        png_unknown_chunk unknown;
        size_t copy_size = Size < sizeof(unknown) ? Size : sizeof(unknown);
        memcpy(&unknown, Data, copy_size);
        
        // Ensure the name field is properly null-terminated
        if (copy_size >= 5) {
            unknown.name[4] = '\0';
        }
        
        // Allocate data if needed
        if (Size > sizeof(unknown) && unknown.size > 0) {
            size_t data_size = Size - sizeof(unknown);
            if (data_size > unknown.size) {
                data_size = unknown.size;
            }
            unknown.data = (png_byte*)malloc(data_size);
            if (unknown.data) {
                memcpy(unknown.data, Data + sizeof(unknown), data_size);
                unknown.size = data_size;
                
                png_set_unknown_chunks(png_ptr, info_ptr, &unknown, 1);
                
                free(unknown.data);
            }
        } else {
            unknown.data = nullptr;
            unknown.size = 0;
            png_set_unknown_chunks(png_ptr, info_ptr, &unknown, 1);
        }
    }
    
    // Test png_set_unknown_chunk_location
    if (Size >= 8) {
        int chunk = *reinterpret_cast<const int*>(Data);
        int location = *reinterpret_cast<const int*>(Data + 4);
        png_set_unknown_chunk_location(png_ptr, info_ptr, chunk, location);
    }
    
    // Test png_set_invalid
    if (Size >= 4) {
        int mask = *reinterpret_cast<const int*>(Data);
        png_set_invalid(png_ptr, info_ptr, mask);
    }
    
    // Test png_get_unknown_chunks
    png_unknown_chunkpp entries = nullptr;
    png_get_unknown_chunks(png_ptr, info_ptr, entries);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}