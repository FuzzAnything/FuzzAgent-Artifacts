// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
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
#include "code/png.h"
#include <cstdint>

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    // Test png_save_int_32 and png_get_int_32
    if (Size >= 4) {
        png_byte buf32[4];
        png_int_32 i32;
        // Safe copy of 4 bytes
        memcpy(&i32, Data, 4);
        png_save_int_32(buf32, i32);
        png_int_32 i32_get = png_get_int_32(buf32);
        (void)i32_get; // Avoid unused variable warning
    }

    // Test png_save_uint_16 and png_get_uint_16
    if (Size >= 2) {
        png_byte buf16[2];
        unsigned int u16 = 0;
        // Safe copy of 2 bytes
        if (Size >= 2) {
            memcpy(&u16, Data, 2);
        }
        png_save_uint_16(buf16, u16);
        png_uint_16 u16_get = png_get_uint_16(buf16);
        (void)u16_get;
    }

    // Test png_save_uint_32 and png_get_uint_32
    if (Size >= 4) {
        png_byte buf32[4];
        png_uint_32 u32;
        // Safe copy of 4 bytes
        memcpy(&u32, Data, 4);
        png_save_uint_32(buf32, u32);
        png_uint_32 u32_get = png_get_uint_32(buf32);
        (void)u32_get;
    }

    // Test png_get_uint_32 directly on input data
    if (Size >= 4) {
        png_uint_32 u32_direct = png_get_uint_32((const png_byte*)Data);
        (void)u32_direct;
    }

    // Test png_get_uint_16 directly on input data
    if (Size >= 2) {
        png_uint_16 u16_direct = png_get_uint_16((const png_byte*)Data);
        (void)u16_direct;
    }

    // Test png_get_int_32 directly on input data
    if (Size >= 4) {
        png_int_32 i32_direct = png_get_int_32((const png_byte*)Data);
        (void)i32_direct;
    }

    // Additional tests with different offsets to explore more states
    if (Size >= 8) {
        // Test with second 4-byte chunk
        png_byte buf32[4];
        png_int_32 i32;
        // Safe copy of 4 bytes from offset 4
        memcpy(&i32, Data + 4, 4);
        png_save_int_32(buf32, i32);
        png_int_32 i32_get = png_get_int_32(buf32);
        (void)i32_get;
    }

    if (Size >= 6) {
        // Test with second 2-byte chunk
        png_byte buf16[2];
        unsigned int u16 = 0;
        // Safe copy of 2 bytes from offset 4
        if (Size >= 6) {
            memcpy(&u16, Data + 4, 2);
        }
        png_save_uint_16(buf16, u16);
        png_uint_16 u16_get = png_get_uint_16(buf16);
        (void)u16_get;
    }

    return 0;
}