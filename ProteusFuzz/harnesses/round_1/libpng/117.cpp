// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
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
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Buffer for save operations (2 bytes for uint16, 4 bytes for uint32/int32)
    png_byte save_buf[4];
    // Buffer for get operations (4 bytes)
    png_byte get_buf[4];

    // Helper to extract 4-byte values from input safely
    auto get_4byte_value = [&](size_t offset) -> png_uint_32 {
        if (offset + 4 > Size) {
            return 0;
        }
        png_uint_32 val = 0;
        val |= ((png_uint_32)Data[offset]) << 24;
        val |= ((png_uint_32)Data[offset + 1]) << 16;
        val |= ((png_uint_32)Data[offset + 2]) << 8;
        val |= ((png_uint_32)Data[offset + 3]);
        return val;
    };

    // 1. png_save_uint_16
    // Test with various values derived from input
    unsigned int val16 = 0;
    if (Size >= 2) {
        val16 = (Data[0] << 8) | Data[1];
    } else {
        val16 = 0;
    }
    // Also test with a value that might exceed 16 bits to test truncation
    unsigned int val16_large = (Size >= 4) ? ((unsigned int)Data[0] << 24) | ((unsigned int)Data[1] << 16) | ((unsigned int)Data[2] << 8) | Data[3] : 65536;
    
    png_save_uint_16(save_buf, val16);
    png_save_uint_16(save_buf, val16_large);

    // 2. png_save_uint_32
    png_uint_32 val32 = get_4byte_value(0);
    png_save_uint_32(save_buf, val32);

    // 3. png_save_int_32
    // Cast the 32-bit value to signed for testing
    png_int_32 val32_signed = (png_int_32)val32;
    png_save_int_32(save_buf, val32_signed);

    // 4. png_get_uint_32
    // Ensure we have at least 4 bytes for this
    if (Size >= 4) {
        memcpy(get_buf, Data, 4);
        png_uint_32 res = png_get_uint_32(get_buf);
        (void)res; // Use result to avoid unused warning
    }

    // 5. png_get_int_32
    if (Size >= 4) {
        memcpy(get_buf, Data, 4);
        png_int_32 res = png_get_int_32(get_buf);
        (void)res;
    }

    // 6. png_get_uint_31
    // This function triggers png_error if value > PNG_UINT_31_MAX.
    // We must pre-validate to avoid aborting the fuzzer.
    if (Size >= 4) {
        png_uint_32 candidate = get_4byte_value(0);
        if (candidate <= PNG_UINT_31_MAX) {
            // Create a dummy png_struct to satisfy the signature
            // We cannot fully initialize a png_struct without complex setup, 
            // but the function signature requires png_const_structrp.
            // However, looking at the implementation of png_get_uint_31, 
            // it often checks the value and calls png_error which uses the struct.
            // To safely fuzz without full initialization, we rely on the pre-check.
            // If we pass a valid pointer, it might try to access it on error.
            // Since we pre-checked, it should not error.
            // But we need a valid pointer to avoid null dereference if logic changes.
            // Creating a minimal struct is risky without full init.
            // Alternative: Only call if we are sure it won't error.
            // The safest approach in fuzzing for this specific function is to 
            // skip calling it if we cannot guarantee a safe environment, 
            // OR initialize a minimal png_struct.
            
            // Let's try to initialize a minimal png_struct.
            png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if (png_ptr) {
                memcpy(get_buf, Data, 4);
                png_uint_32 res = png_get_uint_31(png_ptr, get_buf);
                (void)res;
                png_destroy_read_struct(&png_ptr, NULL, NULL);
            }
        }
    }

    return 0;
}