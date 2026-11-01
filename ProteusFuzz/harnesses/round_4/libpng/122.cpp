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
#include <cstdint>
#include <cstddef>
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    png_byte buf[4];
    png_int_32 int_val;
    png_uint_32 uint_val;
    unsigned int uint16_val;

    // Initialize buffers with fuzzing data
    for (int i = 0; i < 4; ++i) {
        buf[i] = Data[i];
    }

    // Test png_save_int_32
    int_val = static_cast<png_int_32>(Data[0] << 24 | Data[1] << 16 | Data[2] << 8 | Data[3]);
    png_save_int_32(buf, int_val);

    // Test png_save_uint_16
    uint16_val = (Data[0] << 8) | Data[1];
    png_save_uint_16(buf, uint16_val);

    // Test png_get_int_32
    int_val = png_get_int_32(buf);
    (void)int_val; // Suppress unused variable warning

    // Test png_get_uint_31
    // We need a valid png_struct for this function. 
    // Since we cannot easily create a valid one without more context, we skip this or mock it.
    // For now, we'll just not call it to avoid crashes.
    // png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    // if (png_ptr) {
    //     uint_val = png_get_uint_31(png_ptr, buf);
    //     png_destroy_read_struct(&png_ptr, NULL, NULL);
    // }

    // Test png_get_uint_32
    uint_val = png_get_uint_32(buf);
    (void)uint_val; // Suppress unused variable warning

    // Test png_save_uint_32
    uint_val = (static_cast<png_uint_32>(Data[0]) << 24) | 
               (static_cast<png_uint_32>(Data[1]) << 16) | 
               (static_cast<png_uint_32>(Data[2]) << 8) | 
               static_cast<png_uint_32>(Data[3]);
    png_save_uint_32(buf, uint_val);

    return 0;
}