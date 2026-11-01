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
#include <png.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Allocate buffers for save operations
    png_byte save_buf_32[4];
    png_byte save_buf_16[2];
    
    // Extract values from input data
    png_int_32 int_32_val = 0;
    png_uint_32 uint_32_val = 0;
    unsigned int uint_16_val = 0;

    // Safely read values from input
    if (Size >= 4) {
        int_32_val = static_cast<png_int_32>(
            static_cast<uint32_t>(Data[0]) |
            (static_cast<uint32_t>(Data[1]) << 8) |
            (static_cast<uint32_t>(Data[2]) << 16) |
            (static_cast<uint32_t>(Data[3]) << 24)
        );
        uint_32_val = static_cast<png_uint_32>(
            static_cast<uint32_t>(Data[0]) |
            (static_cast<uint32_t>(Data[1]) << 8) |
            (static_cast<uint32_t>(Data[2]) << 16) |
            (static_cast<uint32_t>(Data[3]) << 24)
        );
    }
    
    if (Size >= 2) {
        uint_16_val = static_cast<unsigned int>(
            static_cast<uint16_t>(Data[0]) |
            (static_cast<uint16_t>(Data[1]) << 8)
        );
    }

    // Test png_save_int_32
    png_save_int_32(save_buf_32, int_32_val);

    // Test png_save_uint_16
    png_save_uint_16(save_buf_16, uint_16_val);

    // Test png_save_uint_32
    png_save_uint_32(save_buf_32, uint_32_val);

    // Test png_get_int_32
    png_int_32 retrieved_int_32 = png_get_int_32(save_buf_32);
    (void)retrieved_int_32; // Suppress unused variable warning

    // Test png_get_uint_32
    png_uint_32 retrieved_uint_32 = png_get_uint_32(save_buf_32);
    (void)retrieved_uint_32; // Suppress unused variable warning

    // For png_get_uint_31, we need a valid png_struct
    // Since we cannot easily create a valid png_struct without complex setup,
    // we will skip this function or use a dummy pointer if the API allows.
    // However, png_get_uint_31 requires a valid png_ptr to call png_error.
    // To avoid crashes, we will not call png_get_uint_31 with a NULL or invalid pointer.
    // Instead, we can create a minimal png_struct if needed, but for fuzzing safety,
    // we'll skip this specific call unless we can safely initialize png_struct.
    // Given the constraints, we will not call png_get_uint_31 to prevent potential crashes.

    return 0;
}