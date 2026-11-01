// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_write_to_stdio at pngwrite.c:2353:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_memory at pngread.c:1523:5 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_stdio at pngread.c:1425:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f != nullptr) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file for stdio operations
    write_dummy_file(Data, Size);

    // Allocate and initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // Buffer for pixel data
    size_t max_buffer_size = 1024 * 1024; // 1MB limit for safety
    void *buffer = nullptr;
    png_alloc_size_t buffer_size = 0;

    // Test png_image_begin_read_from_memory
    if (Size > 0) {
        if (png_image_begin_read_from_memory(&image, Data, Size)) {
            // Determine buffer size needed
            png_alloc_size_t required_size = 0;
            if (png_image_write_to_memory(&image, nullptr, &required_size, 0, nullptr, 0, nullptr) == 0) {
                if (required_size > 0 && required_size <= max_buffer_size) {
                    buffer = malloc(required_size);
                    if (buffer != nullptr) {
                        // Try to finish read
                        png_image_finish_read(&image, nullptr, buffer, 0, nullptr);
                        // Try to write to memory
                        png_image_write_to_memory(&image, buffer, &required_size, 0, buffer, 0, nullptr);
                        free(buffer);
                        buffer = nullptr;
                    }
                }
            }
            png_image_free(&image);
            memset(&image, 0, sizeof(image));
            image.version = PNG_IMAGE_VERSION;
        }
    }

    // Test png_image_begin_read_from_stdio
    FILE *f = fopen("./dummy_file", "rb");
    if (f != nullptr) {
        if (png_image_begin_read_from_stdio(&image, f)) {
            // Allocate buffer for reading
            png_alloc_size_t required_size = 0;
            if (png_image_write_to_memory(&image, nullptr, &required_size, 0, nullptr, 0, nullptr) == 0) {
                if (required_size > 0 && required_size <= max_buffer_size) {
                    buffer = malloc(required_size);
                    if (buffer != nullptr) {
                        png_image_finish_read(&image, nullptr, buffer, 0, nullptr);
                        free(buffer);
                        buffer = nullptr;
                    }
                }
            }
            png_image_free(&image);
            memset(&image, 0, sizeof(image));
            image.version = PNG_IMAGE_VERSION;
        }
        fclose(f);
    }

    // Test png_image_write_to_stdio
    f = fopen("./dummy_file", "wb");
    if (f != nullptr) {
        // Prepare a simple image structure for writing
        image.width = 10;
        image.height = 10;
        image.format = PNG_FORMAT_RGB;
        image.flags = 0; // Use 0 instead of undefined macro
        
        // Allocate buffer for source image
        size_t row_stride = image.width * 3; // RGB
        buffer = malloc(image.height * row_stride);
        if (buffer != nullptr) {
            // Fill with dummy data
            memset(buffer, 0xFF, image.height * row_stride);
            
            png_image_write_to_stdio(&image, f, 0, buffer, row_stride, nullptr);
            
            free(buffer);
            buffer = nullptr;
        }
        fclose(f);
        png_image_free(&image);
    }

    // Test png_image_write_to_memory with writing
    if (Size > 0) {
        image.width = 10;
        image.height = 10;
        image.format = PNG_FORMAT_RGB;
        image.flags = 0; // Use 0 instead of undefined macro
        
        size_t row_stride = image.width * 3;
        buffer = malloc(image.height * row_stride);
        if (buffer != nullptr) {
            memset(buffer, 0xFF, image.height * row_stride);
            
            png_alloc_size_t mem_bytes = 0;
            // First query size
            if (png_image_write_to_memory(&image, nullptr, &mem_bytes, 0, buffer, row_stride, nullptr) == 0) {
                if (mem_bytes > 0 && mem_bytes <= max_buffer_size) {
                    void *mem = malloc(mem_bytes);
                    if (mem != nullptr) {
                        png_image_write_to_memory(&image, mem, &mem_bytes, 0, buffer, row_stride, nullptr);
                        free(mem);
                    }
                }
            }
            free(buffer);
            buffer = nullptr;
        }
        png_image_free(&image);
    }

    // Cleanup
    if (buffer != nullptr) {
        free(buffer);
    }

    return 0;
}