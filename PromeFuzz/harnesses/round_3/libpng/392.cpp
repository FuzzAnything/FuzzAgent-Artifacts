// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_get_chunk_malloc_max at pngget.c:1325:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_get_chunk_cache_max at pngget.c:1318:1 in png.h
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
#include "code/png.h"
#include <cstdint>
#include <cstring>
#include <cstdlib>

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}
static png_voidp dummy_malloc_fn(png_structp, png_size_t size) { return malloc(size); }
static void dummy_free_fn(png_structp, png_voidp ptr) { free(ptr); }

extern "C" int LLVMFuzzerTestOneInput_392(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create dummy PNG structures
    png_structp png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        dummy_error_fn,
        dummy_warning_fn,
        nullptr,
        dummy_malloc_fn,
        dummy_free_fn
    );
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Test png_set_chunk_malloc_max and png_get_chunk_malloc_max
    if (Size >= sizeof(png_alloc_size_t)) {
        png_alloc_size_t max_alloc;
        memcpy(&max_alloc, Data, sizeof(png_alloc_size_t));
        png_set_chunk_malloc_max(png_ptr, max_alloc);
        png_alloc_size_t retrieved = png_get_chunk_malloc_max(png_ptr);
        (void)retrieved;
    }

    // Test png_set_chunk_cache_max and png_get_chunk_cache_max
    if (Size >= sizeof(png_uint_32)) {
        png_uint_32 cache_max;
        memcpy(&cache_max, Data, sizeof(png_uint_32));
        png_set_chunk_cache_max(png_ptr, cache_max);
        png_uint_32 retrieved_cache = png_get_chunk_cache_max(png_ptr);
        (void)retrieved_cache;
    }

    // Test png_set_unknown_chunks - fixed to avoid freeing arbitrary pointers
    if (Size >= sizeof(png_unknown_chunk)) {
        png_unknown_chunk unknown;
        size_t copy_size = Size < sizeof(unknown) ? Size : sizeof(unknown);
        memcpy(&unknown, Data, copy_size);
        
        // Ensure name has null terminator
        if (copy_size >= 5) unknown.name[4] = '\0';
        
        // Initialize data pointer safely
        unknown.data = nullptr;
        unknown.size = 0;
        
        // Only allocate and set data if we have enough input
        if (Size > sizeof(unknown)) {
            size_t data_size = Size - sizeof(unknown);
            if (data_size > 0) {
                unknown.data = (png_byte*)malloc(data_size);
                if (unknown.data) {
                    unknown.size = data_size;
                    memcpy(unknown.data, Data + sizeof(unknown), data_size);
                }
            }
        }
        
        png_set_unknown_chunks(png_ptr, info_ptr, &unknown, 1);
        
        // Only free if we allocated it
        if (unknown.data) {
            free(unknown.data);
        }
    }

    // Test png_set_keep_unknown_chunks
    if (Size >= 5) {
        int keep = Data[0] % 3;
        int num_chunks = 1;
        png_byte chunk_list[5];
        
        size_t copy_len = Size < 5 ? Size : 5;
        memcpy(chunk_list, Data, copy_len);
        
        if (copy_len == 5) chunk_list[4] = chunk_list[4] % 3;
        else if (copy_len > 0) chunk_list[copy_len-1] = chunk_list[copy_len-1] % 3;
        
        png_set_keep_unknown_chunks(png_ptr, keep, chunk_list, num_chunks);
    }

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}