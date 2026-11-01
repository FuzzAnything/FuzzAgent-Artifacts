// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <setjmp.h>
#include <png.h>

// Global jump buffer to handle libpng errors without crashing
static jmp_buf jump_buffer;

static void safe_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(jump_buffer, 1);
}

static void safe_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize buffers for save operations
    png_byte buf32[4];
    png_byte buf16[2];
    
    // Initialize PNG struct for functions requiring it
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Create PNG struct with error handlers to prevent crashes
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, safe_png_error, safe_png_warning);
    if (png_ptr == nullptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Ensure we have enough data for various operations
    size_t offset = 0;

    // Test png_save_int_32
    if (Size >= offset + 4) {
        png_int_32 val32 = 0;
        memcpy(&val32, Data + offset, 4);
        offset += 4;
        png_save_int_32(buf32, val32);
    }

    // Test png_save_uint_16
    if (Size >= offset + 2) {
        unsigned int val16 = 0;
        memcpy(&val16, Data + offset, 2);
        offset += 2;
        png_save_uint_16(buf16, val16);
    }

    // Test png_get_int_32
    if (Size >= offset + 4) {
        const png_byte *readBuf32 = Data + offset;
        offset += 4;
        png_int_32 result32 = png_get_int_32(readBuf32);
        (void)result32; // Use result to avoid unused variable warning
    }

    // Test png_get_uint_31
    // This function may trigger png_error if value > PNG_UINT_31_MAX
    // We use setjmp/longjmp to catch the error
    if (Size >= offset + 4) {
        const png_byte *readBuf32 = Data + offset;
        offset += 4;
        
        if (setjmp(jump_buffer) == 0) {
            // First time, try the call
            png_uint_32 result31 = png_get_uint_31(png_ptr, readBuf32);
            (void)result31;
        } else {
            // Error occurred, jump here, continue safely
        }
    }

    // Test png_get_uint_32
    if (Size >= offset + 4) {
        const png_byte *readBuf32 = Data + offset;
        offset += 4;
        png_uint_32 result32 = png_get_uint_32(readBuf32);
        (void)result32;
    }

    // Test png_save_uint_32
    if (Size >= offset + 4) {
        png_uint_32 val32u = 0;
        memcpy(&val32u, Data + offset, 4);
        offset += 4;
        png_save_uint_32(buf32, val32u);
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}