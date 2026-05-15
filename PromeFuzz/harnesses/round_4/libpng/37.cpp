// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_image_write_to_file at pngwrite.c:2402:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_set_crc_action at pngrtran.c:24:1 in png.h
// png_benign_error at pngerror.c:308:1 in png.h
// png_chunk_warning at pngerror.c:436:1 in png.h
// png_error at pngerror.c:50:1 in png.h
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
#include <csetjmp>
#include <cstdio>

static jmp_buf error_jmp_buf;

static void custom_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Use longjmp to return control instead of aborting
    longjmp(error_jmp_buf, 1);
}

static void custom_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler - do nothing to avoid abort
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    // Initialize png_struct for reading
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    // Set custom error handler to avoid abort()
    png_set_error_fn(png_ptr, nullptr, custom_error_handler, custom_warning_handler);
    
    // Set up error recovery with setjmp/longjmp
    if (setjmp(error_jmp_buf)) {
        // Error occurred - clean up and return
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Extract parameters from fuzzer input
    int allowed = (Size > 0) ? (Data[0] & 1) : 0;
    int crit_action = (Size > 0) ? ((Data[0] >> 1) & 3) : 0;
    int ancil_action = (Size > 0) ? ((Data[0] >> 3) & 3) : 0;
    
    // 1. Test png_set_benign_errors
    png_set_benign_errors(png_ptr, allowed);
    
    // 2. Test png_set_crc_action
    png_set_crc_action(png_ptr, crit_action, ancil_action);
    
    // 3. Test png_benign_error
    if (Size > 1) {
        // Create null-terminated message from input data
        size_t msg_len = (Size - 1 < 100) ? Size - 1 : 100;
        char* benign_msg = new char[msg_len + 1];
        memcpy(benign_msg, Data + 1, msg_len);
        benign_msg[msg_len] = '\0';
        png_benign_error(png_ptr, benign_msg);
        delete[] benign_msg;
    }
    
    // 4. Test png_chunk_warning
    if (Size > 2) {
        size_t msg_len = (Size - 2 < 100) ? Size - 2 : 100;
        char* chunk_msg = new char[msg_len + 1];
        memcpy(chunk_msg, Data + 2, msg_len);
        chunk_msg[msg_len] = '\0';
        png_chunk_warning(png_ptr, chunk_msg);
        delete[] chunk_msg;
    }
    
    // 5. Test png_error (will trigger longjmp to error handler)
    if (Size > 3) {
        size_t msg_len = (Size - 3 < 100) ? Size - 3 : 100;
        char* error_msg = new char[msg_len + 1];
        memcpy(error_msg, Data + 3, msg_len);
        error_msg[msg_len] = '\0';
        png_error(png_ptr, error_msg);
        delete[] error_msg;
    }
    
    // 6. Test png_image_write_to_file
    // Create a minimal png_image structure
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = 1;
    image.height = 1;
    image.format = PNG_FORMAT_RGBA;
    
    // Prepare buffer for 1x1 RGBA image
    png_byte buffer[4] = {0, 0, 0, 255};
    
    // Write to dummy file
    const char* filename = "./dummy_file";
    int convert_to_8bit = (Size > 0) ? (Data[0] & 1) : 0;
    png_int_32 row_stride = 4; // For 1 pixel RGBA
    
    png_image_write_to_file(&image, filename, convert_to_8bit, buffer, row_stride, nullptr);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    
    return 0;
}