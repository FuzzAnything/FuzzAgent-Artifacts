// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_set_text_compression_method at pngwrite.c:1332:1 in png.h
// png_set_text_compression_level at pngwrite.c:1273:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_set_text_compression_method at pngwrite.c:1332:1 in png.h
// png_set_text_compression_level at pngwrite.c:1273:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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
#include <cstdio>

static void write_dummy_png_file(const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal valid PNG header
    unsigned char png_header[8] = {
        0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A
    };
    fwrite(png_header, 1, 8, f);
    
    // Write a minimal IHDR chunk
    unsigned char ihdr[13] = {
        0x00, 0x00, 0x00, 0x01, // width: 1
        0x00, 0x00, 0x00, 0x01, // height: 1
        0x08, // bit depth
        0x02, // color type (grayscale)
        0x00, // compression
        0x00, // filter
        0x00  // interlace
    };
    
    // Length (4 bytes)
    uint32_t len = 13;
    unsigned char len_bytes[4];
    len_bytes[0] = (len >> 24) & 0xFF;
    len_bytes[1] = (len >> 16) & 0xFF;
    len_bytes[2] = (len >> 8) & 0xFF;
    len_bytes[3] = len & 0xFF;
    fwrite(len_bytes, 1, 4, f);
    
    // Chunk type "IHDR"
    fwrite("IHDR", 1, 4, f);
    
    // IHDR data
    fwrite(ihdr, 1, 13, f);
    
    // CRC (dummy)
    uint32_t crc = 0;
    fwrite(&crc, 1, 4, f);
    
    // Write a minimal IDAT chunk
    unsigned char idat_data[10] = {
        0x78, 0x9C, 0x62, 0x60, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01
    };
    
    len = 10;
    len_bytes[0] = (len >> 24) & 0xFF;
    len_bytes[1] = (len >> 16) & 0xFF;
    len_bytes[2] = (len >> 8) & 0xFF;
    len_bytes[3] = len & 0xFF;
    fwrite(len_bytes, 1, 4, f);
    
    // Chunk type "IDAT"
    fwrite("IDAT", 1, 4, f);
    
    // IDAT data
    fwrite(idat_data, 1, 10, f);
    
    // CRC (dummy)
    fwrite(&crc, 1, 4, f);
    
    // Write IEND chunk
    len = 0;
    len_bytes[0] = 0; len_bytes[1] = 0; len_bytes[2] = 0; len_bytes[3] = 0;
    fwrite(len_bytes, 1, 4, f);
    fwrite("IEND", 1, 4, f);
    fwrite(&crc, 1, 4, f);
    
    fclose(f);
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Create a dummy PNG file for testing
    write_dummy_png_file("./dummy_file");
    
    // Extract parameters from fuzz input
    int strategy = Data[0];
    int window_bits = Data[1];
    int mem_level = (Size > 2) ? Data[2] : 0;
    int method = (Size > 3) ? Data[3] : 0;
    int level = (Size > 4) ? Data[4] : 0;
    
    // Create PNG write structure
    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        NULL,
        NULL
    );
    
    if (!png_ptr) {
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Test png_set_text_compression_strategy
    png_set_text_compression_strategy(png_ptr, strategy);
    
    // Test png_set_text_compression_window_bits
    png_set_text_compression_window_bits(png_ptr, window_bits);
    
    // Test png_set_text_compression_mem_level
    png_set_text_compression_mem_level(png_ptr, mem_level);
    
    // Test png_set_compression_window_bits
    png_set_compression_window_bits(png_ptr, window_bits);
    
    // Test png_set_text_compression_method
    png_set_text_compression_method(png_ptr, method);
    
    // Test png_set_text_compression_level
    png_set_text_compression_level(png_ptr, level);
    
    // Test with NULL pointer (should be safe)
    png_set_text_compression_strategy(NULL, strategy);
    png_set_text_compression_window_bits(NULL, window_bits);
    png_set_text_compression_mem_level(NULL, mem_level);
    png_set_compression_window_bits(NULL, window_bits);
    png_set_text_compression_method(NULL, method);
    png_set_text_compression_level(NULL, level);
    
    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}