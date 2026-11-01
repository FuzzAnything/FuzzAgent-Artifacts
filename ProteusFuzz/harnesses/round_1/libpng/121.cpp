// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
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
#include <cstring>
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    png_byte buf16[2];
    png_byte buf32[4];
    png_int_32 int32_result;
    png_uint_32 uint32_result;
    png_uint_32 uint31_result;

    // Test png_save_uint_16
    unsigned int val16 = Data[0] | (Data[1] << 8);
    png_save_uint_16(buf16, val16);

    // Test png_save_uint_32
    png_uint_32 val32 = (png_uint_32)Data[0] | ((png_uint_32)Data[1] << 8) | 
                        ((png_uint_32)Data[2] << 16) | ((png_uint_32)Data[3] << 24);
    png_save_uint_32(buf32, val32);

    // Test png_get_int_32
    int32_result = png_get_int_32(buf32);

    // Test png_get_uint_32
    uint32_result = png_get_uint_32(buf32);

    // Test png_save_int_32
    png_int_32 val_signed = (png_int_32)(int32_t)val32;
    png_save_int_32(buf32, val_signed);

    // Test png_get_uint_31 with pre-validation to avoid png_error
    if (Size >= 4) {
        png_uint_32 raw_val = png_get_uint_32(Data);
        if (raw_val <= PNG_UINT_31_MAX) {
            // Create a minimal png_struct for png_get_uint_31 if needed,
            // but since we cannot easily initialize png_struct without more context,
            // we skip calling png_get_uint_31 directly to avoid potential aborts.
            // Instead, we rely on the pre-validation check above.
            (void)raw_val; // Suppress unused variable warning
        }
    }

    return 0;
}