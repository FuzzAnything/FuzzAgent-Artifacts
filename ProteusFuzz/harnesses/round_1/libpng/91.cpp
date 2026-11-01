// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare buffers for save operations
    png_byte buf16[2];
    png_byte buf32[4];
    
    // Extract values from fuzz input
    uint16_t val16 = (Size >= 2) ? ((Data[0] << 8) | Data[1]) : 0;
    uint32_t val32 = (Size >= 4) ? ((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]) : 0;
    int32_t val_int32 = static_cast<int32_t>(val32);
    
    // Test png_save_uint_16
    png_save_uint_16(buf16, val16);
    
    // Test png_save_uint_32
    png_save_uint_32(buf32, val32);
    
    // Test png_save_int_32
    png_save_int_32(buf32, val_int32);
    
    // Test png_get_int_32
    png_int_32 result_int32 = png_get_int_32(buf32);
    (void)result_int32; // Use result to avoid unused variable warning
    
    // Test png_get_uint_32
    png_uint_32 result_uint32 = png_get_uint_32(buf32);
    (void)result_uint32; // Use result to avoid unused variable warning
    
    // Test png_get_uint_31 with pre-validation
    // Only call if value is within safe range to avoid triggering error handler
    if (val32 <= PNG_UINT_31_MAX) {
        // Create a minimal png_struct for png_get_uint_31
        // Note: In real fuzzing, we might need a full png_struct setup
        // For this test, we'll skip the actual call if we can't safely initialize png_struct
        // This is a simplified approach; in practice, you'd need to initialize png_struct properly
        // png_const_structrp png_ptr = nullptr; // Would need proper initialization
        // png_uint_32 result_uint31 = png_get_uint_31(png_ptr, buf32);
        // (void)result_uint31;
    }
    
    // Additional exploration: test with edge cases
    png_save_uint_16(buf16, 0);
    png_save_uint_16(buf16, 65535);
    png_save_uint_16(buf16, 100000); // Test truncation
    
    png_save_uint_32(buf32, 0);
    png_save_uint_32(buf32, 4294967295U);
    
    png_save_int_32(buf32, 0);
    png_save_int_32(buf32, 2147483647);
    png_save_int_32(buf32, -2147483648);
    
    // Test reading with various patterns
    memset(buf32, 0, 4);
    png_get_int_32(buf32);
    png_get_uint_32(buf32);
    
    memset(buf32, 0xFF, 4);
    png_get_int_32(buf32);
    png_get_uint_32(buf32);
    
    return 0;
}