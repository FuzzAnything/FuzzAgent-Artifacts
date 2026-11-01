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
#include <cstdlib>
#include <cstdio>
#include <cstring>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // Allocate a buffer for image data (max reasonable size for fuzzing)
    size_t max_buffer_size = 1024 * 1024; // 1MB
    void *buffer = nullptr;
    if (Size > 0) {
        buffer = malloc(max_buffer_size);
        if (!buffer) return 0;
        memset(buffer, 0, max_buffer_size);
    }

    // Case 1: png_image_begin_read_from_memory
    {
        png_image img_mem;
        memset(&img_mem, 0, sizeof(img_mem));
        img_mem.version = PNG_IMAGE_VERSION;
        
        // Try to read from memory
        png_image_begin_read_from_memory(&img_mem, Data, Size);
        
        // Try to finish read if memory read succeeded
        if (img_mem.opaque != nullptr) {
            png_image_finish_read(&img_mem, nullptr, buffer, 0, nullptr);
        }
        
        png_image_free(&img_mem);
    }

    // Case 2: png_image_begin_read_from_file
    {
        write_dummy_file(Data, Size);
        
        png_image img_file;
        memset(&img_file, 0, sizeof(img_file));
        img_file.version = PNG_IMAGE_VERSION;
        
        png_image_begin_read_from_file(&img_file, "./dummy_file");
        
        if (img_file.opaque != nullptr) {
            png_image_finish_read(&img_file, nullptr, buffer, 0, nullptr);
        }
        
        png_image_free(&img_file);
    }

    // Case 3: png_image_begin_read_from_stdio
    {
        FILE *f = fopen("./dummy_file", "rb");
        if (f) {
            png_image img_stdio;
            memset(&img_stdio, 0, sizeof(img_stdio));
            img_stdio.version = PNG_IMAGE_VERSION;
            
            png_image_begin_read_from_stdio(&img_stdio, f);
            
            if (img_stdio.opaque != nullptr) {
                png_image_finish_read(&img_stdio, nullptr, buffer, 0, nullptr);
            }
            
            png_image_free(&img_stdio);
            fclose(f);
        }
    }

    // Case 4: png_set_interlace_handling (requires a valid png_struct)
    // Note: This is a bit tricky as we need a valid png_struct from libpng
    // We'll try to create one via the old API if possible, but it's complex
    // For fuzzing purposes, we'll skip this as it requires more setup
    // and the other functions are more critical for the image reading path

    // Cleanup
    if (buffer) {
        free(buffer);
    }

    return 0;
}