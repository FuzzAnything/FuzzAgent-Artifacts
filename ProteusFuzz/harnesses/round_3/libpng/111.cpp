// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
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

// Custom error handler that does not abort or longjmp
static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Silently ignore errors to prevent fuzzer crash
}

// Custom warning handler
static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Silently ignore warnings
}

extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare buffers for save operations
    png_byte buf16[2];
    png_byte buf32[4];

    // Extract values from fuzz input
    unsigned int val16 = 0;
    png_uint_32 val32 = 0;
    png_int_32 valSigned32 = 0;

    // Safely extract 16-bit value
    if (Size >= 2) {
        val16 = (Data[0] << 8) | Data[1];
    }

    // Safely extract 32-bit unsigned value
    if (Size >= 4) {
        val32 = ((png_uint_32)Data[0] << 24) | 
                ((png_uint_32)Data[1] << 16) | 
                ((png_uint_32)Data[2] << 8) | 
                ((png_uint_32)Data[3]);
    }

    // Safely extract 32-bit signed value
    if (Size >= 4) {
        valSigned32 = ((png_int_32)Data[0] << 24) | 
                      ((png_int_32)Data[1] << 16) | 
                      ((png_int_32)Data[2] << 8) | 
                      ((png_int_32)Data[3]);
    }

    // Test png_save_uint_16
    png_save_uint_16(buf16, val16);

    // Test png_save_uint_32
    png_save_uint_32(buf32, val32);

    // Test png_save_int_32
    png_save_int_32(buf32, valSigned32);

    // Test png_get_uint_32
    png_uint_32 read32 = png_get_uint_32(buf32);
    (void)read32; // Suppress unused variable warning

    // Test png_get_int_32
    png_int_32 readSigned32 = png_get_int_32(buf32);
    (void)readSigned32; // Suppress unused variable warning

    // Test png_get_uint_31 requires a valid png_ptr
    // We create a read struct and set our custom error handler to prevent crashes
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error, dummy_warning);
    if (png_ptr != NULL) {
        // We need to set the error handler explicitly if png_create_read_struct didn't fully initialize it
        // However, png_create_read_struct with our callbacks should be enough.
        // To be extra safe against longjmp, we can wrap the call in a try-catch if using C++, 
        // but libpng uses setjmp/longjmp. The custom error handler returning void is the key.
        
        // Test png_get_uint_31
        // If the value in buf32 is > PNG_UINT_31_MAX, png_get_uint_31 will call png_error.
        // Our dummy_error function will catch it and prevent the crash.
        png_uint_32 read31 = png_get_uint_31(png_ptr, buf32);
        (void)read31;
        
        // Cleanup
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }

    return 0;
}