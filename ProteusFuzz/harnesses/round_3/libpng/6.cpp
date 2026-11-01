// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
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
#include <png.h>
#include <setjmp.h>

// Global jump buffer for handling libpng errors
static jmp_buf fuzzer_jmp_buf;

// Custom error handler to jump back to fuzzer instead of aborting
static void custom_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(fuzzer_jmp_buf, 1);
}

// Custom warning handler
static void custom_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    return;
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare buffers for testing
    png_byte buf16[2];
    png_byte buf32[4];
    png_uint_32 val32;
    png_int_32 val32_signed;
    png_uint_32 val31;

    // Initialize buffers with input data
    std::memcpy(buf16, Data, std::min(Size, (size_t)2));
    std::memcpy(buf32, Data, std::min(Size, (size_t)4));

    // Test png_save_uint_16
    unsigned int input_val_16 = 0;
    if (Size >= 2) {
        input_val_16 = (Data[0] << 8) | Data[1];
    } else {
        input_val_16 = Data[0];
    }
    png_save_uint_16(buf16, input_val_16);

    // Test png_save_int_32
    png_int_32 input_val_int32 = 0;
    if (Size >= 4) {
        input_val_int32 = (png_int_32)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
    } else if (Size >= 1) {
        input_val_int32 = (png_int_32)Data[0];
    }
    png_save_int_32(buf32, input_val_int32);

    // Test png_save_uint_32
    png_uint_32 input_val_uint32 = 0;
    if (Size >= 4) {
        input_val_uint32 = (png_uint_32)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
    } else if (Size >= 1) {
        input_val_uint32 = (png_uint_32)Data[0];
    }
    png_save_uint_32(buf32, input_val_uint32);

    // Test png_get_uint_32
    if (Size >= 4) {
        val32 = png_get_uint_32(buf32);
    }

    // Test png_get_int_32
    if (Size >= 4) {
        val32_signed = png_get_int_32(buf32);
    }

    // Test png_get_uint_31
    // This function may call png_error if value > PNG_UINT_31_MAX
    // We set up a custom error handler to catch this
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, custom_error_handler, custom_warning_handler);
    if (png_ptr != NULL) {
        // Set the error handler for this struct
        png_set_error_fn(png_ptr, NULL, custom_error_handler, custom_warning_handler);

        if (Size >= 4) {
            // Use setjmp to catch the longjmp from png_error
            if (setjmp(fuzzer_jmp_buf) == 0) {
                // Try to read the value; if it's out of range, png_error will be called
                val31 = png_get_uint_31(png_ptr, buf32);
            } else {
                // We jumped here due to an error, just continue
                val31 = 0;
            }
        }
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }

    return 0;
}