// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_write_to_file at pngwrite.c:2402:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_write_chunk at pngwutil.c:209:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
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

static void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void user_flush_data(png_structp png_ptr) {
    // Dummy flush function that does nothing
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_389(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    // Initialize png_struct for writing
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    // Initialize png_info
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

    // Test png_set_write_fn
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        png_set_write_fn(png_ptr, dummy_file, user_write_data, user_flush_data);
        fclose(dummy_file);
    }

    // Test png_write_chunk
    // Use first 4 bytes as chunk name, rest as chunk data
    if (Size >= 4) {
        png_byte chunk_name[4];
        memcpy(chunk_name, Data, 4);
        size_t chunk_data_len = Size - 4;
        const png_byte* chunk_data = Data + 4;
        
        png_write_chunk(png_ptr, chunk_name, chunk_data, chunk_data_len);
    }

    // Test png_set_option
    // Use first byte for option, second byte for onoff
    if (Size >= 2) {
        int option = Data[0] % 10; // Limit to reasonable range
        int onoff = Data[1] & 1;   // 0 or 1
        png_set_option(png_ptr, option, onoff);
    }

    // Test png_set_compression_window_bits
    // Use third byte for window_bits (clamped to 8-15)
    if (Size >= 3) {
        int window_bits = Data[2] % 16;
        if (window_bits < 8) window_bits = 8;
        if (window_bits > 15) window_bits = 15;
        png_set_compression_window_bits(png_ptr, window_bits);
    }

    // Test png_image_write_to_file
    // Create a minimal png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = 1;
    image.height = 1;
    image.format = PNG_FORMAT_GRAY;
    
    // Prepare buffer for 1x1 grayscale image
    png_byte buffer[1] = {0};
    if (Size >= 4) {
        buffer[0] = Data[3] & 0xFF;
    }
    
    // Write to dummy file
    png_image_write_to_file(&image, "./dummy_file", 0, buffer, 0, nullptr);

    // Test png_write_row
    // Prepare a simple row
    png_byte row[1] = {0};
    if (Size >= 5) {
        row[0] = Data[4] & 0xFF;
    }
    
    // Set up basic image info for writing rows
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_GRAY, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, 
                 PNG_FILTER_TYPE_DEFAULT);
    
    // Write info and row
    png_write_info(png_ptr, info_ptr);
    png_write_row(png_ptr, row);
    png_write_end(png_ptr, nullptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}