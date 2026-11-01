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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Buffer for reading and writing operations
    png_byte buf[4];
    png_byte buf16[2];

    // Initialize buffers with fuzzing data
    std::memcpy(buf, Data, 4);
    if (Size >= 2) {
        std::memcpy(buf16, Data, 2);
    } else {
        buf16[0] = Data[0];
        buf16[1] = Data[0];
    }

    // Test png_get_uint_32
    png_uint_32 u32_val = png_get_uint_32(buf);
    (void)u32_val; // Suppress unused variable warning

    // Test png_save_uint_32
    png_uint_32 save_u32_val = (Size >= 4) ? png_get_uint_32(Data) : 0;
    png_save_uint_32(buf, save_u32_val);

    // Test png_get_int_32
    png_int_32 i32_val = png_get_int_32(buf);
    (void)i32_val;

    // Test png_save_int_32
    png_int_32 save_i32_val = (Size >= 4) ? png_get_int_32(Data) : 0;
    png_save_int_32(buf, save_i32_val);

    // Test png_save_uint_16
    unsigned int u16_val = (Size >= 2) ? (unsigned int)(Data[0] | (Data[1] << 8)) : 0;
    png_save_uint_16(buf16, u16_val);

    // Test png_get_uint_31 requires a valid png_struct pointer
    // Since we cannot easily create a valid png_struct without more context,
    // we skip direct invocation of png_get_uint_31 to avoid crashes or undefined behavior.
    // However, we can simulate the buffer usage that would be passed to it.
    if (Size >= 4) {
        png_uint_32 u31_check = png_get_uint_32(Data);
        if (u31_check > PNG_UINT_31_MAX) {
            // This would normally trigger an error in png_get_uint_31, but we just check the value here.
            (void)u31_check;
        }
    }

    return 0;
}