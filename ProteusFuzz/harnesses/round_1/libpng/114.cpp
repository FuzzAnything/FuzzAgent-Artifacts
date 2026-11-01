// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    png_byte buf[4];
    png_uint_32 val32;
    png_int_32 val32s;
    unsigned int val16;

    // Test png_save_uint_16
    val16 = (Size > 0) ? (Data[0] << 8) | Data[1] : 0;
    png_save_uint_16(buf, val16);

    // Test png_save_uint_32
    val32 = (Size >= 4) ? ((png_uint_32)Data[0] << 24) | ((png_uint_32)Data[1] << 16) | 
                          ((png_uint_32)Data[2] << 8) | (png_uint_32)Data[3] : 0;
    png_save_uint_32(buf, val32);

    // Test png_get_int_32
    val32s = png_get_int_32(buf);
    (void)val32s; // Use result to avoid unused variable warning

    // Test png_get_uint_32
    val32 = png_get_uint_32(buf);
    (void)val32; // Use result to avoid unused variable warning

    // Test png_save_int_32
    val32s = (Size >= 4) ? (png_int_32)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]) : 0;
    png_save_int_32(buf, val32s);

    // Test png_get_uint_31 with pre-validation
    if (Size >= 4) {
        png_uint_32 raw_val = png_get_uint_32(buf);
        if (raw_val <= PNG_UINT_31_MAX) {
            // Create a minimal png_struct for the call
            png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if (png_ptr) {
                png_get_uint_31(png_ptr, buf);
                png_destroy_read_struct(&png_ptr, NULL, NULL);
            }
        }
    }

    return 0;
}