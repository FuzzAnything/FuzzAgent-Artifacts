// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_set_crc_action at pngrtran.c:24:1 in png.h
// png_set_sCAL_fixed at pngset.c:694:1 in png.h
// png_image_write_to_file at pngwrite.c:2402:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_sCAL_s at pngset.c:598:1 in png.h
// png_set_option at png.c:3740:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <cstdio>

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function
}

static void dummy_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Dummy error handler
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Dummy warning handler
}

extern "C" int LLVMFuzzerTestOneInput_418(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize png_struct and png_info
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up dummy I/O functions
    png_set_write_fn(png_ptr, nullptr, dummy_write_data, nullptr);
    png_set_read_fn(png_ptr, nullptr, dummy_read_data);

    // Use input data to determine parameters
    const uint8_t* data_ptr = Data;
    size_t remaining = Size;

    // Extract unit parameter (must be 1 or 2)
    int unit = 1;
    if (remaining > 0) {
        unit = (data_ptr[0] % 2) + 1;  // Ensures 1 or 2
        data_ptr++;
        remaining--;
    }

    // Extract string parameters for png_set_sCAL_s
    char swidth[256] = "1.0";
    char sheight[256] = "1.0";
    
    if (remaining > 0) {
        size_t width_len = remaining > 255 ? 255 : remaining;
        memcpy(swidth, data_ptr, width_len);
        swidth[width_len] = '\0';
        data_ptr += width_len;
        remaining -= width_len;
    }
    
    if (remaining > 0) {
        size_t height_len = remaining > 255 ? 255 : remaining;
        memcpy(sheight, data_ptr, height_len);
        sheight[height_len] = '\0';
        data_ptr += height_len;
        remaining -= height_len;
    }

    // Call png_set_sCAL_s with validated unit
    png_set_sCAL_s(png_ptr, info_ptr, unit, swidth, sheight);

    // Call png_set_option with random parameters
    if (remaining > 0) {
        int option = *data_ptr;
        data_ptr++;
        remaining--;
        
        int onoff = 0;
        if (remaining > 0) {
            onoff = *data_ptr % 2;
            data_ptr++;
            remaining--;
        }
        
        png_set_option(png_ptr, option, onoff);
    }

    // Call png_set_benign_errors
    if (remaining > 0) {
        int allowed = *data_ptr % 2;
        data_ptr++;
        remaining--;
        png_set_benign_errors(png_ptr, allowed);
    }

    // Call png_set_crc_action
    if (remaining > 0) {
        int crit_action = *data_ptr;
        data_ptr++;
        remaining--;
        
        int ancil_action = 0;
        if (remaining > 0) {
            ancil_action = *data_ptr;
            data_ptr++;
            remaining--;
        }
        
        png_set_crc_action(png_ptr, crit_action, ancil_action);
    }

    // Call png_set_sCAL_fixed
    if (remaining >= 8) {
        png_fixed_point width, height;
        memcpy(&width, data_ptr, 4);
        memcpy(&height, data_ptr + 4, 4);
        data_ptr += 8;
        remaining -= 8;
        
        png_set_sCAL_fixed(png_ptr, info_ptr, unit, width, height);
    }

    // Test png_image_write_to_file
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = 64;
    image.height = 64;
    image.format = PNG_FORMAT_RGBA;
    
    // Create dummy buffer
    size_t buffer_size = PNG_IMAGE_SIZE(image);
    void* buffer = malloc(buffer_size);
    if (buffer) {
        // Fill buffer with some data
        if (remaining > 0) {
            size_t copy_size = remaining > buffer_size ? buffer_size : remaining;
            memcpy(buffer, data_ptr, copy_size);
        }
        
        // Write to dummy file
        png_image_write_to_file(&image, "./dummy_file", 0, buffer, 0, nullptr);
        
        free(buffer);
    }

    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}