// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
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
#include <png.h>
#include <setjmp.h>

// Global jump buffer for handling libpng errors
static jmp_buf fuzz_jmp_buf;

static void safe_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(fuzz_jmp_buf, 1);
}

static void safe_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Ignore warnings
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Allocate buffers for read/write operations
    png_byte buf32[4];
    png_byte buf16[2];
    png_uint_32 u32_val;
    png_int_32 i32_val;
    unsigned int u16_val;

    // Initialize buffers with input data
    std::memcpy(buf32, Data, 4);
    std::memcpy(buf16, Data, std::min(Size, (size_t)2));

    // Test png_get_uint_32
    u32_val = png_get_uint_32(buf32);
    (void)u32_val; // Suppress unused variable warning

    // Test png_save_uint_32
    png_save_uint_32(buf32, u32_val);

    // Test png_get_int_32
    i32_val = png_get_int_32(buf32);
    (void)i32_val; // Suppress unused variable warning

    // Test png_save_int_32
    png_save_int_32(buf32, i32_val);

    // Test png_save_uint_16
    u16_val = (unsigned int)(buf32[0] << 8 | buf32[1]);
    png_save_uint_16(buf16, u16_val);

    // Test png_get_uint_31 with a minimal png_struct setup
    // We need to create a png_struct to call png_get_uint_31
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, safe_png_error, safe_png_warning);
    if (png_ptr == NULL) {
        return 0;
    }

    // Set up error handling with setjmp/longjmp
    if (setjmp(fuzz_jmp_buf) == 0) {
        // Ensure we have at least 4 bytes for the buffer
        if (Size >= 4) {
            png_get_uint_31(png_ptr, buf32);
        }
    }
    // If longjmp is triggered, we jump here and continue

    // Cleanup
    png_destroy_read_struct(&png_ptr, NULL, NULL);

    return 0;
}