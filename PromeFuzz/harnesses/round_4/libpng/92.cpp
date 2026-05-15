// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
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
#include <cstring>

#include "png.h"

extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for all operations
    if (Size < 8) {
        return 0;
    }

    // Test png_get_uint_16 - requires at least 2 bytes
    if (Size >= 2) {
        png_uint_16 result_16 = png_get_uint_16(Data);
        (void)result_16; // Use result to avoid unused variable warning
    }

    // Test png_get_uint_32 - requires at least 4 bytes
    if (Size >= 4) {
        png_uint_32 result_32 = png_get_uint_32(Data);
        (void)result_32;
    }

    // Test png_get_int_32 - requires at least 4 bytes
    if (Size >= 4) {
        png_int_32 result_int32 = png_get_int_32(Data);
        (void)result_int32;
    }

    // Test save functions - need buffers to write to
    static png_byte buffer_16[2];
    static png_byte buffer_32[4];

    // Test png_save_uint_16
    png_save_uint_16(buffer_16, static_cast<unsigned int>(Data[0] << 8 | Data[1]));

    // Test png_save_int_32
    png_int_32 int32_value = static_cast<png_int_32>(
        (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]
    );
    png_save_int_32(buffer_32, int32_value);

    // Test png_save_uint_32
    png_uint_32 uint32_value = static_cast<png_uint_32>(
        (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7]
    );
    png_save_uint_32(buffer_32, uint32_value);

    // Additional test: Chain operations to explore more states
    if (Size >= 6) {
        // Save a value then read it back
        png_byte temp_buffer[4];
        png_uint_32 original_value = static_cast<png_uint_32>(
            (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]
        );
        png_save_uint_32(temp_buffer, original_value);
        png_uint_32 readback_value = png_get_uint_32(temp_buffer);
        (void)readback_value;

        // Test with different byte patterns
        png_byte pattern_buffer[4] = {0xFF, 0x00, 0xAA, 0x55};
        png_uint_32 pattern_result = png_get_uint_32(pattern_buffer);
        (void)pattern_result;
    }

    return 0;
}