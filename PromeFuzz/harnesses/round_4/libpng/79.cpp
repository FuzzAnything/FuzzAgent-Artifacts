// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    // Ensure we have at least 4 bytes for the 32-bit operations
    if (Size < 4) {
        return 0;
    }

    // Test png_get_uint_16
    if (Size >= 2) {
        png_uint_16 val16 = png_get_uint_16(Data);
        (void)val16; // Prevent unused variable warning
    }

    // Test png_get_uint_32
    png_uint_32 val32 = png_get_uint_32(Data);
    (void)val32;

    // Test png_get_int_32
    png_int_32 val_int32 = png_get_int_32(Data);
    (void)val_int32;

    // Test png_save_uint_16
    png_byte buf16[2];
    png_save_uint_16(buf16, static_cast<unsigned int>(val32 & 0xFFFF));

    // Test png_save_int_32
    png_byte buf32[4];
    png_save_int_32(buf32, static_cast<png_int_32>(val_int32));

    // Test png_save_uint_32
    png_save_uint_32(buf32, val32);

    // Additional tests with different offsets to explore more states
    if (Size >= 8) {
        // Test with offset data
        const png_byte* offset_data = Data + 4;
        
        png_uint_16 offset_val16 = png_get_uint_16(offset_data);
        (void)offset_val16;
        
        png_uint_32 offset_val32 = png_get_uint_32(offset_data);
        (void)offset_val32;
        
        png_int_32 offset_val_int32 = png_get_int_32(offset_data);
        (void)offset_val_int32;
        
        // Test save functions with different values
        png_save_uint_16(buf16, static_cast<unsigned int>(offset_val32 & 0xFFFF));
        png_save_int_32(buf32, offset_val_int32);
        png_save_uint_32(buf32, offset_val32);
    }

    // Test with maximum and minimum values
    png_save_uint_16(buf16, 0xFFFF);
    png_save_uint_16(buf16, 0);
    
    png_save_int_32(buf32, 0x7FFFFFFF);
    png_save_int_32(buf32, -0x80000000);
    png_save_int_32(buf32, 0);
    
    png_save_uint_32(buf32, 0xFFFFFFFF);
    png_save_uint_32(buf32, 0);

    return 0;
}