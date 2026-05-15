// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
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
#include <cstdint>
#include <cstring>

// Include libpng headers using the specified relative path
#include "png.h"

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for testing
    if (Size < 8) {
        return 0;
    }

    // Convert input to png_byte pointer for libpng functions
    const png_byte* png_data = reinterpret_cast<const png_byte*>(Data);
    
    // Test png_get_uint_16 - requires at least 2 bytes
    if (Size >= 2) {
        png_uint_16 uint16_val = png_get_uint_16(png_data);
        (void)uint16_val; // Use result to avoid compiler warnings
    }
    
    // Test png_get_uint_32 - requires at least 4 bytes
    if (Size >= 4) {
        png_uint_32 uint32_val = png_get_uint_32(png_data);
        (void)uint32_val;
    }
    
    // Test png_get_int_32 - requires at least 4 bytes
    if (Size >= 4) {
        png_int_32 int32_val = png_get_int_32(png_data);
        (void)int32_val;
    }
    
    // Test save functions - need buffers to write to
    static png_byte write_buffer[4];
    
    // Test png_save_uint_16 - use first 2 bytes as input value
    if (Size >= 2) {
        unsigned int uint16_input = static_cast<unsigned int>(png_get_uint_16(png_data));
        png_save_uint_16(write_buffer, uint16_input);
        
        // Verify the saved value can be read back
        png_uint_16 read_back = png_get_uint_16(write_buffer);
        (void)read_back;
    }
    
    // Test png_save_int_32 - use first 4 bytes as input value
    if (Size >= 4) {
        png_int_32 int32_input = png_get_int_32(png_data);
        png_save_int_32(write_buffer, int32_input);
        
        // Verify the saved value can be read back
        png_int_32 read_back = png_get_int_32(write_buffer);
        (void)read_back;
    }
    
    // Test png_save_uint_32 - use first 4 bytes as input value
    if (Size >= 4) {
        png_uint_32 uint32_input = png_get_uint_32(png_data);
        png_save_uint_32(write_buffer, uint32_input);
        
        // Verify the saved value can be read back
        png_uint_32 read_back = png_get_uint_32(write_buffer);
        (void)read_back;
    }
    
    // Try different offsets to explore more states
    if (Size >= 6) {
        // Test with offset data
        png_uint_16 offset_uint16 = png_get_uint_16(png_data + 2);
        (void)offset_uint16;
    }
    
    if (Size >= 8) {
        // Test with different offset for 32-bit values
        png_uint_32 offset_uint32 = png_get_uint_32(png_data + 4);
        (void)offset_uint32;
        
        png_int_32 offset_int32 = png_get_int_32(png_data + 4);
        (void)offset_int32;
    }
    
    // Test save functions with different input values
    if (Size >= 4) {
        // Use different parts of input for different save operations
        png_save_uint_16(write_buffer, static_cast<unsigned int>(Data[0] << 8 | Data[1]));
        png_save_int_32(write_buffer, static_cast<png_int_32>(Data[2] << 24 | Data[3] << 16 | Data[0] << 8 | Data[1]));
        png_save_uint_32(write_buffer, static_cast<png_uint_32>(Data[0] << 24 | Data[1] << 16 | Data[2] << 8 | Data[3]));
    }
    
    return 0;
}