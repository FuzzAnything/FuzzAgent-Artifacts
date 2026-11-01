// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_process_data_pause at pngpread.c:65:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <setjmp.h>

// Custom error handler that does not abort, allowing fuzzing to continue
static void custom_error_handler(png_structp png_ptr, const char *error_message) {
    (void)png_ptr;
    (void)error_message;
}

// Custom warning handler
static void custom_warning_handler(png_structp png_ptr, const char *warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

// Custom malloc
static void *custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    if (size == 0) return NULL;
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

// Custom free
static void custom_free(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    free(ptr);
}

// Dummy write callback for png_write_flush to work without a real file
static void dummy_write_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    
    // Create write struct with custom handlers
    png_ptr = png_create_write_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr, 
        custom_error_handler,
        custom_warning_handler,
        nullptr, 
        custom_malloc,
        custom_free
    );

    if (!png_ptr) {
        return 0;
    }

    // Initialize info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up a dummy write function
    png_set_write_fn(png_ptr, nullptr, dummy_write_callback, nullptr);

    // Use setjmp to handle fatal errors gracefully
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Test png_set_compression_buffer_size
    size_t buffer_size = 0;
    if (Size >= 4) {
        buffer_size = *(const uint32_t*)(Data);
    } else {
        buffer_size = Size;
    }
    
    if (buffer_size > 2147483647UL) buffer_size = 2147483647UL;
    
    png_set_compression_buffer_size(png_ptr, buffer_size);

    // Test png_warning
    const char *warning_msg = "Fuzz warning";
    if (Size > 10) {
        char msg_buf[256];
        size_t msg_len = (Size < 255) ? Size : 255;
        memcpy(msg_buf, Data, msg_len);
        msg_buf[msg_len] = '\0';
        warning_msg = msg_buf;
    }
    png_warning(png_ptr, warning_msg);

    // Test png_write_flush
    png_write_flush(png_ptr);

    // Test png_error
    if (Size > 0 && Data[0] == 0xFF) {
        const char *error_msg = "Fuzz fatal error";
        if (Size > 15) {
            char err_buf[256];
            size_t err_len = (Size < 255) ? Size : 255;
            memcpy(err_buf, Data, err_len);
            err_buf[err_len] = '\0';
            error_msg = err_buf;
        }
        png_error(png_ptr, error_msg);
    }

    // Test png_process_data_pause
    // Ensure we have enough data before accessing Data[1] and Data[2]
    if (Size > 2 && Data[1] == 0xAA) {
        int save = (Data[2] % 2);
        size_t remaining = png_process_data_pause(png_ptr, save);
        (void)remaining;
    }

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}