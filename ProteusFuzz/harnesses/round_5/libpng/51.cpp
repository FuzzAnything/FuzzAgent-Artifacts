// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
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
#include <cstdint>
#include <cstring>
#include <cstdlib>

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Allocate buffers for operations
    png_byte buf_in[4];
    png_byte buf_out[4];
    png_byte buf_out16[2];

    // Initialize input buffer with fuzz data (at least 4 bytes)
    std::memcpy(buf_in, Data, 4);

    // Test png_get_uint_32
    png_uint_32 val_uint32 = png_get_uint_32(buf_in);
    (void)val_uint32; // Suppress unused variable warning

    // Test png_save_uint_32
    png_save_uint_32(buf_out, val_uint32);

    // Test png_get_int_32
    png_int_32 val_int32 = png_get_int_32(buf_in);
    (void)val_int32;

    // Test png_save_int_32
    png_save_int_32(buf_out, val_int32);

    // Test png_save_uint_16
    // Use first 2 bytes of input as 16-bit value
    unsigned int val_uint16 = (buf_in[0] << 8) | buf_in[1];
    png_save_uint_16(buf_out16, val_uint16);

    // Test png_get_uint_31 requires a valid png_struct
    // We cannot easily create a valid png_struct without initializing the library fully,
    // and png_get_uint_31 calls png_error on overflow which terminates the process.
    // To avoid crashing the fuzzer, we skip direct invocation of png_get_uint_31
    // unless we can mock the error handling, which is complex and risky in a fuzzer.
    // Instead, we rely on the other functions to exercise the core logic.

    return 0;
}