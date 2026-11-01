// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_begin_read_from_memory at pngread.c:1523:5 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_file at pngread.c:1455:1 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_stdio at pngread.c:1425:1 in png.h
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
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for file-based APIs
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f != nullptr) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Allocate and initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // Allocate buffer for image data
    // We use a fixed large size to avoid allocation issues during fuzzing
    const size_t max_buffer_size = 1024 * 1024; // 1MB
    uint8_t *buffer = nullptr;
    if (Size > 0 && Size < max_buffer_size) {
        buffer = (uint8_t *)malloc(Size);
        if (buffer == nullptr) {
            return 0;
        }
        memcpy(buffer, Data, Size);
    } else if (Size >= max_buffer_size) {
        buffer = (uint8_t *)malloc(max_buffer_size);
        if (buffer == nullptr) {
            return 0;
        }
        memcpy(buffer, Data, max_buffer_size);
    }

    // Define background color for transparency handling
    png_color background = {0, 0, 0};

    // Test 1: png_image_begin_read_from_memory
    if (buffer != nullptr) {
        size_t mem_size = (Size < max_buffer_size) ? Size : max_buffer_size;
        png_image image_mem = image;
        if (png_image_begin_read_from_memory(&image_mem, buffer, mem_size)) {
            // Try to finish reading
            png_image_finish_read(&image_mem, &background, buffer, 0, nullptr);
            png_image_free(&image_mem);
        }
    }

    // Test 2: png_image_begin_read_from_file
    if (f != nullptr) {
        png_image image_file = image;
        if (png_image_begin_read_from_file(&image_file, dummy_file)) {
            png_image_finish_read(&image_file, &background, buffer, 0, nullptr);
            png_image_free(&image_file);
        }
    }

    // Test 3: png_image_begin_read_from_stdio
    if (f != nullptr) {
        FILE *stdio_file = fopen(dummy_file, "rb");
        if (stdio_file != nullptr) {
            png_image image_stdio = image;
            if (png_image_begin_read_from_stdio(&image_stdio, stdio_file)) {
                png_image_finish_read(&image_stdio, &background, buffer, 0, nullptr);
                png_image_free(&image_stdio);
            }
            fclose(stdio_file);
        }
    }

    // Test 4: png_set_interlace_handling (requires png_struct)
    // Note: This function requires a png_struct, which is typically created via png_create_read_struct
    // Since we are focusing on png_image APIs, we skip direct testing of png_set_interlace_handling
    // as it requires a different initialization path not covered by png_image_begin_* functions.

    // Cleanup
    if (buffer != nullptr) {
        free(buffer);
    }
    if (f != nullptr) {
        remove(dummy_file);
    }

    return 0;
}