// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_image_write_to_memory at pngwrite.c:2286:1 in png.h
// png_set_filter at pngwrite.c:1058:1 in png.h
// png_image_write_to_stdio at pngwrite.c:2353:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_image_begin_read_from_stdio at pngread.c:1425:1 in png.h
// png_image_free at png.c:3989:1 in png.h
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
#include "code/png.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - suppress error output during fuzzing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - suppress warning output during fuzzing
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return nullptr;
    }
    return png_ptr;
}

static void cleanup_png_struct(png_structp png_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_320(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Write data to dummy file for functions that need it
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);

    // Test png_set_option
    png_structp png_ptr = create_png_struct();
    if (png_ptr) {
        // Use fuzzer data to determine option and onoff values
        int option = Data[0] % 10;  // Limited to reasonable range
        int onoff = Data[0] % 2;    // 0 or 1
        png_set_option(png_ptr, option, onoff);
        cleanup_png_struct(png_ptr);
    }

    // Test png_image_begin_read_from_stdio
    dummy_file = fopen("./dummy_file", "rb");
    if (dummy_file) {
        png_image image;
        memset(&image, 0, sizeof(image));
        image.version = PNG_IMAGE_VERSION;
        
        int result = png_image_begin_read_from_stdio(&image, dummy_file);
        // Result is ignored as we're testing for crashes
        fclose(dummy_file);
        
        // Cleanup if needed
        if (result == 0) {
            // If successful, we should free the image
            png_image_free(&image);
        }
    }

    // Test png_image_write_to_file
    {
        png_image image;
        memset(&image, 0, sizeof(image));
        image.version = PNG_IMAGE_VERSION;
        image.width = 100;
        image.height = 100;
        image.format = PNG_FORMAT_RGBA;
        
        // Create a dummy buffer
        size_t buffer_size = 100 * 100 * 4;
        void *buffer = malloc(buffer_size);
        if (buffer) {
            memset(buffer, Data[0], buffer_size);
            
            int convert_to_8bit = Data[0] % 2;
            png_int_32 row_stride = 100 * 4;
            
            png_image_write_to_file(&image, "./dummy_output.png", 
                                   convert_to_8bit, buffer, 
                                   row_stride, nullptr);
            free(buffer);
        }
    }

    // Test png_image_write_to_memory
    {
        png_image image;
        memset(&image, 0, sizeof(image));
        image.version = PNG_IMAGE_VERSION;
        image.width = 50;
        image.height = 50;
        image.format = PNG_FORMAT_RGBA;
        
        size_t buffer_size = 50 * 50 * 4;
        void *buffer = malloc(buffer_size);
        if (buffer) {
            memset(buffer, Data[0], buffer_size);
            
            int convert_to_8_bit = Data[0] % 2;
            png_int_32 row_stride = 50 * 4;
            png_alloc_size_t memory_bytes = 0;
            
            // First call to get required size
            png_image_write_to_memory(&image, nullptr, &memory_bytes, 
                                     convert_to_8_bit, buffer, 
                                     row_stride, nullptr);
            
            // Second call with allocated memory if size is reasonable
            if (memory_bytes > 0 && memory_bytes < 1024 * 1024) {
                void *memory = malloc(memory_bytes);
                if (memory) {
                    png_image_write_to_memory(&image, memory, &memory_bytes, 
                                             convert_to_8_bit, buffer, 
                                             row_stride, nullptr);
                    free(memory);
                }
            }
            free(buffer);
        }
    }

    // Test png_set_filter
    png_ptr = create_png_struct();
    if (png_ptr) {
        int method = 0;  // As per documentation, must be 0
        int filters = Data[0] % 32;  // Filter types are bit flags
        png_set_filter(png_ptr, method, filters);
        cleanup_png_struct(png_ptr);
    }

    // Test png_image_write_to_stdio
    dummy_file = fopen("./dummy_output_stdio.png", "wb");
    if (dummy_file) {
        png_image image;
        memset(&image, 0, sizeof(image));
        image.version = PNG_IMAGE_VERSION;
        image.width = 80;
        image.height = 80;
        image.format = PNG_FORMAT_RGB;
        
        size_t buffer_size = 80 * 80 * 3;
        void *buffer = malloc(buffer_size);
        if (buffer) {
            memset(buffer, Data[0], buffer_size);
            
            int convert_to_8_bit = Data[0] % 2;
            png_int_32 row_stride = 80 * 3;
            
            png_image_write_to_stdio(&image, dummy_file, 
                                    convert_to_8_bit, buffer, 
                                    row_stride, nullptr);
            free(buffer);
        }
        fclose(dummy_file);
    }

    // Clean up temporary files
    remove("./dummy_file");
    remove("./dummy_output.png");
    remove("./dummy_output_stdio.png");

    return 0;
}