// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_io_chunk_type at pngget.c:1340:1 in png.h
// png_get_user_chunk_ptr at pngget.c:1272:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
// png_handle_as_unknown at png.c:903:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
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
#include <cstdio>

#include "code/png.h"

static int user_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    // Simple callback that returns 0 (save chunk) or 1 (handle/discard chunk)
    // based on first byte of chunk data, if available
    if (chunk->size > 0) {
        return chunk->data[0] % 2;
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_370(const uint8_t *Data, size_t Size) {
    if (Size < 20) {
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

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Test png_set_read_user_chunk_fn
    int user_data = 42;
    png_set_read_user_chunk_fn(png_ptr, &user_data, user_chunk_callback);

    // Test png_set_keep_unknown_chunks
    // We need 5-byte entries: 4-byte chunk name + 1-byte handling code
    size_t max_chunks = 10;
    size_t chunk_list_size = max_chunks * 5;
    
    if (Size >= 20 + chunk_list_size) {
        png_byte* chunk_list = (png_byte*)Data + 20;
        int num_chunks = max_chunks;
        png_set_keep_unknown_chunks(png_ptr, PNG_HANDLE_CHUNK_AS_DEFAULT, chunk_list, num_chunks);
    }

    // Test png_handle_as_unknown
    if (Size >= 24) {
        png_byte chunk_name[5];
        memcpy(chunk_name, Data + 20, 4);
        chunk_name[4] = '\0';
        int result = png_handle_as_unknown(png_ptr, chunk_name);
        (void)result; // Use result to avoid unused variable warning
    }

    // Test png_set_unknown_chunks
    if (Size >= 100) {
        png_unknown_chunk unknown_chunk;
        
        // Set chunk name (4 bytes + null terminator)
        size_t name_offset = 30;
        if (Size > name_offset + 4) {
            memcpy(unknown_chunk.name, Data + name_offset, 4);
            unknown_chunk.name[4] = '\0';
        } else {
            memcpy(unknown_chunk.name, "TEST", 4);
            unknown_chunk.name[4] = '\0';
        }
        
        // Set chunk data
        size_t data_size = Size - 50;
        if (data_size > 0 && data_size < 10000) {
            unknown_chunk.data = (png_byte*)malloc(data_size);
            if (unknown_chunk.data) {
                memcpy(unknown_chunk.data, Data + 50, data_size);
                unknown_chunk.size = data_size;
                unknown_chunk.location = PNG_HAVE_IHDR;
                
                png_set_unknown_chunks(png_ptr, info_ptr, &unknown_chunk, 1);
                free(unknown_chunk.data);
            }
        }
    }

    // Test png_get_io_chunk_type
    png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr);
    (void)chunk_type; // Use chunk_type to avoid unused variable warning

    // Test png_get_user_chunk_ptr
    void* retrieved_ptr = png_get_user_chunk_ptr(png_ptr);
    (void)retrieved_ptr; // Use retrieved_ptr to avoid unused variable warning

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}