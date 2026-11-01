// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_memory at pngread.c:1523:5 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_write_to_stdio at pngwrite.c:2353:1 in png.h
// png_image_write_to_file at pngwrite.c:2402:1 in png.h
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

static void setup_png_image(png_image *image) {
    if (image == nullptr) return;
    memset(image, 0, sizeof(*image));
    image->version = PNG_IMAGE_VERSION;
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_image image;
    setup_png_image(&image);

    // Use input data as the image source
    int read_result = png_image_begin_read_from_memory(&image, Data, Size);
    
    // Only proceed if the header was parsed successfully
    if (read_result == 0) {
        png_image_free(&image);
        return 0;
    }

    // Validate dimensions to prevent overflow in buffer allocation
    if (image.width == 0 || image.height == 0) {
        png_image_free(&image);
        return 0;
    }

    // Calculate buffer size safely
    // Use a conservative multiplier for bytes per pixel (max 4 for RGBA)
    // Check for overflow before multiplication
    if (image.width > 10000 || image.height > 10000) {
        png_image_free(&image);
        return 0;
    }

    size_t bytes_per_row = image.width * 4; // Assume max 4 bytes per pixel
    size_t total_size = image.height * bytes_per_row;
    
    // Additional safety check
    if (total_size == 0 || total_size > 10 * 1024 * 1024) { // Limit to 10MB
        png_image_free(&image);
        return 0;
    }

    void *pixel_buffer = malloc(total_size);
    if (pixel_buffer == nullptr) {
        png_image_free(&image);
        return 0;
    }

    // Initialize buffer to avoid uninitialized memory issues
    memset(pixel_buffer, 0, total_size);

    // Try to finish reading
    png_color background = {0, 0, 0};
    png_image_finish_read(&image, &background, pixel_buffer, 0, nullptr);

    // Try writing to memory
    png_alloc_size_t memory_bytes = 0;
    void *write_memory = nullptr;
    
    // Query size first
    // Pass a valid row_stride (bytes per row) to avoid FPE in libpng
    png_int_32 row_stride = (png_int_32)bytes_per_row;
    
    int query_result = png_image_write_to_memory(&image, nullptr, &memory_bytes, 0, pixel_buffer, row_stride, nullptr);
    
    if (query_result == 0 && memory_bytes > 0) {
        write_memory = malloc(memory_bytes);
        if (write_memory != nullptr) {
            int write_result = png_image_write_to_memory(&image, write_memory, &memory_bytes, 0, pixel_buffer, row_stride, nullptr);
            (void)write_result;
            free(write_memory);
        }
    }

    // Try writing to a file
    const char *filename = "./dummy_file";
    FILE *file = fopen(filename, "wb");
    if (file != nullptr) {
        int file_write_result = png_image_write_to_stdio(&image, file, 0, pixel_buffer, row_stride, nullptr);
        (void)file_write_result;
        fclose(file);
        
        // Also try the high-level file write
        int high_level_result = png_image_write_to_file(&image, filename, 0, pixel_buffer, row_stride, nullptr);
        (void)high_level_result;
    }

    // Cleanup
    free(pixel_buffer);
    png_image_free(&image);
    
    // Remove dummy file if it exists
    remove(filename);

    return 0;
}