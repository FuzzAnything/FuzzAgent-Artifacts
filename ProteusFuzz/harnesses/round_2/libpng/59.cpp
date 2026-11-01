// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
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

// Custom error handler that jumps to a safe location instead of aborting
static jmp_buf jump_buffer;

static void handle_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(jump_buffer, 1);
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare buffers for saving functions
    png_byte save_buf_32[4];
    png_byte save_buf_16[2];

    // Initialize PNG struct for png_get_uint_31
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }

    // Set custom error handler to prevent abort
    png_set_error_fn(png_ptr, nullptr, handle_png_error, nullptr);

    // Test png_get_uint_32 (requires 4 bytes)
    if (Size >= 4) {
        png_uint_32 val = png_get_uint_32(Data);
        (void)val; // Use value to avoid unused warning
    }

    // Test png_get_int_32 (requires 4 bytes)
    if (Size >= 4) {
        png_int_32 val = png_get_int_32(Data);
        (void)val;
    }

    // Test png_save_uint_32
    png_uint_32 test_val_32 = 0;
    if (Size >= 4) {
        test_val_32 = png_get_uint_32(Data);
    } else if (Size > 0) {
        test_val_32 = Data[0];
    }
    png_save_uint_32(save_buf_32, test_val_32);

    // Test png_save_uint_16
    unsigned int test_val_16 = 0;
    if (Size >= 2) {
        test_val_16 = (unsigned int)Data[0] | ((unsigned int)Data[1] << 8);
    } else if (Size > 0) {
        test_val_16 = Data[0];
    }
    png_save_uint_16(save_buf_16, test_val_16);

    // Test png_save_int_32
    png_int_32 test_val_int_32 = 0;
    if (Size >= 4) {
        test_val_int_32 = png_get_int_32(Data);
    } else if (Size > 0) {
        test_val_int_32 = (png_int_32)Data[0];
    }
    png_save_int_32(save_buf_32, test_val_int_32);

    // Test png_get_uint_31 (requires png_ptr and 4 bytes)
    if (Size >= 4) {
        // Use setjmp to catch the longjmp from png_error
        if (setjmp(jump_buffer) == 0) {
            // This function may trigger error if value > PNG_UINT_31_MAX,
            // but our custom error handler will longjmp here.
            png_uint_32 val_31 = png_get_uint_31(png_ptr, Data);
            (void)val_31;
        }
        // If we are here, either the function succeeded or we jumped back
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    return 0;
}