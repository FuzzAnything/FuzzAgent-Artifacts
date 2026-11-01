// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
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

// Dummy error handler to prevent actual aborts during fuzzing
static void dummy_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // In a real scenario, this would log the error.
    // For fuzzing, we just return to avoid terminating the process.
}

// Dummy warning handler
static void dummy_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Custom longjmp handler to catch png_error calls
static jmp_buf fuzz_jmp_buf;

static void fuzz_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(fuzz_jmp_buf, 1);
}

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Initialize buffers for save operations
    png_byte save_buf_32[4];
    png_byte save_buf_16[2];
    
    // Extract values from input data for testing
    png_uint_32 test_uint_32 = 0;
    png_uint_16 test_uint_16 = 0;
    png_int_32 test_int_32 = 0;

    // Read 4 bytes for uint32 test
    if (Size >= 4) {
        memcpy(&test_uint_32, Data, 4);
    }
    
    // Read 2 bytes for uint16 test (if available)
    if (Size >= 6) {
        memcpy(&test_uint_16, Data + 4, 2);
    }

    // Read 4 bytes for int32 test (if available)
    if (Size >= 8) {
        memcpy(&test_int_32, Data + 4, 4);
    }

    // Test png_get_uint_32 (No png_ptr required)
    // Requires at least 4 bytes
    if (Size >= 4) {
        png_uint_32 val32 = png_get_uint_32(Data);
        (void)val32; // Use value to suppress unused variable warning
    }

    // Test png_save_uint_32
    png_save_uint_32(save_buf_32, test_uint_32);

    // Test png_save_uint_16
    png_save_uint_16(save_buf_16, test_uint_16);

    // Test png_get_int_32
    if (Size >= 4) {
        png_int_32 val_int32 = png_get_int_32(Data);
        (void)val_int32;
    }

    // Test png_save_int_32
    png_save_int_32(save_buf_32, test_int_32);

    // Setup for png_get_uint_31 which requires a png_struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        return 0;
    }

    // Set custom error handler that uses longjmp
    png_set_error_fn(png_ptr, NULL, fuzz_png_error, dummy_png_warning);

    // Test png_get_uint_31
    // This function checks if value > PNG_UINT_31_MAX (2^31 - 1)
    // We need at least 4 bytes for the buffer
    if (Size >= 4) {
        // Setup longjmp to catch errors
        if (setjmp(fuzz_jmp_buf) == 0) {
            // Try with the first 4 bytes
            png_uint_32 val31 = png_get_uint_31(png_ptr, Data);
            (void)val31;
        } else {
            // Error was triggered, longjmp caught it
            // Continue execution
        }
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, NULL, NULL);

    return 0;
}