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
#include <cstddef>
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare buffers
    png_byte buf16[2];
    png_byte buf32[4];
    const png_byte *src32 = Data;
    
    // Extract values from input data
    unsigned int val16 = 0;
    if (Size >= 2) {
        val16 = Data[0] | (Data[1] << 8);
    }
    
    png_uint_32 val32 = 0;
    if (Size >= 4) {
        val32 = ((png_uint_32)Data[0] << 24) | 
                ((png_uint_32)Data[1] << 16) | 
                ((png_uint_32)Data[2] << 8) | 
                ((png_uint_32)Data[3]);
    }
    
    png_int_32 valInt32 = (png_int_32)val32;

    // Test png_save_uint_16
    png_save_uint_16(buf16, val16);
    
    // Test png_save_uint_32
    png_save_uint_32(buf32, val32);
    
    // Test png_save_int_32
    png_save_int_32(buf32, valInt32);
    
    // Test png_get_uint_32
    png_uint_32 read32 = png_get_uint_32(src32);
    (void)read32; // Suppress unused variable warning
    
    // Test png_get_int_32
    png_int_32 readInt32 = png_get_int_32(src32);
    (void)readInt32; // Suppress unused variable warning

    // Test png_get_uint_31 requires a valid png_struct
    // We cannot easily create a valid png_struct without more complex setup,
    // so we skip this specific call in the fuzzer to avoid crashes from invalid pointers.
    // However, we can simulate the buffer check logic if needed, but the function 
    // requires a valid png_ptr to call png_error.
    // To safely fuzz this, we would need to initialize a png_struct, which is complex.
    // Given the constraints, we omit png_get_uint_31 direct call with a fake pointer
    // as it would likely crash or trigger an error immediately.
    // Instead, we rely on the other functions to cover the byte manipulation logic.

    return 0;
}