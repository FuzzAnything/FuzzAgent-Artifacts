// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
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
#include "code/png.h"
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data to work with
    if (Size == 0) {
        return 0;
    }

    // Test png_get_uint_16 - requires at least 2 bytes
    if (Size >= 2) {
        png_uint_16 result16 = png_get_uint_16(Data);
        (void)result16; // Use result to avoid compiler warnings
    }

    // Test png_get_uint_32 - requires at least 4 bytes
    if (Size >= 4) {
        png_uint_32 result32 = png_get_uint_32(Data);
        (void)result32;
    }

    // Test png_get_int_32 - requires at least 4 bytes
    if (Size >= 4) {
        png_int_32 result_int32 = png_get_int_32(Data);
        (void)result_int32;
    }

    // Test png_save_uint_16 - needs a buffer and value
    static png_byte buffer16[2];
    png_save_uint_16(buffer16, static_cast<unsigned int>(Size & 0xFFFF));

    // Test png_save_int_32 - needs a buffer and value
    static png_byte buffer32_int[4];
    // Use part of the input data to generate a signed integer value
    png_int_32 int32_val = 0;
    if (Size >= 4) {
        // Create a signed integer from first 4 bytes
        int32_val = static_cast<png_int_32>(Data[0] << 24 | Data[1] << 16 | Data[2] << 8 | Data[3]);
    }
    png_save_int_32(buffer32_int, int32_val);

    // Test png_save_uint_32 - needs a buffer and value
    static png_byte buffer32_uint[4];
    png_uint_32 uint32_val = static_cast<png_uint_32>(Size);
    png_save_uint_32(buffer32_uint, uint32_val);

    // Test round-trip conversions for consistency
    if (Size >= 4) {
        // Save then get uint32
        png_byte roundtrip_buffer[4];
        png_save_uint_32(roundtrip_buffer, uint32_val);
        png_uint_32 roundtrip_result = png_get_uint_32(roundtrip_buffer);
        (void)roundtrip_result;
        
        // Save then get int32
        png_save_int_32(roundtrip_buffer, int32_val);
        png_int_32 roundtrip_int_result = png_get_int_32(roundtrip_buffer);
        (void)roundtrip_int_result;
    }

    // Test with different offsets in the input data
    if (Size >= 8) {
        // Test at offset 4
        const png_byte* offset_data = Data + 4;
        if (Size >= 6) {  // Need at least 2 more bytes from offset
            png_uint_16 offset_result = png_get_uint_16(offset_data);
            (void)offset_result;
        }
    }

    // Test with maximum and minimum values
    png_save_uint_16(buffer16, 0xFFFF);
    png_save_uint_16(buffer16, 0);
    
    png_save_int_32(buffer32_int, 0x7FFFFFFF);  // Max positive
    png_save_int_32(buffer32_int, -0x7FFFFFFF); // Large negative
    png_save_int_32(buffer32_int, 0);
    
    png_save_uint_32(buffer32_uint, 0xFFFFFFFF);
    png_save_uint_32(buffer32_uint, 0);

    return 0;
}