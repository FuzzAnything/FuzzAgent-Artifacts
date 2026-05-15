// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_write_to_stdio at pngwrite.c:2353:1 in png.h
// png_image_begin_read_from_memory at pngread.c:1523:5 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_file at pngread.c:1455:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_stdio at pngread.c:1425:1 in png.h
// png_image_free at png.c:3989:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* file = fopen("./dummy_file", "wb");
    if (!file) return;
    fwrite(data, 1, size, file);
    fclose(file);
}

static void cleanup_dummy_file() {
    remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_311(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Write input to dummy file for file-based operations
    write_dummy_file(Data, Size);
    
    // Initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    // Test png_image_begin_read_from_memory
    if (png_image_begin_read_from_memory(&image, Data, Size) == 0) {
        // If successful, test png_image_finish_read
        if (image.width > 0 && image.height > 0) {
            // Allocate buffer for image data
            size_t buffer_size = PNG_IMAGE_SIZE(image);
            void* buffer = malloc(buffer_size);
            if (buffer) {
                png_color background;
                background.red = 0;
                background.green = 0;
                background.blue = 0;
                
                if (Size >= 3) {
                    background.red = static_cast<png_byte>(Data[0]);
                    background.green = static_cast<png_byte>(Data[1]);
                    background.blue = static_cast<png_byte>(Data[2]);
                } else if (Size >= 1) {
                    background.red = static_cast<png_byte>(Data[0]);
                }
                
                png_image_finish_read(&image, &background, buffer, 0, NULL);
                free(buffer);
            }
        }
        png_image_free(&image);
    }
    
    // Reinitialize for file-based tests
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    // Test png_image_begin_read_from_file
    if (png_image_begin_read_from_file(&image, "./dummy_file") == 0) {
        png_image_free(&image);
    }
    
    // Reinitialize for stdio tests
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    // Test png_image_begin_read_from_stdio
    FILE* file = fopen("./dummy_file", "rb");
    if (file) {
        if (png_image_begin_read_from_stdio(&image, file) == 0) {
            png_image_free(&image);
        }
        fclose(file);
    }
    
    // Test png_image_write_to_stdio
    // First create a minimal valid image structure
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = 1;
    image.height = 1;
    image.format = PNG_FORMAT_RGBA;
    
    FILE* outfile = fopen("./dummy_file", "wb");
    if (outfile) {
        uint8_t pixel[4] = {0, 0, 0, 255};  // Default to black with full alpha
        
        // Safely copy up to 4 bytes from input data
        size_t bytes_to_copy = Size < 4 ? Size : 4;
        for (size_t i = 0; i < bytes_to_copy; i++) {
            pixel[i] = Data[i];
        }
        
        int convert_to_8_bit = 0;
        if (Size >= 5) {
            convert_to_8_bit = Data[4] % 2;
        }
        
        png_image_write_to_stdio(&image, outfile, convert_to_8_bit, pixel, 0, NULL);
        fclose(outfile);
    }
    
    // Cleanup dummy file
    cleanup_dummy_file();
    
    return 0;
}