// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
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
#include "png.h"
#include <cstdint>
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for all operations
    if (Size < 12) {
        return 0;
    }

    // Test png_get_uint_16 - requires at least 2 bytes
    png_uint_16 uint16_result = png_get_uint_16(Data);
    
    // Test png_get_uint_32 - requires at least 4 bytes
    png_uint_32 uint32_result = png_get_uint_32(Data + 2);
    
    // Test png_get_int_32 - requires at least 4 bytes
    png_int_32 int32_result = png_get_int_32(Data + 6);
    
    // Test png_save_uint_16 - requires 2-byte buffer
    png_byte buf16[2];
    png_save_uint_16(buf16, static_cast<unsigned int>(uint16_result));
    
    // Test png_save_int_32 - requires 4-byte buffer
    png_byte buf32_int[4];
    png_save_int_32(buf32_int, int32_result);
    
    // Test png_save_uint_32 - requires 4-byte buffer
    png_byte buf32_uint[4];
    png_save_uint_32(buf32_uint, uint32_result);
    
    // Additional tests with different values to explore more states
    if (Size >= 16) {
        // Test with different offsets
        png_uint_16 uint16_result2 = png_get_uint_16(Data + 10);
        png_byte buf16_2[2];
        png_save_uint_16(buf16_2, static_cast<unsigned int>(uint16_result2));
        
        // Test with maximum values
        png_save_uint_16(buf16, 65535);
        png_save_int_32(buf32_int, 2147483647);
        png_save_int_32(buf32_int, -2147483648);
        png_save_uint_32(buf32_uint, 4294967295U);
    }
    
    return 0;
}