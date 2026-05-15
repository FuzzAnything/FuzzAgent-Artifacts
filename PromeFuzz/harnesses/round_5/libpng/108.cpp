// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_error at pngerror.c:50:1 in png.h
// png_longjmp at pngerror.c:663:1 in png.h
// png_benign_error at pngerror.c:308:1 in png.h
// png_chunk_error at pngerror.c:418:1 in png.h
// png_chunk_warning at pngerror.c:436:1 in png.h
// png_chunk_benign_error at pngerror.c:453:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <csetjmp>
#include <cstdio>

#include "code/png.h"

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Custom error handler that uses longjmp instead of abort
    jmp_buf* buf = (jmp_buf*)png_get_error_ptr(png_ptr);
    if (buf) {
        longjmp(*buf, 1);
    }
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Custom warning handler - does nothing
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    static FILE* dummy_file = nullptr;
    
    // Create or reopen dummy file
    if (!dummy_file) {
        dummy_file = fopen("./dummy_file", "wb");
        if (!dummy_file) {
            return 0;
        }
    } else {
        freopen(nullptr, "wb", dummy_file);
    }
    
    // Write fuzzer data to dummy file
    if (Size > 0) {
        fwrite(Data, 1, Size, dummy_file);
    }
    fflush(dummy_file);
    rewind(dummy_file);
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(dummy_file);
        return 0;
    }
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(dummy_file);
        return 0;
    }
    
    // Set up error handling with setjmp/longjmp
    jmp_buf jmpbuf;
    png_set_error_fn(png_ptr, &jmpbuf, error_handler, warning_handler);
    
    if (setjmp(jmpbuf)) {
        // Error occurred - clean up and return
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(dummy_file);
        return 0;
    }
    
    // Initialize PNG reading
    png_init_io(png_ptr, dummy_file);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Prepare error message from fuzzer data
    static char error_msg[256];
    if (Size > 0) {
        size_t msg_len = Size < 255 ? Size : 255;
        memcpy(error_msg, Data, msg_len);
        error_msg[msg_len] = '\0';
    } else {
        strcpy(error_msg, "Fuzzer generated error");
    }
    
    // Test different error functions based on input
    if (Size > 0) {
        uint8_t selector = Data[0];
        
        switch (selector % 6) {
            case 0:
                png_error(png_ptr, error_msg);
                break;
            case 1:
                png_longjmp(png_ptr, selector % 10);
                break;
            case 2:
                png_benign_error(png_ptr, error_msg);
                break;
            case 3:
                png_chunk_error(png_ptr, error_msg);
                break;
            case 4:
                png_chunk_warning(png_ptr, error_msg);
                break;
            case 5:
                png_chunk_benign_error(png_ptr, error_msg);
                break;
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(dummy_file);
    
    return 0;
}