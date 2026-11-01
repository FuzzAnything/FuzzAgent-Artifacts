// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
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
#include <setjmp.h>

// Custom error handler that uses longjmp to return control to the fuzz harness
static void custom_png_error(png_structp png_ptr, png_const_charp error_message) {
    (void)error_message;
    longjmp(png_jmpbuf(png_ptr), 1);
}

// Custom warning handler
static void custom_png_warning(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
    return;
}

// Custom malloc wrapper
static png_voidp custom_malloc(png_structp png_ptr, png_size_t size) {
    (void)png_ptr;
    if (size == 0) return nullptr;
    void *ptr = malloc(size);
    if (!ptr) {
        // Simulate out of memory error if needed
    }
    return ptr;
}

// Custom free wrapper
static void custom_free(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    free(ptr);
}

// Custom write callback for flushing data
static void custom_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
    // Discard data for fuzzing
}

// Custom flush callback
static void custom_flush(png_structp png_ptr) {
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // 1. Prepare the environment
    // Create write struct with custom handlers
    png_ptr = png_create_write_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr, // error_ptr
        custom_png_error,
        custom_png_warning,
        nullptr, // mem_ptr
        custom_malloc,
        custom_free
    );

    if (!png_ptr) {
        return 0;
    }

    // Create info struct
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        // If longjmp is called, we clean up and return
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize I/O callbacks
    png_set_write_fn(png_ptr, nullptr, custom_write_data, custom_flush);

    // 2. Invoke target API functions diversely

    // A. Test png_set_compression_buffer_size
    size_t buffer_size = 0;
    if (Size >= 2) {
        buffer_size = static_cast<size_t>(Data[0]) | (static_cast<size_t>(Data[1]) << 8);
    }
    
    // Force some edge cases for buffer size
    if (Size > 2 && Data[2] == 0xFF) {
        buffer_size = 0; // Invalid size (0)
    } else if (Size > 2 && Data[2] == 0xFE) {
        buffer_size = 2147483648ULL; // > 2GB (Invalid)
    } else if (buffer_size == 0) {
        buffer_size = 1024; // Default valid size
    }
    
    // Call png_set_compression_buffer_size
    png_set_compression_buffer_size(png_ptr, buffer_size);

    // B. Test png_warning
    // Create a dummy warning message from fuzz data safely
    if (Size > 4) {
        // Ensure we don't read past the buffer
        size_t max_msg_len = 255;
        size_t available = Size - 4;
        size_t msg_len = (available < max_msg_len) ? available : max_msg_len;
        
        char warning_msg[256];
        // Safe copy
        memcpy(warning_msg, Data + 4, msg_len);
        warning_msg[msg_len] = '\0';
        
        png_warning(png_ptr, warning_msg);
    }

    // C. Test png_error
    // This will trigger the custom error handler which longjmps back to setjmp
    if (Size > 10 && Data[10] == 0x42) {
        size_t max_msg_len = 255;
        size_t available = Size - 10;
        size_t msg_len = (available < max_msg_len) ? available : max_msg_len;
        
        char error_msg[256];
        memcpy(error_msg, Data + 10, msg_len);
        error_msg[msg_len] = '\0';
        
        png_error(png_ptr, error_msg);
        // If we reach here, it means the custom handler returned (which shouldn't happen with longjmp)
        // or the error wasn't triggered.
    }

    // D. Test png_write_flush
    // This requires the image to be in a state where flushing is possible.
    // We haven't written any rows, so this might be a no-op or trigger an error.
    png_write_flush(png_ptr);

    // E. Test png_process_data_pause
    // This function can only be called within png_process_data.
    // Calling it outside might cause undefined behavior, so we skip it.
    // size_t remaining = png_process_data_pause(png_ptr, 0);
    // (void)remaining;

    // 3. Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}