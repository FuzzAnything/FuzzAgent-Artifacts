// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
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
#include <cstring>

#include "png.h"

extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    // Ensure we have at least 4 bytes for the functions that need them
    if (Size < 4) {
        return 0;
    }

    // Create a buffer for save operations
    static png_byte buffer[4];
    
    // Test png_get_uint_16 - requires at least 2 bytes
    if (Size >= 2) {
        png_uint_16 result_uint16 = png_get_uint_16(Data);
        (void)result_uint16; // Use result to avoid unused variable warning
    }

    // Test png_get_uint_32 - requires at least 4 bytes
    if (Size >= 4) {
        png_uint_32 result_uint32 = png_get_uint_32(Data);
        (void)result_uint32;
    }

    // Test png_save_uint_16 - use first 2 bytes as integer value
    if (Size >= 2) {
        unsigned int value_uint16 = (Data[0] << 8) | Data[1];
        png_save_uint_16(buffer, value_uint16);
    }

    // Test png_save_int_32 - use first 4 bytes as integer value
    if (Size >= 4) {
        png_int_32 value_int32 = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        png_save_int_32(buffer, value_int32);
    }

    // Test png_save_uint_32 - use first 4 bytes as integer value
    if (Size >= 4) {
        png_uint_32 value_uint32 = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        png_save_uint_32(buffer, value_uint32);
    }

    // Test png_get_int_32 - requires at least 4 bytes
    if (Size >= 4) {
        png_int_32 result_int32 = png_get_int_32(Data);
        (void)result_int32;
    }

    // Test combinations: save then get
    if (Size >= 4) {
        // Save a value then get it back
        png_uint_32 original_value = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        png_save_uint_32(buffer, original_value);
        png_uint_32 retrieved_value = png_get_uint_32(buffer);
        (void)retrieved_value;

        // Test with different byte patterns
        png_save_int_32(buffer, static_cast<png_int_32>(original_value));
        png_int_32 retrieved_int = png_get_int_32(buffer);
        (void)retrieved_int;
    }

    // Test with different offsets in the input data
    if (Size >= 8) {
        // Test with offset 2
        const png_byte* data_offset = Data + 2;
        if (Size - 2 >= 2) {
            png_uint_16 result_offset = png_get_uint_16(data_offset);
            (void)result_offset;
        }
        if (Size - 2 >= 4) {
            png_uint_32 result_offset32 = png_get_uint_32(data_offset);
            (void)result_offset32;
        }
    }

    return 0;
}