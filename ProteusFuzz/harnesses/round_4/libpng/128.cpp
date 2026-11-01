// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
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
#include <cstddef>
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Allocate buffers for saving operations
    png_byte save_buf_32[4];
    png_byte save_buf_16[2];
    
    // Extract values from fuzz input for testing
    png_int_32 int_val = static_cast<png_int_32>(Data[0]) | 
                         (static_cast<png_int_32>(Data[1]) << 8) | 
                         (static_cast<png_int_32>(Data[2]) << 16) | 
                         (static_cast<png_int_32>(Data[3]) << 24);
    
    unsigned int uint16_val = (Size > 4) ? (Data[4] | (Data[5] << 8)) : 0;
    png_uint_32 uint32_val = (Size > 8) ? (static_cast<png_uint_32>(Data[6]) | 
                                           (static_cast<png_uint_32>(Data[7]) << 8) | 
                                           (static_cast<png_uint_32>(Data[8]) << 16) | 
                                           (static_cast<png_uint_32>(Data[9]) << 24)) : 0;

    // Test png_save_int_32
    png_save_int_32(save_buf_32, int_val);

    // Test png_save_uint_16
    png_save_uint_16(save_buf_16, uint16_val);

    // Test png_save_uint_32
    png_save_uint_32(save_buf_32, uint32_val);

    // Test png_get_int_32
    png_int_32 retrieved_int = png_get_int_32(save_buf_32);
    (void)retrieved_int; // Suppress unused variable warning

    // Test png_get_uint_32
    png_uint_32 retrieved_uint = png_get_uint_32(save_buf_32);
    (void)retrieved_uint; // Suppress unused variable warning

    // Test png_get_uint_31 requires a valid png_struct
    // We cannot easily create a valid png_struct without complex initialization
    // and error handling callbacks that might crash during fuzzing.
    // However, we can attempt to call it with a null pointer if the library
    // handles it gracefully, or skip it to avoid crashes.
    // Given the requirement to explore states, we will skip this specific call
    // unless we can safely initialize a png_struct, which is complex.
    // For safety in fuzzing, we omit png_get_uint_31 as it requires a valid
    // png_ptr which is non-trivial to set up for a simple fuzzer without
    // risking immediate crashes from missing error handlers.

    return 0;
}