// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
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

// Fuzz driver for libpng integer conversion functions
extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Allocate buffers for testing
    png_byte buf[4];
    png_byte buf2[2];
    png_int_32 int32_val;
    png_uint_32 uint32_val;
    unsigned int uint16_val;

    // Initialize with fuzzed data
    std::memcpy(buf, Data, 4);
    std::memcpy(buf2, Data, 2);

    // Test png_get_int_32
    int32_val = png_get_int_32(buf);
    (void)int32_val; // Use the value to prevent unused variable warnings

    // Test png_get_uint_32
    uint32_val = png_get_uint_32(buf);
    (void)uint32_val;

    // Test png_save_int_32
    png_save_int_32(buf, int32_val);

    // Test png_save_uint_32
    png_save_uint_32(buf, uint32_val);

    // Test png_save_uint_16
    uint16_val = (Size > 2) ? (Data[0] << 8) | Data[1] : 0;
    png_save_uint_16(buf2, uint16_val);

    // Test png_get_uint_31
    // We need a valid png_struct pointer for this function
    // Since we cannot easily create a valid png_struct without more context,
    // we will skip this test or use a dummy pointer if the function allows it.
    // However, png_get_uint_31 typically requires a valid png_struct to call png_error.
    // For fuzzing purposes, we can try to pass a null pointer or a dummy pointer,
    // but this might cause a crash if the function dereferences it.
    // Given the constraints, we will skip this test to avoid potential crashes.
    // If we were to test it, we would need to initialize a png_struct properly.
    // For now, we'll just ensure the buffer is valid and skip the call.
    // png_uint_32 result = png_get_uint_31(nullptr, buf); // This might crash
    // (void)result;

    return 0;
}