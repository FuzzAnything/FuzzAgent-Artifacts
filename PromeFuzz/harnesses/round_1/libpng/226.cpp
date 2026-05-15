// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
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
#include <cstring>

#include "code/png.h"

extern "C" int LLVMFuzzerTestOneInput_226(const uint8_t *Data, size_t Size) {
    // We need at least 8 bytes for all operations: 4 for integer values and 4 for buffer
    if (Size < 8) {
        return 0;
    }

    // Extract integer values from fuzzer input
    unsigned int uint16_val = (Data[0] << 8) | Data[1];
    png_uint_32 uint32_val = (Data[2] << 24) | (Data[3] << 16) | (Data[4] << 8) | Data[5];
    png_int_32 int32_val = static_cast<png_int_32>(uint32_val);

    // Prepare buffers
    png_byte buf16[2];
    png_byte buf32[4];
    png_byte read_buf[4];

    // Copy data for read operations
    size_t copy_size = Size < 12 ? Size - 8 : 4;
    if (copy_size > 0) {
        memcpy(read_buf, Data + 8, copy_size);
        // Pad with zeros if needed
        for (size_t i = copy_size; i < 4; i++) {
            read_buf[i] = 0;
        }
    }

    // Test png_save_uint_16
    png_save_uint_16(buf16, uint16_val);

    // Test png_save_uint_32
    png_save_uint_32(buf32, uint32_val);

    // Test png_save_int_32
    png_save_int_32(buf32, int32_val);

    // Test png_get_uint_16
    png_uint_16 retrieved_uint16 = png_get_uint_16(read_buf);

    // Test png_get_uint_32
    png_uint_32 retrieved_uint32 = png_get_uint_32(read_buf);

    // Test png_get_int_32
    png_int_32 retrieved_int32 = png_get_int_32(read_buf);

    // Additional test: Use the saved buffers as input for get functions
    png_uint_16 from_saved16 = png_get_uint_16(buf16);
    png_uint_32 from_saved32 = png_get_uint_32(buf32);
    png_int_32 from_saved_int32 = png_get_int_32(buf32);

    // Test with different buffer alignments
    if (Size >= 12) {
        png_byte alt_buf[4];
        memcpy(alt_buf, Data + 8, 4);
        
        // Test with the alternative buffer
        png_uint_16 alt_uint16 = png_get_uint_16(alt_buf);
        png_uint_32 alt_uint32 = png_get_uint_32(alt_buf);
        png_int_32 alt_int32 = png_get_int_32(alt_buf);
    }

    // Test edge cases by manipulating the integer values
    // Test with maximum values
    png_save_uint_16(buf16, 0xFFFF);
    png_save_uint_32(buf32, 0xFFFFFFFF);
    png_save_int_32(buf32, 0x7FFFFFFF);
    png_save_int_32(buf32, static_cast<png_int_32>(0x80000000)); // Minimum signed 32-bit

    // Test with zero values
    png_save_uint_16(buf16, 0);
    png_save_uint_32(buf32, 0);
    png_save_int_32(buf32, 0);

    return 0;
}