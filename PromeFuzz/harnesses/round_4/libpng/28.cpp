// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_chunk_error at pngerror.c:418:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_benign_error at pngerror.c:308:1 in png.h
// png_chunk_warning at pngerror.c:436:1 in png.h
// png_chunk_benign_error at pngerror.c:453:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <csetjmp>
#include <cstdlib>

#include "png.h"

// Custom error handling structure
struct fuzz_error_data {
    jmp_buf jmpbuf;
    int error_occurred;
    char* allocated_msg;
};

// Error handler that uses longjmp instead of aborting
static void fuzz_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    fuzz_error_data* error_data = static_cast<fuzz_error_data*>(png_get_error_ptr(png_ptr));
    if (error_data) {
        // Free allocated message if it exists
        if (error_data->allocated_msg) {
            free(error_data->allocated_msg);
            error_data->allocated_msg = nullptr;
        }
        error_data->error_occurred = 1;
        longjmp(error_data->jmpbuf, 1);
    }
}

// Warning handler that does nothing
static void fuzz_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - just absorb warnings
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Need at least some data for chunk names
    if (Size < 1) {
        return 0;
    }

    // Create error data for setjmp/longjmp error recovery
    fuzz_error_data error_data;
    error_data.error_occurred = 0;
    error_data.allocated_msg = nullptr;

    // Create PNG read structure with custom error handlers
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 &error_data,
                                                 fuzz_error_handler,
                                                 fuzz_warning_handler);
    if (!png_ptr) {
        return 0;
    }

    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up setjmp for error recovery
    if (setjmp(error_data.jmpbuf)) {
        // Error occurred, clean up and return
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        // Free allocated message if it exists (in case error handler didn't)
        if (error_data.allocated_msg) {
            free(error_data.allocated_msg);
            error_data.allocated_msg = nullptr;
        }
        return 0;
    }

    // Set chunk name for chunk-specific functions
    // Use first 4 bytes of input as chunk name, or "FUZZ" if not enough data
    png_byte chunk_name[5] = {0};
    if (Size >= 4) {
        memcpy(chunk_name, Data, 4);
    } else {
        memcpy(chunk_name, "FUZZ", 4);
    }
    chunk_name[4] = '\0';
    
    // Prepare error/warning message
    // Use remaining data as message, or default if none
    const char* message = "Fuzzer generated error";
    if (Size > 4) {
        // Create null-terminated string from remaining data
        size_t msg_len = Size - 4;
        char* dynamic_msg = static_cast<char*>(malloc(msg_len + 1));
        if (dynamic_msg) {
            memcpy(dynamic_msg, Data + 4, msg_len);
            dynamic_msg[msg_len] = '\0';
            message = dynamic_msg;
            error_data.allocated_msg = dynamic_msg;  // Store for cleanup
        }
    }

    // Determine which function to call based on input data
    // Use first byte modulo 6 to select function
    uint8_t func_selector = Data[0] % 6;

    switch (func_selector) {
        case 0:
            // png_chunk_error - will longjmp via error handler
            png_chunk_error(png_ptr, message);
            break;
        case 1:
            // png_error - will longjmp via error handler
            png_error(png_ptr, message);
            break;
        case 2:
            // png_warning - non-fatal
            png_warning(png_ptr, message);
            break;
        case 3:
            // png_benign_error - non-fatal by default
            png_benign_error(png_ptr, message);
            break;
        case 4:
            // png_chunk_warning - non-fatal
            png_chunk_warning(png_ptr, message);
            break;
        case 5:
            // png_chunk_benign_error - non-fatal by default
            png_chunk_benign_error(png_ptr, message);
            break;
    }

    // Clean up dynamically allocated message if we created one
    if (error_data.allocated_msg) {
        free(error_data.allocated_msg);
        error_data.allocated_msg = nullptr;
    }

    // Clean up PNG structures
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}