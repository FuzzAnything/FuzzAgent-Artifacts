// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_free at png.c:3989:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_write_to_file at pngwrite.c:2402:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_image_finish_read at pngread.c:4069:1 in png.h
// png_image_free at png.c:3989:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_quantize at pngrtran.c:472:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_image_free at png.c:3989:1 in png.h
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
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    int mode = Data[0] % 6;
    size_t offset = 1;

    // Allocate a buffer for image data if needed
    std::vector<uint8_t> buffer(1024, 0);
    std::vector<uint8_t> memory_buffer(1024, 0);
    std::vector<png_color> palette(256);
    std::vector<png_uint_16> histogram(256);

    // Initialize palette and histogram with fuzz data safely
    for (size_t i = 0; i < palette.size() && offset + 3 <= Size; ++i) {
        palette[i].red = Data[offset++];
        palette[i].green = Data[offset++];
        palette[i].blue = Data[offset++];
    }
    for (size_t i = 0; i < histogram.size() && offset + 2 <= Size; ++i) {
        histogram[i] = (Data[offset] << 8) | Data[offset + 1];
        offset += 2;
    }

    png_imagep image = nullptr;
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Helper lambda to check bounds
    auto safe_read = [&](size_t count) {
        if (offset + count > Size) return false;
        return true;
    };

    switch (mode) {
        case 0: {
            // Fuzz png_set_quantize
            png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            if (!png_ptr) return 0;
            
            info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr) {
                png_destroy_write_struct(&png_ptr, nullptr);
                return 0;
            }

            if (!safe_read(3)) {
                png_destroy_write_struct(&png_ptr, &info_ptr);
                return 0;
            }

            int num_palette = (Data[offset] % 256) + 1;
            int max_colors = (Data[offset + 1] % 256) + 1;
            int full_quantize = Data[offset + 2] % 2;
            offset += 3;

            // This might crash if png_ptr is not fully initialized, which is what we want to find.
            png_set_quantize(png_ptr, palette.data(), num_palette, max_colors, histogram.data(), full_quantize);
            
            png_destroy_write_struct(&png_ptr, &info_ptr);
            break;
        }
        case 1: {
            // Fuzz png_image_write_to_stdio
            image = (png_imagep)malloc(sizeof(png_image));
            if (!image) return 0;
            memset(image, 0, sizeof(png_image));
            
            if (!safe_read(5)) {
                png_image_free(image);
                return 0;
            }

            image->version = PNG_IMAGE_VERSION;
            image->width = Data[offset] % 100 + 1;
            image->height = Data[offset + 1] % 100 + 1;
            image->format = Data[offset + 2] % 6;
            offset += 3;

            int convert = Data[offset] % 2;
            offset++;
            
            png_int_32 stride = (Data[offset] << 8) | Data[offset + 1];
            if (stride < 0) stride = 0;
            offset += 2;

            FILE *file = fopen("./dummy_file", "wb");
            if (file) {
                png_image_write_to_stdio(image, file, convert, buffer.data(), stride, palette.data());
                fclose(file);
            }
            
            png_image_free(image);
            break;
        }
        case 2: {
            // Fuzz png_image_free
            image = (png_imagep)malloc(sizeof(png_image));
            if (!image) return 0;
            memset(image, 0, sizeof(png_image));
            // Free without initialization to test NULL checks
            png_image_free(image);
            
            // Also test with partially initialized
            image = (png_imagep)malloc(sizeof(png_image));
            if (image) {
                memset(image, 0, sizeof(png_image));
                image->version = PNG_IMAGE_VERSION;
                png_image_free(image);
            }
            break;
        }
        case 3: {
            // Fuzz png_image_write_to_memory
            image = (png_imagep)malloc(sizeof(png_image));
            if (!image) return 0;
            memset(image, 0, sizeof(png_image));
            
            if (!safe_read(5)) {
                png_image_free(image);
                return 0;
            }

            image->version = PNG_IMAGE_VERSION;
            image->width = Data[offset] % 100 + 1;
            image->height = Data[offset + 1] % 100 + 1;
            image->format = Data[offset + 2] % 6;
            offset += 3;

            int convert = Data[offset] % 2;
            offset++;
            
            png_int_32 stride = (Data[offset] << 8) | Data[offset + 1];
            if (stride < 0) stride = 0;
            offset += 2;

            png_alloc_size_t mem_bytes = memory_buffer.size();
            
            // First call to get size (memory = NULL)
            png_image_write_to_memory(image, nullptr, &mem_bytes, convert, buffer.data(), stride, palette.data());
            
            // Second call to write
            if (mem_bytes > 0) {
                memory_buffer.resize(mem_bytes);
                png_image_write_to_memory(image, memory_buffer.data(), &mem_bytes, convert, buffer.data(), stride, palette.data());
            }
            
            png_image_free(image);
            break;
        }
        case 4: {
            // Fuzz png_image_write_to_file
            image = (png_imagep)malloc(sizeof(png_image));
            if (!image) return 0;
            memset(image, 0, sizeof(png_image));
            
            if (!safe_read(5)) {
                png_image_free(image);
                return 0;
            }

            image->version = PNG_IMAGE_VERSION;
            image->width = Data[offset] % 100 + 1;
            image->height = Data[offset + 1] % 100 + 1;
            image->format = Data[offset + 2] % 6;
            offset += 3;

            int convert = Data[offset] % 2;
            offset++;
            
            png_int_32 stride = (Data[offset] << 8) | Data[offset + 1];
            if (stride < 0) stride = 0;
            offset += 2;

            png_image_write_to_file(image, "./dummy_file", convert, buffer.data(), stride, palette.data());
            
            png_image_free(image);
            break;
        }
        case 5: {
            // Fuzz png_image_finish_read
            image = (png_imagep)malloc(sizeof(png_image));
            if (!image) return 0;
            memset(image, 0, sizeof(png_image));
            
            if (!safe_read(5)) {
                png_image_free(image);
                return 0;
            }

            image->version = PNG_IMAGE_VERSION;
            image->width = Data[offset] % 100 + 1;
            image->height = Data[offset + 1] % 100 + 1;
            image->format = Data[offset + 2] % 6;
            offset += 3;

            // Try to finish read without starting read (should fail gracefully)
            png_const_colorp bg = nullptr;
            png_int_32 stride = (Data[offset] << 8) | Data[offset + 1];
            if (stride < 0) stride = 0;
            offset += 2;

            png_image_finish_read(image, bg, buffer.data(), stride, palette.data());
            
            png_image_free(image);
            break;
        }
    }

    return 0;
}