// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
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
#include "code/png.h"
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_403(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for all operations
    if (Size < 12) {
        return 0;
    }

    // Create buffers for write operations
    png_byte write_buffer_16[2];
    png_byte write_buffer_32[4];
    
    // Create buffers for read operations
    png_byte read_buffer_16[2];
    png_byte read_buffer_32[4];
    
    // Initialize read buffers with fuzzer data
    memcpy(read_buffer_16, Data, 2);
    memcpy(read_buffer_32, Data + 2, 4);
    
    // Extract integer values from fuzzer data
    // For 16-bit values, use bytes 6-7
    unsigned int uint16_val = (Data[6] << 8) | Data[7];
    
    // For 32-bit values, use bytes 8-11
    png_uint_32 uint32_val = (Data[8] << 24) | (Data[9] << 16) | 
                             (Data[10] << 8) | Data[11];
    png_int_32 int32_val = static_cast<png_int_32>(uint32_val);
    
    // Test png_save_uint_16
    png_save_uint_16(write_buffer_16, uint16_val);
    
    // Test png_save_uint_32
    png_save_uint_32(write_buffer_32, uint32_val);
    
    // Test png_save_int_32
    png_save_int_32(write_buffer_32, int32_val);
    
    // Test png_get_uint_16
    png_uint_16 read_uint16 = png_get_uint_16(read_buffer_16);
    
    // Test png_get_uint_32
    png_uint_32 read_uint32 = png_get_uint_32(read_buffer_32);
    
    // Test png_get_int_32
    png_int_32 read_int32 = png_get_int_32(read_buffer_32);
    
    // Additional test: round-trip consistency for 16-bit
    png_byte roundtrip_buffer_16[2];
    png_save_uint_16(roundtrip_buffer_16, read_uint16);
    png_uint_16 roundtrip_uint16 = png_get_uint_16(roundtrip_buffer_16);
    
    // Additional test: round-trip consistency for 32-bit unsigned
    png_byte roundtrip_buffer_32_uint[4];
    png_save_uint_32(roundtrip_buffer_32_uint, read_uint32);
    png_uint_32 roundtrip_uint32 = png_get_uint_32(roundtrip_buffer_32_uint);
    
    // Additional test: round-trip consistency for 32-bit signed
    png_byte roundtrip_buffer_32_int[4];
    png_save_int_32(roundtrip_buffer_32_int, read_int32);
    png_int_32 roundtrip_int32 = png_get_int_32(roundtrip_buffer_32_int);
    
    // Test with different byte patterns if we have more data
    if (Size >= 16) {
        // Test with swapped byte order
        png_byte swapped_buffer[4];
        swapped_buffer[0] = Data[12];
        swapped_buffer[1] = Data[13];
        swapped_buffer[2] = Data[14];
        swapped_buffer[3] = Data[15];
        
        png_uint_32 swapped_uint32 = png_get_uint_32(swapped_buffer);
        png_int_32 swapped_int32 = png_get_int_32(swapped_buffer);
        
        // Save the swapped values
        png_save_uint_32(write_buffer_32, swapped_uint32);
        png_save_int_32(write_buffer_32, swapped_int32);
    }
    
    // Test edge cases for signed integer
    if (Size >= 20) {
        // Create buffer with potential overflow pattern
        png_byte overflow_buffer[4];
        memcpy(overflow_buffer, Data + 16, 4);
        
        // This should handle overflow by returning 0
        png_int_32 overflow_val = png_get_int_32(overflow_buffer);
        
        // Save it back
        png_save_int_32(write_buffer_32, overflow_val);
    }
    
    return 0;
}