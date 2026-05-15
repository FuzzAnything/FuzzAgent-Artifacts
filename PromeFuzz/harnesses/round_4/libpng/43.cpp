// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_image_write_to_file at pngwrite.c:2402:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_image_write_to_stdio at pngwrite.c:2353:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_filter at pngwrite.c:1058:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "png.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void initialize_png_image(png_image& image, const uint8_t* data, size_t size) {
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    
    if (size >= 8) {
        image.width = (data[0] << 8) | data[1];
        image.height = (data[2] << 8) | data[3];
        image.format = data[4] % 16;
        image.flags = data[5];
        image.colormap_entries = data[6];
    } else {
        image.width = 64;
        image.height = 64;
        image.format = PNG_FORMAT_RGBA;
        image.flags = 0;
        image.colormap_entries = 0;
    }
    
    if (image.width == 0) image.width = 1;
    if (image.height == 0) image.height = 1;
    if (image.width > 1024) image.width = 1024;
    if (image.height > 1024) image.height = 1024;
}

static void* create_test_buffer(png_image& image, size_t* buffer_size) {
    size_t pixel_size = 0;
    
    switch (image.format) {
        case PNG_FORMAT_GRAY:
            pixel_size = 1;
            break;
        case PNG_FORMAT_GA:
            pixel_size = 2;
            break;
        case PNG_FORMAT_RGB:
            pixel_size = 3;
            break;
        case PNG_FORMAT_RGBA:
            pixel_size = 4;
            break;
        case PNG_FORMAT_GA | PNG_FORMAT_FLAG_LINEAR:
            pixel_size = 4;
            break;
        case PNG_FORMAT_RGBA | PNG_FORMAT_FLAG_LINEAR:
            pixel_size = 8;
            break;
        default:
            pixel_size = 4;
            image.format = PNG_FORMAT_RGBA;
            break;
    }
    
    *buffer_size = image.width * image.height * pixel_size;
    if (*buffer_size == 0) *buffer_size = 1;
    
    void* buffer = malloc(*buffer_size);
    if (buffer) {
        memset(buffer, 0xAA, *buffer_size);
    }
    return buffer;
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    png_image image;
    initialize_png_image(image, Data, Size);
    
    size_t buffer_size = 0;
    void* buffer = create_test_buffer(image, &buffer_size);
    if (!buffer) return 0;
    
    png_int_32 row_stride = 0;
    switch (image.format) {
        case PNG_FORMAT_GRAY:
            row_stride = image.width;
            break;
        case PNG_FORMAT_GA:
            row_stride = image.width * 2;
            break;
        case PNG_FORMAT_RGB:
            row_stride = image.width * 3;
            break;
        case PNG_FORMAT_RGBA:
            row_stride = image.width * 4;
            break;
        case PNG_FORMAT_GA | PNG_FORMAT_FLAG_LINEAR:
            row_stride = image.width * 4;
            break;
        case PNG_FORMAT_RGBA | PNG_FORMAT_FLAG_LINEAR:
            row_stride = image.width * 8;
            break;
        default:
            row_stride = image.width * 4;
            break;
    }
    
    int convert_to_8bit = (Size > 7) ? (Data[7] & 1) : 0;
    
    const char* dummy_file = "./dummy_file";
    
    FILE* file = fopen(dummy_file, "wb");
    if (!file) {
        free(buffer);
        return 0;
    }
    
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(file);
        free(buffer);
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(file);
        free(buffer);
        return 0;
    }
    
    uint8_t selector = (Size > 0) ? (Data[0] % 6) : 0;
    
    switch (selector) {
        case 0:
            png_image_write_to_file(&image, dummy_file, convert_to_8bit, buffer, row_stride, nullptr);
            break;
            
        case 1: {
            png_alloc_size_t memory_bytes = 0;
            png_image_write_to_memory(&image, nullptr, &memory_bytes, convert_to_8bit, buffer, row_stride, nullptr);
            
            if (memory_bytes > 0 && memory_bytes < 1024 * 1024) {
                void* memory = malloc(memory_bytes);
                if (memory) {
                    png_image_write_to_memory(&image, memory, &memory_bytes, convert_to_8bit, buffer, row_stride, nullptr);
                    free(memory);
                }
            }
            break;
        }
            
        case 2:
            if (Size > 1) {
                int option = Data[1] % 10;
                int onoff = Data[1] & 1;
                png_set_option(png_ptr, option, onoff);
            }
            break;
            
        case 3:
            png_image_write_to_stdio(&image, file, convert_to_8bit, buffer, row_stride, nullptr);
            break;
            
        case 4:
            if (Size > 1) {
                int strategy = Data[1] % 10;
                png_set_compression_strategy(png_ptr, strategy);
            }
            break;
            
        case 5:
            if (Size > 1) {
                int method = 0;
                int filters = Data[1] & 0xFF;
                png_set_filter(png_ptr, method, filters);
            }
            break;
    }
    
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(file);
    free(buffer);
    
    remove(dummy_file);
    return 0;
}