// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
// png_handle_as_unknown at png.c:903:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_set_unknown_chunks at pngset.c:1493:1 in png.h
// png_get_unknown_chunks at pngget.c:1249:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <vector>

#include "code/png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {}
static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return nullptr;
    return png_ptr;
}

static png_infop create_png_info(png_structp png_ptr) {
    if (!png_ptr) return nullptr;
    png_infop info_ptr = png_create_info_struct(png_ptr);
    return info_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;

    // Create PNG structures
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) return 0;
    
    png_infop info_ptr = create_png_info(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Use first 4 bytes for chunk_name
    png_byte chunk_name[5];
    memcpy(chunk_name, Data, 4);
    chunk_name[4] = '\0';
    
    // Use next 4 bytes for user_chunk_cache_max
    png_uint_32 user_chunk_cache_max;
    memcpy(&user_chunk_cache_max, Data + 4, 4);
    
    // Use next byte for keep parameter
    int keep = Data[8] % 3;
    
    // Use next byte for location - must be valid PNG location values
    // PNG_HAVE_IHDR (0x01), PNG_HAVE_PLTE (0x02), PNG_AFTER_IDAT (0x04)
    // Only one bit should be set at a time
    int location = 0;
    switch (Data[9] % 3) {
        case 0: location = PNG_HAVE_IHDR; break;
        case 1: location = PNG_HAVE_PLTE; break;
        case 2: location = PNG_AFTER_IDAT; break;
    }
    
    // Use next byte for num_chunks
    int num_chunks = Data[10] % 5;
    if (num_chunks == 0) num_chunks = 1;
    
    // Prepare chunk_list for png_set_keep_unknown_chunks
    // Each chunk needs 5 bytes: 4 for name + 1 for handling code
    size_t chunk_list_size = num_chunks * 5;
    if (chunk_list_size > Size - 11) {
        chunk_list_size = Size - 11;
        num_chunks = chunk_list_size / 5;
        if (num_chunks == 0) {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            return 0;
        }
        chunk_list_size = num_chunks * 5;
    }
    
    std::vector<png_byte> chunk_list(chunk_list_size);
    if (chunk_list_size > 0) {
        memcpy(chunk_list.data(), Data + 11, chunk_list_size);
    }
    
    // Prepare unknown chunks for png_set_unknown_chunks
    size_t unknowns_offset = 11 + chunk_list_size;
    size_t remaining = Size - unknowns_offset;
    
    int num_unknowns = 1;
    if (remaining > sizeof(png_unknown_chunk)) {
        num_unknowns = (remaining / sizeof(png_unknown_chunk)) % 5;
        if (num_unknowns == 0) num_unknowns = 1;
    }
    
    std::vector<png_unknown_chunk> unknowns(num_unknowns);
    for (int i = 0; i < num_unknowns; i++) {
        // Initialize with safe defaults
        memset(unknowns[i].name, 0, 5);
        unknowns[i].data = nullptr;
        unknowns[i].size = 0;
        
        // Copy name from input if available
        size_t name_offset = unknowns_offset + i * 4;
        if (name_offset + 4 <= Size) {
            memcpy(unknowns[i].name, Data + name_offset, 4);
        }
        
        // Set location to a valid single-bit value
        // Use the same logic as above to ensure only one bit is set
        int chunk_location = 0;
        switch ((Data[9] + i) % 3) {
            case 0: chunk_location = PNG_HAVE_IHDR; break;
            case 1: chunk_location = PNG_HAVE_PLTE; break;
            case 2: chunk_location = PNG_AFTER_IDAT; break;
        }
        unknowns[i].location = static_cast<png_byte>(chunk_location);
    }
    
    // Call target functions
    png_set_chunk_cache_max(png_ptr, user_chunk_cache_max);
    
    if (!chunk_list.empty()) {
        png_set_keep_unknown_chunks(png_ptr, keep, chunk_list.data(), num_chunks);
    }
    
    png_handle_as_unknown(png_ptr, chunk_name);
    
    if (!unknowns.empty()) {
        // Set location for first chunk before setting unknown chunks
        png_set_unknown_chunk_location(png_ptr, info_ptr, 0, location);
        
        png_set_unknown_chunks(png_ptr, info_ptr, unknowns.data(), num_unknowns);
    }
    
    png_unknown_chunkpp entries = nullptr;
    png_get_unknown_chunks(png_ptr, info_ptr, entries);
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}