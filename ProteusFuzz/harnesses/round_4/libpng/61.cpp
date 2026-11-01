// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_set_quantize at pngrtran.c:472:1 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_write_to_file at pngwrite.c:2402:1 in png.h
// png_image_write_to_stdio at pngwrite.c:2353:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Use the first byte to select which API to fuzz
    int mode = Data[0] % 6;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    // Common setup for image structures
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // Allocate buffers for testing
    size_t max_buf_size = 1024 * 1024; // 1MB limit for safety
    if (payload_size > max_buf_size) payload_size = max_buf_size;

    void *buffer = nullptr;
    void *colormap = nullptr;
    png_color *palette = nullptr;
    png_uint_16 *histogram = nullptr;
    png_alloc_size_t memory_bytes = 0;
    void *memory = nullptr;

    // Initialize palette and histogram if needed
    int num_palette = 0;
    if (payload_size >= 6) {
        num_palette = (payload[0] % 256) + 1;
        int max_colors = (payload[1] % 256) + 1;
        if (num_palette > 0 && num_palette <= 256) {
            palette = (png_color *)malloc(num_palette * sizeof(png_color));
            if (palette) {
                for (int i = 0; i < num_palette; ++i) {
                    palette[i].red = payload[2 + (i * 3) % payload_size];
                    palette[i].green = payload[3 + (i * 3) % payload_size];
                    palette[i].blue = payload[4 + (i * 3) % payload_size];
                }
            }
        }
        if (num_palette > 0 && palette) {
            histogram = (png_uint_16 *)malloc(num_palette * sizeof(png_uint_16));
            if (histogram) {
                for (int i = 0; i < num_palette; ++i) {
                    histogram[i] = payload[(5 + i) % payload_size];
                }
            }
        }
    }

    // Allocate buffer for pixel data if needed
    // Ensure we have enough data to create a valid buffer if we are going to use it
    if (payload_size > 0) {
        buffer = malloc(payload_size);
        if (buffer) {
            memcpy(buffer, payload, payload_size);
        }
    }

    // Allocate colormap if needed
    if (palette && num_palette > 0) {
        colormap = malloc(num_palette * sizeof(png_color));
        if (colormap) {
            memcpy(colormap, palette, num_palette * sizeof(png_color));
        }
    }

    int result = 0;
    int full_quantize = 0;

    // Helper to safely read payload bytes
    auto get_byte = [&](size_t idx) -> uint8_t {
        if (idx < payload_size) return payload[idx];
        return 0;
    };

    switch (mode) {
        case 0: {
            // Fuzz png_image_write_to_memory
            image.width = (get_byte(0) % 1024) + 1;
            image.height = (get_byte(1) % 1024) + 1;
            image.format = (get_byte(2) % 4) + 1; // 1 to 4
            image.flags = (get_byte(3) % 10);

            // Calculate required buffer size based on dimensions and format
            // PNG_ROWBYTES is a macro: ((width * bit_depth) / 8) + 1
            // We approximate max bytes per pixel as 4 (32-bit) for safety if format is unknown or large
            // However, libpng calculates exact size. We must ensure buffer is large enough.
            // A safe upper bound for row_stride is width * 4 + 1.
            // Total size = height * row_stride.
            // To prevent overflow in calculation, check limits.
            png_alloc_size_t row_stride = image.width * 4 + 1; 
            png_alloc_size_t required_size = image.height * row_stride;
            
            if (required_size > max_buf_size) {
                // If required size exceeds limit, skip or resize dimensions (here we skip)
                break; 
            }

            if (buffer == nullptr || required_size > payload_size) {
                free(buffer);
                buffer = malloc(required_size);
                if (buffer) memset(buffer, 0, required_size);
            }

            // First call to get size
            result = png_image_write_to_memory(&image, nullptr, &memory_bytes, 0, buffer, 0, colormap);
            if (result == 0 && memory_bytes > 0) {
                memory = malloc(memory_bytes);
                if (memory) {
                    result = png_image_write_to_memory(&image, memory, &memory_bytes, 0, buffer, 0, colormap);
                    free(memory);
                }
            }
            break;
        }
        case 1: {
            // Fuzz png_set_quantize
            png_structp png_ptr = nullptr;
            int local_num_palette = (get_byte(0) % 256) + 1;
            int maximum_colors = (get_byte(1) % 256) + 1;
            full_quantize = (get_byte(2) % 2);
            
            if (palette) {
                png_set_quantize(png_ptr, palette, local_num_palette, maximum_colors, histogram, full_quantize);
            }
            break;
        }
        case 2: {
            // Fuzz png_image_finish_read
            image.width = (get_byte(0) % 1024) + 1;
            image.height = (get_byte(1) % 1024) + 1;
            image.format = (get_byte(2) % 4) + 1;
            
            // Calculate required buffer size
            png_alloc_size_t row_stride = image.width * 4 + 1; 
            png_alloc_size_t required_size = image.height * row_stride;
            
            if (required_size > max_buf_size) break;

            if (buffer == nullptr || required_size > payload_size) {
                free(buffer);
                buffer = malloc(required_size);
                if (buffer) memset(buffer, 0, required_size);
            }
            
            result = png_image_finish_read(&image, nullptr, buffer, 0, colormap);
            break;
        }
        case 3: {
            // Fuzz png_image_write_to_file
            image.width = (get_byte(0) % 1024) + 1;
            image.height = (get_byte(1) % 1024) + 1;
            image.format = (get_byte(2) % 4) + 1;
            image.flags = (get_byte(3) % 10);
            
            // Calculate required buffer size
            png_alloc_size_t row_stride = image.width * 4 + 1; 
            png_alloc_size_t required_size = image.height * row_stride;
            
            if (required_size > max_buf_size) break;

            if (buffer == nullptr || required_size > payload_size) {
                free(buffer);
                buffer = malloc(required_size);
                if (buffer) memset(buffer, 0, required_size);
            }

            const char *filename = "./dummy_file";
            result = png_image_write_to_file(&image, filename, 0, buffer, 0, colormap);
            break;
        }
        case 4: {
            // Fuzz png_image_write_to_stdio
            image.width = (get_byte(0) % 1024) + 1;
            image.height = (get_byte(1) % 1024) + 1;
            image.format = (get_byte(2) % 4) + 1;
            image.flags = (get_byte(3) % 10);
            
            // Calculate required buffer size to prevent heap-buffer-overflow
            // Max bytes per pixel is 4 (RGBA) + 1 for filter byte per row
            png_alloc_size_t row_stride = image.width * 4 + 1; 
            png_alloc_size_t required_size = image.height * row_stride;
            
            // Safety check to prevent integer overflow or excessive allocation
            if (required_size > max_buf_size) break;

            if (buffer == nullptr || required_size > payload_size) {
                free(buffer);
                buffer = malloc(required_size);
                if (buffer) memset(buffer, 0, required_size);
            }

            FILE *file = fopen("./dummy_file", "wb");
            if (file) {
                result = png_image_write_to_stdio(&image, file, 0, buffer, 0, colormap);
                fclose(file);
            }
            break;
        }
        case 5: {
            // Fuzz png_image_free
            png_image_free(&image);
            break;
        }
    }

    // Cleanup
    if (buffer) free(buffer);
    if (colormap) free(colormap);
    if (palette) free(palette);
    if (histogram) free(histogram);

    return 0;
}