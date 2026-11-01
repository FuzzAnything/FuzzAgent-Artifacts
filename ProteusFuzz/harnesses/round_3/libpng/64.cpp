// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
// png_set_keep_unknown_chunks at pngset.c:1665:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_handle_as_unknown at png.c:903:1 in png.h
// png_handle_as_unknown at png.c:903:1 in png.h
// png_get_unknown_chunks at pngget.c:1249:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_set_unknown_chunk_location at pngset.c:1592:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstring>
#include <cstddef>

// Static helper to create a valid PNG file for testing
static void create_dummy_png_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f == nullptr) return;
    
    // Write minimal PNG signature
    const uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    fwrite(png_sig, 1, 8, f);
    
    // Write IHDR chunk (critical, must be first)
    // Length: 13 bytes
    // Type: IHDR
    // Data: width(4), height(4), bit_depth(1), color_type(1), compression(1), filter(1), interlace(1)
    uint8_t ihdr_len[4] = {0, 0, 0, 13};
    uint8_t ihdr_type[4] = {'I', 'H', 'D', 'R'};
    uint8_t ihdr_data[13] = {0, 0, 0, 1, 0, 0, 0, 1, 8, 2, 0, 0, 0}; // 1x1 RGB
    uint32_t ihdr_crc = 0; // CRC is not strictly checked in all paths, but we'll compute a dummy one
    
    fwrite(ihdr_len, 1, 4, f);
    fwrite(ihdr_type, 1, 4, f);
    fwrite(ihdr_data, 1, 13, f);
    fwrite(&ihdr_crc, 1, 4, f);
    
    // Write IDAT chunk (image data)
    uint8_t idat_len[4] = {0, 0, 0, 12};
    uint8_t idat_type[4] = {'I', 'D', 'A', 'T'};
    // Minimal compressed data (just a few bytes to avoid decompression errors)
    uint8_t idat_data[12] = {0x78, 0x9c, 0x62, 0x60, 0x60, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00};
    uint32_t idat_crc = 0;
    
    fwrite(idat_len, 1, 4, f);
    fwrite(idat_type, 1, 4, f);
    fwrite(idat_data, 1, 12, f);
    fwrite(&idat_crc, 1, 4, f);
    
    // Write IEND chunk
    uint8_t iend_len[4] = {0, 0, 0, 0};
    uint8_t iend_type[4] = {'I', 'E', 'N', 'D'};
    uint32_t iend_crc = 0;
    
    fwrite(iend_len, 1, 4, f);
    fwrite(iend_type, 1, 4, f);
    fwrite(&iend_crc, 1, 4, f);
    
    fclose(f);
}

// User chunk callback function
static int user_chunk_callback(png_structp png_ptr, png_unknown_chunkp chunk) {
    // Return 0 to save the chunk, or 1 to discard it
    // We'll randomly decide based on chunk size
    if (chunk->size > 100) {
        return 1; // Discard large chunks
    }
    return 0; // Save small chunks
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0; // Too small to be useful
    
    // Create a dummy PNG file
    create_dummy_png_file("./dummy_file", Data, Size);
    
    // Initialize libpng structures
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    FILE *fp = nullptr;
    
    // Create PNG read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) return 0;
    
    // Create info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Open the dummy file
    fp = fopen("./dummy_file", "rb");
    if (fp == nullptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, fp);
    
    // Test png_set_keep_unknown_chunks
    png_byte chunk_list[] = {'t', 'E', 'X', 't', '\0'};
    png_set_keep_unknown_chunks(png_ptr, PNG_HANDLE_CHUNK_NEVER, chunk_list, 1);
    png_set_keep_unknown_chunks(png_ptr, PNG_HANDLE_CHUNK_AS_DEFAULT, nullptr, 0);
    
    // Test png_set_read_user_chunk_fn
    png_set_read_user_chunk_fn(png_ptr, nullptr, user_chunk_callback);
    
    // Test png_read_info
    png_read_info(png_ptr, info_ptr);
    
    // Test png_handle_as_unknown with various chunk names
    const uint8_t chunk_names[][4] = {
        {'I', 'H', 'D', 'R'},
        {'I', 'D', 'A', 'T'},
        {'I', 'E', 'N', 'D'},
        {'t', 'E', 'X', 't'},
        {'g', 'A', 'M', 'A'},
        {'c', 'H', 'R', 'M'},
        {'b', 'K', 'G', 'D'},
        {'p', 'H', 'Y', 's'},
        {'s', 'B', 'G', 'G'},
        {'s', 'C', 'R', 'N'},
        {'i', 'T', 'X', 't'},
        {'z', 'T', 'X', 't'},
        {'i', 'E', 'N', 'D'}
    };
    
    for (int i = 0; i < 12; ++i) {
        png_handle_as_unknown(png_ptr, chunk_names[i]);
    }
    
    // Test with invalid chunk name (only 3 bytes)
    uint8_t invalid_chunk[3] = {'I', 'H', 'D'};
    png_handle_as_unknown(png_ptr, invalid_chunk);
    
    // Test png_get_unknown_chunks
    png_unknown_chunkp unknowns = nullptr;
    int num_unknowns = png_get_unknown_chunks(png_ptr, info_ptr, &unknowns);
    
    // Test png_set_unknown_chunk_location if we have unknown chunks
    if (num_unknowns > 0) {
        for (int i = 0; i < num_unknowns; ++i) {
            png_set_unknown_chunk_location(png_ptr, info_ptr, i, PNG_HAVE_IHDR);
            png_set_unknown_chunk_location(png_ptr, info_ptr, i, PNG_HAVE_PLTE);
            png_set_unknown_chunk_location(png_ptr, info_ptr, i, PNG_AFTER_IDAT);
        }
    }
    
    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}