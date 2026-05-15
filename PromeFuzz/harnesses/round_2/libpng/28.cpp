// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_write_to_file at pngwrite.c:2402:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_set_crc_action at pngrtran.c:24:1 in png.h
// png_benign_error at pngerror.c:308:1 in png.h
// png_chunk_warning at pngerror.c:436:1 in png.h
// png_chunk_error at pngerror.c:418:1 in png.h
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
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

static jmp_buf error_jmp_buf;

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(error_jmp_buf, 1);
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize png_struct
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, error_handler, warning_handler);
    if (!png_ptr) return 0;

    // Initialize png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Set up error handling for png_chunk_error
    if (setjmp(error_jmp_buf)) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Use fuzz data to determine which functions to call
    uint8_t selector = Data[0];
    const uint8_t* remaining_data = Data + 1;
    size_t remaining_size = Size - 1;

    // Create a null-terminated error message from remaining data
    char error_msg[256];
    if (remaining_size > 0) {
        size_t copy_size = remaining_size < 255 ? remaining_size : 255;
        memcpy(error_msg, remaining_data, copy_size);
        error_msg[copy_size] = '\0';
    } else {
        error_msg[0] = '\0';
    }

    // Call different API functions based on selector
    switch (selector % 7) {
        case 0:
            png_set_benign_errors(png_ptr, selector & 1);
            break;
        case 1:
            png_set_crc_action(png_ptr, selector, selector >> 1);
            break;
        case 2:
            png_benign_error(png_ptr, error_msg);
            break;
        case 3:
            png_chunk_warning(png_ptr, error_msg);
            break;
        case 4:
            // Test png_chunk_error with setjmp protection
            if (setjmp(error_jmp_buf) == 0) {
                png_chunk_error(png_ptr, error_msg);
            }
            break;
        case 5:
        case 6: {
            // Test png_image_write_to_file
            png_image image;
            memset(&image, 0, sizeof(image));
            image.version = PNG_IMAGE_VERSION;
            image.width = 16;
            image.height = 16;
            image.format = PNG_FORMAT_RGBA;
            
            // Create dummy buffer
            const size_t buffer_size = 16 * 16 * 4;
            uint8_t* buffer = (uint8_t*)malloc(buffer_size);
            if (buffer) {
                if (remaining_size > 0) {
                    size_t copy_size = remaining_size < buffer_size ? remaining_size : buffer_size;
                    memcpy(buffer, remaining_data, copy_size);
                }
                
                // Write to dummy file
                const char* filename = "./dummy_file";
                png_image_write_to_file(&image, filename, 0, buffer, 0, NULL);
                
                // Clean up dummy file
                unlink(filename);
                free(buffer);
            }
            break;
        }
    }

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}