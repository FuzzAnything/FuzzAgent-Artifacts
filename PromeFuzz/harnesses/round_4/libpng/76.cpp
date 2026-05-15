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
#include <cstdint>
#include <cstring>

#include "png.h"

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for all operations
    if (Size < 16) {
        return 0;
    }

    // Create buffers for read operations
    const png_byte* read_buf = reinterpret_cast<const png_byte*>(Data);
    
    // Test png_get_uint_16 - requires at least 2 bytes
    png_uint_16 uint16_val = png_get_uint_16(read_buf);
    
    // Test png_get_uint_32 - requires at least 4 bytes
    png_uint_32 uint32_val = png_get_uint_32(read_buf);
    
    // Test png_get_int_32 - requires at least 4 bytes
    png_int_32 int32_val = png_get_int_32(read_buf);
    
    // Create buffers for write operations
    png_byte write_buf_16[2];
    png_byte write_buf_32[4];
    
    // Test png_save_uint_16
    png_save_uint_16(write_buf_16, static_cast<unsigned int>(uint16_val));
    
    // Test png_save_int_32
    png_save_int_32(write_buf_32, int32_val);
    
    // Test png_save_uint_32
    png_save_uint_32(write_buf_32, uint32_val);
    
    // Test with different offsets to explore more states
    if (Size >= 8) {
        const png_byte* alt_buf = reinterpret_cast<const png_byte*>(Data + 4);
        png_uint_16 alt_uint16 = png_get_uint_16(alt_buf);
        png_uint_32 alt_uint32 = png_get_uint_32(alt_buf);
        png_int_32 alt_int32 = png_get_int_32(alt_buf);
        
        // Test write operations with alternate values
        png_save_uint_16(write_buf_16, static_cast<unsigned int>(alt_uint16));
        png_save_int_32(write_buf_32, alt_int32);
        png_save_uint_32(write_buf_32, alt_uint32);
    }
    
    // Test with maximum/minimum values
    png_save_uint_16(write_buf_16, 0xFFFF);
    png_save_uint_16(write_buf_16, 0);
    
    png_save_int_32(write_buf_32, 0x7FFFFFFF);
    png_save_int_32(write_buf_32, -0x7FFFFFFF);
    png_save_int_32(write_buf_32, 0);
    
    png_save_uint_32(write_buf_32, 0xFFFFFFFF);
    png_save_uint_32(write_buf_32, 0);
    
    return 0;
}