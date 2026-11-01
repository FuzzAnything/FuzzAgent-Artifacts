// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
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

// Global jump buffer to handle png_error calls
static jmp_buf jump_buffer;

// Custom error function that triggers longjmp instead of aborting
static void dummy_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Trigger the jump to catch the error in the fuzzer
    longjmp(jump_buffer, 1);
}

// Custom warning function (optional, but good practice)
static void dummy_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare buffers for save functions
    png_byte buf_save_32[4];
    png_byte buf_save_16[2];
    
    // Prepare buffers for get functions (copy from input)
    png_byte buf_get_32[4];

    // Extract values from fuzz input safely
    png_int_32 val_int_32 = 0;
    png_uint_32 val_uint_32 = 0;
    unsigned int val_uint_16 = 0;

    // Safely read input data into our variables
    if (Size >= 4) {
        memcpy(buf_get_32, Data, 4);
        memcpy(buf_save_32, Data, 4);
        
        // Use memcpy to avoid const-cast issues and alignment issues
        memcpy(&val_int_32, Data, sizeof(val_int_32));
        memcpy(&val_uint_32, Data, sizeof(val_uint_32));
    }
    
    if (Size >= 2) {
        memcpy(buf_save_16, Data, 2);
        memcpy(&val_uint_16, Data, sizeof(val_uint_16));
    }

    // 1. Test png_save_int_32
    png_save_int_32(buf_save_32, val_int_32);

    // 2. Test png_save_uint_16
    png_save_uint_16(buf_save_16, val_uint_16);

    // 3. Test png_get_int_32
    // Ensure we have 4 bytes
    if (Size >= 4) {
        png_int_32 result_int = png_get_int_32(buf_get_32);
        (void)result_int; // Use result to avoid unused variable warning
    }

    // 4. Test png_get_uint_31
    // This requires a valid png_struct. We must create one.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr) {
        // Set up error handling to prevent crash
        png_set_error_fn(png_ptr, NULL, dummy_png_error, dummy_png_warning);
        
        if (Size >= 4) {
            // Use setjmp to catch the longjmp from png_error
            if (setjmp(jump_buffer) == 0) {
                // Call the function inside the try block
                png_uint_32 result_uint_31 = png_get_uint_31(png_ptr, buf_get_32);
                (void)result_uint_31;
            } else {
                // Error was triggered, we caught it here.
                // Continue execution safely.
            }
        }
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }

    // 5. Test png_get_uint_32
    if (Size >= 4) {
        png_uint_32 result_uint = png_get_uint_32(buf_get_32);
        (void)result_uint;
    }

    // 6. Test png_save_uint_32
    png_save_uint_32(buf_save_32, val_uint_32);

    // Cleanup: No dynamic memory allocated by us that needs manual free, 
    // png_struct was destroyed.
    
    return 0;
}