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
#include <cstddef>
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    png_byte buf[4];
    png_byte buf2[2];
    png_uint_32 uint32_val;
    png_int_32 int32_val;
    unsigned int uint16_val;

    // Initialize buffers from input data
    for (int i = 0; i < 4; ++i) {
        buf[i] = Data[i];
    }
    if (Size >= 6) {
        buf2[0] = Data[4];
        buf2[1] = Data[5];
    }

    // Fuzz png_get_uint_32
    uint32_val = png_get_uint_32(buf);

    // Fuzz png_save_uint_32
    png_save_uint_32(buf, uint32_val);

    // Fuzz png_get_int_32
    int32_val = png_get_int_32(buf);

    // Fuzz png_save_int_32
    png_save_int_32(buf, int32_val);

    // Fuzz png_save_uint_16
    if (Size >= 6) {
        uint16_val = (buf2[0] << 8) | buf2[1];
        png_save_uint_16(buf2, uint16_val);
    }

    // Fuzz png_get_uint_31
    // This function requires a valid png_struct pointer.
    // Since we cannot easily create a valid png_struct without complex setup
    // and the function triggers png_error on invalid data, we skip direct fuzzing
    // of png_get_uint_31 in this minimal driver to avoid crashes from
    // incomplete png_struct initialization or error handler issues.
    // In a full fuzzing campaign, this would require a proper png_struct setup.
    // For now, we ensure the other functions are exercised.

    return 0;
}