// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for the functions that require buffers
    if (Size < 8) {
        return 0;
    }

    // Cast the input data to png_byte pointer for libpng functions
    const png_byte* png_data = reinterpret_cast<const png_byte*>(Data);

    // Test png_get_uint_16 - requires at least 2 bytes
    if (Size >= 2) {
        png_uint_16 result16 = png_get_uint_16(png_data);
        (void)result16; // Avoid unused variable warning
    }

    // Test png_get_uint_32 - requires at least 4 bytes
    if (Size >= 4) {
        png_uint_32 result32 = png_get_uint_32(png_data);
        (void)result32; // Avoid unused variable warning
    }

    // Test png_get_int_32 - requires at least 4 bytes
    if (Size >= 4) {
        png_int_32 result_int32 = png_get_int_32(png_data);
        (void)result_int32; // Avoid unused variable warning
    }

    // Test png_get_uint_31 - requires at least 4 bytes and a png_struct
    if (Size >= 4) {
        // Create a minimal png_struct for testing
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (png_ptr) {
            png_uint_32 result31 = png_get_uint_31(png_ptr, png_data);
            (void)result31; // Avoid unused variable warning
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        }
    }

    // Test png_save_int_32 - requires a buffer and an integer
    if (Size >= 8) {
        png_byte buf_int32[4];
        // Use bytes 4-7 to form an integer for testing
        png_int_32 test_int32 = static_cast<png_int_32>(
            (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7]
        );
        png_save_int_32(buf_int32, test_int32);
    }

    // Test png_save_uint_32 - requires a buffer and an integer
    if (Size >= 8) {
        png_byte buf_uint32[4];
        // Use bytes 4-7 to form an unsigned integer for testing
        png_uint_32 test_uint32 = static_cast<png_uint_32>(
            (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7]
        );
        png_save_uint_32(buf_uint32, test_uint32);
    }

    return 0;
}