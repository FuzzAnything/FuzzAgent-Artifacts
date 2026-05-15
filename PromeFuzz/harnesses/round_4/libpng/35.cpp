// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_chunk_error at pngerror.c:418:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_benign_error at pngerror.c:308:1 in png.h
// png_chunk_warning at pngerror.c:436:1 in png.h
// png_chunk_benign_error at pngerror.c:453:1 in png.h
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

static jmp_buf error_jmp_buf;

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Use longjmp to return control instead of aborting
    longjmp(error_jmp_buf, 1);
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing for warnings
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set custom error handler that uses longjmp
    png_set_error_fn(png_ptr, nullptr, error_handler, warning_handler);

    // Allocate error message buffer
    size_t msg_len = Size - 1;
    char* error_msg = static_cast<char*>(malloc(msg_len + 1));
    if (!error_msg) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    memcpy(error_msg, Data + 1, msg_len);
    error_msg[msg_len] = '\0';

    // Set up error recovery with setjmp
    if (setjmp(error_jmp_buf)) {
        // Error occurred, clean up and continue fuzzing
        free(error_msg);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Use first byte to select which function to test
    uint8_t function_selector = Data[0];

    // Call different error functions based on selector
    switch (function_selector % 5) {  // Changed from 6 to 5 to exclude png_longjmp
        case 0:
            png_chunk_error(png_ptr, error_msg);
            break;
        case 1:
            png_error(png_ptr, error_msg);
            break;
        case 2:
            // png_longjmp removed - it requires proper setup that we can't guarantee
            // Instead, call png_benign_error
            png_benign_error(png_ptr, error_msg);
            break;
        case 3:
            png_chunk_warning(png_ptr, error_msg);
            break;
        case 4:
            png_chunk_benign_error(png_ptr, error_msg);
            break;
    }

    // Cleanup - only reached if no error occurred
    free(error_msg);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}