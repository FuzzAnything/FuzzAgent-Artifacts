// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
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
#include "code/png.h"
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    // Test png_save_uint_16 and png_get_uint_16
    if (Size >= 2) {
        png_byte buf16[2];
        // Use first 2 bytes as integer value
        unsigned int val16 = (Data[0] << 8) | Data[1];
        png_save_uint_16(buf16, val16);
        
        // Verify round-trip consistency
        png_uint_16 read_val16 = png_get_uint_16(buf16);
        if (read_val16 != val16) {
            // Mismatch found - could indicate bug
            __builtin_trap();
        }
        
        // Also test with random buffer data
        if (Size >= 4) {
            png_byte buf16_2[2];
            memcpy(buf16_2, Data + 2, 2);
            png_uint_16 random_val16 = png_get_uint_16(buf16_2);
            // Test save with this random value
            png_byte buf16_3[2];
            png_save_uint_16(buf16_3, random_val16);
        }
    }
    
    // Test png_get_uint_32
    if (Size >= 4) {
        png_byte buf32[4];
        memcpy(buf32, Data, 4);
        png_uint_32 val32 = png_get_uint_32(buf32);
        
        // Test png_save_uint_32 with the read value
        png_byte buf32_saved[4];
        png_save_uint_32(buf32_saved, val32);
        
        // Verify consistency
        png_uint_32 val32_roundtrip = png_get_uint_32(buf32_saved);
        if (val32 != val32_roundtrip) {
            __builtin_trap();
        }
        
        // Test with different offset if enough data
        if (Size >= 8) {
            png_byte buf32_2[4];
            memcpy(buf32_2, Data + 4, 4);
            png_uint_32 val32_2 = png_get_uint_32(buf32_2);
        }
    }
    
    // Test png_save_int_32 and png_get_int_32
    if (Size >= 4) {
        // Use bytes to create a signed integer (interpreting as two's complement)
        png_int_32 signed_val = static_cast<png_int_32>(
            (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
        
        png_byte buf_signed[4];
        png_save_int_32(buf_signed, signed_val);
        
        // Read it back
        png_int_32 read_signed = png_get_int_32(buf_signed);
        
        // Test with buffer from input
        png_byte buf_signed2[4];
        memcpy(buf_signed2, Data, 4);
        png_int_32 read_signed2 = png_get_int_32(buf_signed2);
        
        // Test save with the second read value
        if (Size >= 8) {
            png_byte buf_signed3[4];
            png_save_int_32(buf_signed3, read_signed2);
        }
    }
    
    // Additional combinatorial tests
    if (Size >= 6) {
        // Mix 16-bit and 32-bit operations
        png_byte mixed_buf[6];
        memcpy(mixed_buf, Data, 6);
        
        // Read 16-bit from first 2 bytes
        png_uint_16 mixed_16 = png_get_uint_16(mixed_buf);
        
        // Read 32-bit from bytes 2-5 (overlapping)
        png_uint_32 mixed_32 = png_get_uint_32(mixed_buf + 2);
        
        // Save both values
        png_byte save_buf[6];
        png_save_uint_16(save_buf, mixed_16);
        png_save_uint_32(save_buf + 2, mixed_32);
    }
    
    return 0;
}