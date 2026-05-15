// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_reset_zstream at png.c:951:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function - does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Dummy flush function - does nothing
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    // Create write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Set up dummy I/O functions since we're not actually writing to a file
    png_set_write_fn(png_ptr, nullptr, dummy_write_fn, dummy_flush_fn);
    
    // Extract parameters from fuzzing input
    int mem_level = 1;
    int text_strategy = 0;
    int strategy = 0;
    int window_bits = 15;
    int level = 6;
    
    if (Size >= 5) {
        // Use first 5 bytes to generate parameter values
        mem_level = (Data[0] % 9) + 1;  // 1-9
        text_strategy = Data[1] % 4;    // 0-3 (common strategies)
        strategy = Data[2] % 4;         // 0-3 (common strategies)
        window_bits = (Data[3] % 8) + 8; // 8-15
        level = Data[4] % 10;           // 0-9
    }
    
    // Call target API functions
    png_set_compression_mem_level(png_ptr, mem_level);
    png_set_text_compression_strategy(png_ptr, text_strategy);
    png_set_compression_strategy(png_ptr, strategy);
    png_set_compression_window_bits(png_ptr, window_bits);
    png_set_compression_level(png_ptr, level);
    
    // Try png_reset_zstream with read structure (deprecated but still testable)
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (read_ptr) {
        png_infop read_info = png_create_info_struct(read_ptr);
        if (read_info) {
            if (!setjmp(png_jmpbuf(read_ptr))) {
                // Create a dummy file with PNG header for reading
                FILE* dummy_file = fopen("./dummy_file", "wb");
                if (dummy_file) {
                    // Write minimal PNG header (8 bytes)
                    const uint8_t png_header[] = {137, 80, 78, 71, 13, 10, 26, 10};
                    fwrite(png_header, 1, sizeof(png_header), dummy_file);
                    fclose(dummy_file);
                    
                    // Open for reading
                    dummy_file = fopen("./dummy_file", "rb");
                    if (dummy_file) {
                        png_init_io(read_ptr, dummy_file);
                        // Read the header
                        png_read_info(read_ptr, read_info);
                        // Try to reset zstream (deprecated)
                        int reset_result = png_reset_zstream(read_ptr);
                        (void)reset_result; // Ignore return value
                        fclose(dummy_file);
                    }
                }
            }
            png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        } else {
            png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        }
    }
    
    // Clean up write structures
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    // Remove dummy file if it exists
    remove("./dummy_file");
    
    return 0;
}