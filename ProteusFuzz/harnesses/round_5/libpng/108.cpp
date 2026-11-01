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

extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare buffers
    png_byte buf[4];
    png_byte buf16[2];
    png_uint_32 val32;
    png_int_32 val32s;
    unsigned int val16;

    // Initialize buffers with input data
    std::memcpy(buf, Data, 4);
    if (Size >= 2) {
        std::memcpy(buf16, Data, 2);
    }

    // Test png_get_uint_32
    val32 = png_get_uint_32(buf);

    // Test png_save_uint_32
    png_save_uint_32(buf, val32);

    // Test png_get_int_32
    val32s = png_get_int_32(buf);

    // Test png_save_int_32
    png_save_int_32(buf, val32s);

    // Test png_save_uint_16
    val16 = (Size >= 2) ? ((buf16[0] << 8) | buf16[1]) : 0;
    png_save_uint_16(buf16, val16);

    // Test png_get_uint_31 requires a valid png_struct
    // Since we cannot easily create a valid png_struct without full initialization,
    // we skip direct testing of png_get_uint_31 to avoid undefined behavior.
    // However, we can simulate the buffer check manually if needed, but the API requires png_ptr.
    // For fuzzing purposes, we assume the caller ensures png_ptr is valid when using png_get_uint_31.
    // Thus, we do not invoke png_get_uint_31 here to prevent crashes.

    return 0;
}