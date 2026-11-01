// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_write_to_stdio at pngwrite.c:2353:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_begin_read_from_memory at pngread.c:1523:5 in png.h
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
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void create_dummy_png_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    // Allocate buffers for testing
    size_t max_buffer = Size * 4 + 1024;
    void *buffer = malloc(max_buffer);
    png_color *colormap = (png_color *)malloc(256 * sizeof(png_color));
    png_color background = {0, 0, 0};
    
    if (!buffer || !colormap) {
        free(buffer);
        free(colormap);
        return 0;
    }

    // Initialize colormap with dummy data
    for (int i = 0; i < 256; i++) {
        colormap[i].red = i;
        colormap[i].green = i;
        colormap[i].blue = i;
    }

    // Initialize image buffer with input data
    memset(buffer, 0, max_buffer);
    if (Size > 0) {
        memcpy(buffer, Data, Size);
    }

    // Fuzz png_image_begin_read_from_memory
    {
        png_image img;
        memset(&img, 0, sizeof(img));
        img.version = PNG_IMAGE_VERSION;
        png_image_begin_read_from_memory(&img, Data, Size);
        if (img.opaque) {
            void *read_buf = malloc(max_buffer);
            if (read_buf) {
                png_image_finish_read(&img, &background, read_buf, 0, colormap);
                free(read_buf);
            }
            png_image_free(&img);
        }
    }

    // Fuzz png_image_begin_read_from_stdio
    {
        create_dummy_png_file(Data, Size);
        FILE *f = fopen("./dummy_file", "rb");
        if (f) {
            png_image img;
            memset(&img, 0, sizeof(img));
            img.version = PNG_IMAGE_VERSION;
            png_image_begin_read_from_stdio(&img, f);
            if (img.opaque) {
                void *read_buf = malloc(max_buffer);
                if (read_buf) {
                    png_image_finish_read(&img, &background, read_buf, 0, colormap);
                    free(read_buf);
                }
                png_image_free(&img);
            }
            fclose(f);
        }
    }

    // Fuzz png_image_write_to_memory
    {
        png_image img;
        memset(&img, 0, sizeof(img));
        img.version = PNG_IMAGE_VERSION;
        img.width = (Size > 0 && Data[0] > 0) ? Data[0] : 1;
        img.height = (Size > 1 && Data[1] > 0) ? Data[1] : 1;
        img.format = PNG_FORMAT_RGB;
        // Note: png_image struct does not have a 'colormap' member for writing configuration directly.
        // The colormap is passed as an argument to the write function.
        
        png_alloc_size_t mem_size = 0;
        // Query size first
        png_image_write_to_memory(&img, NULL, &mem_size, 0, buffer, 0, colormap);
        
        if (mem_size > 0) {
            void *mem = malloc(mem_size);
            if (mem) {
                png_image_write_to_memory(&img, mem, &mem_size, 0, buffer, 0, colormap);
                free(mem);
            }
        }
        png_image_free(&img);
    }

    // Fuzz png_image_write_to_stdio
    {
        png_image img;
        memset(&img, 0, sizeof(img));
        img.version = PNG_IMAGE_VERSION;
        img.width = (Size > 0 && Data[0] > 0) ? Data[0] : 1;
        img.height = (Size > 1 && Data[1] > 0) ? Data[1] : 1;
        img.format = PNG_FORMAT_RGB;

        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            png_image_write_to_stdio(&img, f, 0, buffer, 0, colormap);
            fclose(f);
        }
        png_image_free(&img);
    }

    // Cleanup
    free(buffer);
    free(colormap);
    png_image_free(&image);
    
    return 0;
}