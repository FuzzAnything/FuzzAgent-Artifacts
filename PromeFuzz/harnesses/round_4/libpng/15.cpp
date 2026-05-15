// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_free at png.c:3989:1 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_begin_read_from_memory at pngread.c:1523:5 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_file at pngread.c:1455:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_stdio at pngread.c:1425:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_write_to_stdio at pngwrite.c:2353:1 in png.h
// png_image_write_to_stdio at pngwrite.c:2353:1 in png.h
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

#include "png.h"

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void process_image(png_image* image) {
    if (image->width > 0 && image->height > 0) {
        png_uint_32 buffer_size = PNG_IMAGE_SIZE(*image);
        if (buffer_size > 0 && buffer_size < 100 * 1024 * 1024) { // Limit to 100MB
            void* buffer = malloc(buffer_size);
            if (buffer) {
                // Try with different background colors
                png_color background_color = {128, 128, 128};
                png_image_finish_read(image, &background_color, buffer, 0, NULL);
                free(buffer);
            }
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* Data, size_t Size) {
    if (Size < 8) return 0; // Too small for PNG header

    // Test png_image_begin_read_from_memory
    png_image image_mem;
    memset(&image_mem, 0, sizeof(image_mem));
    image_mem.version = PNG_IMAGE_VERSION;
    
    if (png_image_begin_read_from_memory(&image_mem, Data, Size) != 0) {
        process_image(&image_mem);
        png_image_free(&image_mem);
    }

    // Test png_image_begin_read_from_file
    write_dummy_file(Data, Size);
    
    png_image image_file;
    memset(&image_file, 0, sizeof(image_file));
    image_file.version = PNG_IMAGE_VERSION;
    
    if (png_image_begin_read_from_file(&image_file, "./dummy_file") != 0) {
        process_image(&image_file);
        png_image_free(&image_file);
    }

    // Test png_image_begin_read_from_stdio
    FILE* fp = fopen("./dummy_file", "rb");
    if (fp) {
        png_image image_stdio;
        memset(&image_stdio, 0, sizeof(image_stdio));
        image_stdio.version = PNG_IMAGE_VERSION;
        
        if (png_image_begin_read_from_stdio(&image_stdio, fp) != 0) {
            process_image(&image_stdio);
            png_image_free(&image_stdio);
        }
        fclose(fp);
    }

    // Test png_image_write_to_stdio with a simple image
    png_image write_image;
    memset(&write_image, 0, sizeof(write_image));
    write_image.version = PNG_IMAGE_VERSION;
    write_image.width = 16;
    write_image.height = 16;
    write_image.format = PNG_FORMAT_RGBA;
    
    size_t write_buffer_size = 16 * 16 * 4; // RGBA
    void* write_buffer = malloc(write_buffer_size);
    if (write_buffer) {
        memset(write_buffer, 0x80, write_buffer_size); // Gray color
        
        FILE* write_fp = fopen("./dummy_file_write", "wb");
        if (write_fp) {
            // Try both 8-bit and non-8-bit conversions
            png_image_write_to_stdio(&write_image, write_fp, 0, write_buffer, 0, NULL);
            png_image_write_to_stdio(&write_image, write_fp, 1, write_buffer, 0, NULL);
            fclose(write_fp);
        }
        free(write_buffer);
    }
    png_image_free(&write_image);

    return 0;
}