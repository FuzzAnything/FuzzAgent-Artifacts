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

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    // Need at least 10 bytes to safely extract uint32, int32, and uint16 values
    if (Size < 10) {
        return 0;
    }

    // Allocate buffers for operations
    png_byte buf4[4];
    png_byte buf2[2];
    png_uint_32 u32_val;
    png_int_32 i32_val;
    unsigned int u16_val;

    // Initialize buffers with fuzz data
    std::memcpy(buf4, Data, 4);
    std::memcpy(buf2, Data + 4, 2);

    // Extract values from input for saving operations
    // Use first 4 bytes as uint32
    u32_val = png_get_uint_32(Data);
    // Use next 4 bytes as int32
    i32_val = png_get_int_32(Data + 4);
    // Use next 2 bytes as uint16 (indices 8 and 9)
    u16_val = (unsigned int)Data[8] << 8 | Data[9];

    // Test png_save_uint_32
    png_save_uint_32(buf4, u32_val);
    
    // Test png_save_int_32
    png_save_int_32(buf4, i32_val);
    
    // Test png_save_uint_16
    png_save_uint_16(buf2, u16_val);

    // Test png_get_uint_32
    png_uint_32 read_u32 = png_get_uint_32(buf4);
    (void)read_u32; // Suppress unused variable warning

    // Test png_get_int_32
    png_int_32 read_i32 = png_get_int_32(buf4);
    (void)read_i32;

    // Test png_get_uint_31 requires a valid png_ptr, which we cannot safely construct here
    // without risking crashes via png_error. Skipping to maintain fuzzer stability.

    return 0;
}