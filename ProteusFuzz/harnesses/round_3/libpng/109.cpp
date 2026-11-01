// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <cstddef>
#include <cstdint>

extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    png_byte buf[4];
    png_byte buf16[2];
    png_byte buf_out32[4];
    png_uint_32 u32_val;
    png_int_32 i32_val;
    png_uint_32 u16_val;
    png_int_32 i32_out;
    png_uint_32 u32_out;
    png_uint_32 u31_out;

    // Initialize buffers from input safely
    for (int i = 0; i < 4; ++i) {
        buf[i] = Data[i];
    }

    // Test png_save_uint_16
    // Ensure we have enough data for 2 bytes
    if (Size >= 6) {
        u16_val = (png_uint_32)Data[4] | ((png_uint_32)Data[5] << 8);
    } else {
        u16_val = 0;
    }
    png_save_uint_16(buf16, u16_val);

    // Test png_get_uint_32
    u32_val = png_get_uint_32(buf);

    // Test png_save_int_32
    png_int_32 test_i32 = 0;
    if (Size >= 12) {
        test_i32 = (png_int_32)Data[8] | ((png_int_32)Data[9] << 8) | 
                   ((png_int_32)Data[10] << 16) | ((png_int_32)Data[11] << 24);
    }
    png_save_int_32(buf_out32, test_i32);

    // Test png_save_uint_32
    png_uint_32 test_u32 = 0;
    if (Size >= 16) {
        test_u32 = (png_uint_32)Data[12] | ((png_uint_32)Data[13] << 8) | 
                   ((png_uint_32)Data[14] << 16) | ((png_uint_32)Data[15] << 24);
    }
    png_save_uint_32(buf_out32, test_u32);

    // Test png_get_int_32
    i32_out = png_get_int_32(buf);

    // Test png_get_uint_31
    // Create a minimal png_struct to satisfy the function signature
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr) {
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr) {
            // We use a custom error handler to prevent abort on invalid data
            // However, setting it up requires more complex logic. 
            // Instead, we rely on the fact that if the value is valid, it returns.
            // If it's invalid, it calls png_error. To avoid crash, we can try to catch it?
            // But in C++, we can't easily catch png_error. 
            // The safest way is to ensure the input to png_get_uint_31 is within range 
            // or accept that the fuzzer might crash on invalid inputs (which is sometimes desired).
            // However, to keep the fuzzer running, we can manually check the value first.
            png_uint_32 val = png_get_uint_32(buf);
            if (val <= PNG_UINT_31_MAX) {
                u31_out = png_get_uint_31(png_ptr, buf);
            } else {
                // Skip calling png_get_uint_31 to avoid png_error
                u31_out = 0; 
            }
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        } else {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
        }
    }

    return 0;
}