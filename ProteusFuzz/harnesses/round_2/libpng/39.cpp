// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_set_text_compression_method at pngwrite.c:1332:1 in png.h
// png_set_text_compression_level at pngwrite.c:1273:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
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
#include <fstream>

static void cleanup_png_struct(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for writing
    const char *dummy_file = "./dummy_file";
    
    // Initialize libpng structures
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Create write struct
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Use setjmp to handle libpng errors gracefully
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        cleanup_png_struct(png_ptr, info_ptr);
        return 0;
    }

    // Open dummy file
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        cleanup_png_struct(png_ptr, info_ptr);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Extract fuzz data for parameters with bounds checking
    int mem_level = 1;
    int strategy = 0;
    int text_mem_level = 1;
    int window_bits = 8;
    int method = 8;
    int level = 0;

    if (Size > 0) mem_level = 1 + (Data[0] % 9); // 1-9
    if (Size > 1) strategy = Data[1] % 5; // 0-4 (Z_NO_COMPRESSION to Z_FIXED)
    if (Size > 2) text_mem_level = 1 + (Data[2] % 9); // 1-9
    if (Size > 3) window_bits = 8 + (Data[3] % 8); // 8-15
    
    if (Size > 4) {
        if (Data[4] % 2 == 0) {
            method = 8;
        } else {
            method = 0; // Invalid method to test warning behavior
        }
    }
    
    if (Size > 5) {
        level = Data[5] % 10; // 0-9
    }

    // Call target API functions
    png_set_compression_mem_level(png_ptr, mem_level);
    png_set_text_compression_strategy(png_ptr, strategy);
    png_set_text_compression_mem_level(png_ptr, text_mem_level);
    png_set_compression_window_bits(png_ptr, window_bits);
    png_set_text_compression_method(png_ptr, method);
    png_set_text_compression_level(png_ptr, level);

    // Write minimal PNG header to trigger actual usage
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    try {
        png_write_info(png_ptr, info_ptr);
        
        // Write a single pixel
        png_byte row[3] = {255, 0, 0};
        png_write_row(png_ptr, row);
        
        png_write_end(png_ptr, info_ptr);
    } catch (...) {
        // Ignore exceptions during write
    }

    // Cleanup
    if (fp) {
        fclose(fp);
    }
    cleanup_png_struct(png_ptr, info_ptr);
    
    return 0;
}