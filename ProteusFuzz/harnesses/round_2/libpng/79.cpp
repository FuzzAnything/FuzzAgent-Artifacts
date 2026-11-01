// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <setjmp.h>

// Custom error handler to prevent libpng from aborting the process
static jmp_buf jbuf;

static void custom_error_handler(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    // Jump back to the fuzzer instead of aborting
    longjmp(jbuf, 1);
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Initialize buffers for save operations
    png_byte save_buf_32[4];
    png_byte save_buf_16[2];

    // Initialize png_struct for png_get_uint_31
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    // Set up custom error handler and jump buffer
    png_set_error_fn(png_ptr, nullptr, custom_error_handler, nullptr);

    const uint8_t *ptr = Data;
    
    // Test png_get_uint_32 (requires 4 bytes)
    if (Size >= 4) {
        png_uint_32 val32 = png_get_uint_32(ptr);
        (void)val32;
    }

    // Test png_get_int_32 (requires 4 bytes)
    if (Size >= 4) {
        png_int_32 val_int32 = png_get_int_32(ptr);
        (void)val_int32;
    }

    // Test png_get_uint_31 (requires 4 bytes and valid png_ptr)
    // This function may trigger an error if the value > PNG_UINT_31_MAX.
    // We use setjmp/longjmp to catch the error and continue.
    if (Size >= 4) {
        if (setjmp(jbuf) == 0) {
            // First time through, execute the function
            png_uint_32 val31 = png_get_uint_31(png_ptr, ptr);
            (void)val31;
        } else {
            // Longjmp was called due to an error, skip this iteration
            // No need to do anything, just continue
        }
    }

    // Test png_save_uint_32
    if (Size >= 4) {
        png_uint_32 input_val = png_get_uint_32(ptr);
        png_save_uint_32(save_buf_32, input_val);
    }

    // Test png_save_int_32
    if (Size >= 4) {
        png_int_32 input_int = png_get_int_32(ptr);
        png_save_int_32(save_buf_32, input_int);
    }

    // Test png_save_uint_16 (requires 2 bytes)
    if (Size >= 2) {
        unsigned int input_16 = (ptr[0] << 8) | ptr[1];
        png_save_uint_16(save_buf_16, input_16);
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    return 0;
}