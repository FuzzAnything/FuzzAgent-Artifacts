// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    // Minimum size required to extract at least one 32-bit value for testing
    if (Size < 4) {
        return 0;
    }

    // Prepare buffers
    png_byte buf16[2];
    png_byte buf32[4];

    // Extract values from input data
    unsigned int val16 = 0;
    png_uint_32 val32 = 0;
    png_int_32 sVal32 = 0;

    // Extract 16-bit value
    if (Size >= 2) {
        val16 = (unsigned int)(Data[0] | (Data[1] << 8));
    }

    // Extract 32-bit value
    if (Size >= 4) {
        val32 = (png_uint_32)Data[0] | ((png_uint_32)Data[1] << 8) | 
                ((png_uint_32)Data[2] << 16) | ((png_uint_32)Data[3] << 24);
        sVal32 = (png_int_32)val32;
    }

    // 1. Test png_save_uint_16
    png_save_uint_16(buf16, val16);

    // 2. Test png_get_uint_32 (requires 4 bytes, safe as we checked Size >= 4)
    // We write some data to buf32 first to ensure it has content
    png_save_uint_32(buf32, val32);
    png_uint_32 read32 = png_get_uint_32(buf32);
    (void)read32; // Suppress unused warning

    // 3. Test png_save_int_32
    png_save_int_32(buf32, sVal32);

    // 4. Test png_save_uint_32
    png_save_uint_32(buf32, val32);

    // 5. Test png_get_int_32
    png_int_32 readS32 = png_get_int_32(buf32);
    (void)readS32;

    // 6. Test png_get_uint_31
    // This function calls png_error if the value exceeds PNG_UINT_31_MAX.
    // png_error typically aborts or longjmps, which crashes the fuzzer.
    // To prevent the fuzzer from crashing on every input that triggers this,
    // we only call png_get_uint_31 if the value is within the safe range.
    // This allows the fuzzer to explore other paths without terminating.
    // If we wanted to test the error path, we would need a custom error handler
    // or accept that the fuzzer will crash on those specific inputs.
    // For stability, we skip the call if the value is too large.
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr) {
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr) {
            // Only call if value is safe to avoid png_error crash
            if (val32 <= PNG_UINT_31_MAX) {
                png_uint_32 safeVal = png_get_uint_31(png_ptr, buf32);
                (void)safeVal;
            }
            // If val32 > PNG_UINT_31_MAX, we skip the call to prevent crash.
            // This is a deliberate choice to keep the fuzzer running.
            // In a real-world scenario, one might want to catch the error,
            // but libpng's error handling is not exception-based.
            
            png_destroy_info_struct(png_ptr, &info_ptr);
        }
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }

    return 0;
}