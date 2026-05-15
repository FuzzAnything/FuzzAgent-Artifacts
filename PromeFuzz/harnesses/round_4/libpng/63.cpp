// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_chunk_error at pngerror.c:418:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_longjmp at pngerror.c:663:1 in png.h
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
#include "png.h"
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static jmp_buf jump_buffer;

static void custom_error(png_structp png_ptr, png_const_charp error_msg) {
    // Instead of aborting, longjmp back to the setjmp point
    longjmp(jump_buffer, 1);
}

static void custom_warning(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing for warnings in fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Set custom error handlers that don't abort
    png_set_error_fn(png_ptr, NULL, custom_error, custom_warning);
    
    // Set up setjmp for error recovery
    if (setjmp(jump_buffer)) {
        // Error occurred, clean up and return
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Prepare error message from fuzzer input
    // Ensure null termination for safety
    char* error_msg = (char*)malloc(Size + 1);
    if (!error_msg) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    memcpy(error_msg, Data, Size);
    error_msg[Size] = '\0';
    
    // Use first byte to select which function to test
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0:
            png_chunk_error(png_ptr, error_msg);
            break;
        case 1:
            png_error(png_ptr, error_msg);
            break;
        case 2:
            png_longjmp(png_ptr, Data[0] % 10);
            break;
        case 3:
            png_benign_error(png_ptr, error_msg);
            break;
        case 4:
            png_chunk_warning(png_ptr, error_msg);
            break;
        case 5:
            png_chunk_benign_error(png_ptr, error_msg);
            break;
    }
    
    // Clean up
    free(error_msg);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}