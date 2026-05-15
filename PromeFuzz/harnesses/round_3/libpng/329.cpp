// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_chunk_error at pngerror.c:418:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_longjmp at pngerror.c:663:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
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
#include "code/png.h"
#include <cstdint>
#include <cstring>
#include <csetjmp>

// Static callback functions
static void error_callback(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - just return
}

static void warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - just return
}

static void longjmp_callback(struct __jmp_buf_tag* jmp_buf_ptr, int val) {
    if (jmp_buf_ptr) {
        longjmp(jmp_buf_ptr, val);
    }
}

extern "C" int LLVMFuzzerTestOneInput_329(const uint8_t *Data, size_t Size) {
    // Create a dummy file if needed
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Initialize libpng structures
    const char* version = PNG_LIBPNG_VER_STRING;
    png_structp png_ptr = png_create_read_struct_2(
        version,
        nullptr,  // error_ptr
        error_callback,
        warning_callback,
        nullptr,  // mem_ptr
        nullptr,  // malloc_fn
        nullptr   // free_fn
    );
    
    if (!png_ptr) {
        return 0;
    }
    
    // Set up error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred - clean up and return
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Use fuzz data to determine which functions to call
    if (Size > 0) {
        uint8_t selector = Data[0];
        
        switch (selector % 6) {
            case 0:
                // Test png_set_error_fn
                png_set_error_fn(png_ptr, (void*)Data, error_callback, warning_callback);
                break;
                
            case 1:
                // Test png_set_benign_errors
                png_set_benign_errors(png_ptr, (Size > 1) ? (Data[1] & 1) : 0);
                break;
                
            case 2:
                // Test png_longjmp - this will jump to setjmp above
                if (Size > 1) {
                    png_longjmp(png_ptr, Data[1] % 10);
                }
                break;
                
            case 3:
                // Test png_create_read_struct_2 again with different parameters
                {
                    png_structp png_ptr2 = png_create_read_struct_2(
                        version,
                        (void*)Data,
                        (Size > 10) ? error_callback : nullptr,
                        (Size > 20) ? warning_callback : nullptr,
                        nullptr,
                        nullptr,
                        nullptr
                    );
                    if (png_ptr2) {
                        png_destroy_read_struct(&png_ptr2, nullptr, nullptr);
                    }
                }
                break;
                
            case 4:
                // Test png_set_longjmp_fn
                {
                    jmp_buf* jb = png_set_longjmp_fn(png_ptr, longjmp_callback, sizeof(jmp_buf));
                    (void)jb;  // Suppress unused variable warning
                }
                break;
                
            case 5:
                // Test png_chunk_error - this will jump to setjmp above
                if (Size > 1) {
                    char error_msg[256];
                    size_t msg_len = (Size - 1) < 255 ? (Size - 1) : 255;
                    memcpy(error_msg, Data + 1, msg_len);
                    error_msg[msg_len] = '\0';
                    png_chunk_error(png_ptr, error_msg);
                }
                break;
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    
    return 0;
}