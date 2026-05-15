// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_memory at pngread.c:1523:5 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_write_to_stdio at pngwrite.c:2353:1 in png.h
// png_image_begin_read_from_file at pngread.c:1455:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_stdio at pngread.c:1425:1 in png.h
// png_image_free at png.c:3989:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t* Data, size_t Size) {
    if (Size < 100) return 0;

    // Initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    // Test png_image_begin_read_from_memory
    if (png_image_begin_read_from_memory(&image, Data, Size) == 0) {
        png_image_free(&image);
        return 0;
    }
    
    // Allocate buffer for image data
    png_uint_32 buffer_size = PNG_IMAGE_SIZE(image);
    void* buffer = malloc(buffer_size);
    if (!buffer) {
        png_image_free(&image);
        return 0;
    }
    
    // Test png_image_finish_read with different background colors
    png_color background1 = {0, 0, 0};
    png_color background2 = {255, 255, 255};
    png_color* backgrounds[] = {&background1, &background2, nullptr};
    
    for (int i = 0; i < 3; i++) {
        png_image image_copy = image;
        if (png_image_finish_read(&image_copy, backgrounds[i], buffer, 0, nullptr) == 0) {
            break;
        }
    }
    
    // Test writing to file
    write_dummy_file(Data, Size);
    FILE* file = fopen("./dummy_file", "rb+");
    if (file) {
        png_image_write_to_stdio(&image, file, 0, buffer, 0, nullptr);
        fclose(file);
    }
    cleanup_dummy_file();
    
    // Test png_image_begin_read_from_file
    write_dummy_file(Data, Size);
    png_image image2;
    memset(&image2, 0, sizeof(image2));
    image2.version = PNG_IMAGE_VERSION;
    
    if (png_image_begin_read_from_file(&image2, "./dummy_file") == 0) {
        png_image_free(&image2);
    } else {
        png_image_free(&image2);
    }
    cleanup_dummy_file();
    
    // Test png_image_begin_read_from_stdio
    write_dummy_file(Data, Size);
    FILE* stdio_file = fopen("./dummy_file", "rb");
    if (stdio_file) {
        png_image image3;
        memset(&image3, 0, sizeof(image3));
        image3.version = PNG_IMAGE_VERSION;
        
        if (png_image_begin_read_from_stdio(&image3, stdio_file) == 0) {
            png_image_free(&image3);
        } else {
            png_image_free(&image3);
        }
        fclose(stdio_file);
    }
    cleanup_dummy_file();
    
    // Cleanup
    free(buffer);
    png_image_free(&image);
    
    return 0;
}