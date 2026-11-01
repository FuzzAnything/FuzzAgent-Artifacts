// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare buffers for save functions
    png_byte buf16[2];
    png_byte buf32[4];

    // Extract values from input data
    uint32_t val16 = 0;
    uint32_t val32 = 0;

    if (Size >= 2) {
        val16 = (Data[0] << 8) | Data[1];
    }
    if (Size >= 4) {
        val32 = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
    }

    // Fuzz png_save_uint_16
    png_save_uint_16(buf16, val16);

    // Fuzz png_save_uint_32
    png_save_uint_32(buf32, val32);

    // Fuzz png_save_int_32 (cast to signed)
    png_save_int_32(buf32, static_cast<png_int_32>(val32));

    // Fuzz png_get_int_32
    png_int_32 result_int32 = png_get_int_32(buf32);
    (void)result_int32; // Avoid unused variable warning

    // Fuzz png_get_uint_32
    png_uint_32 result_uint32 = png_get_uint_32(buf32);
    (void)result_uint32; // Avoid unused variable warning

    // Fuzz png_get_uint_31 with validation to avoid png_error abort
    // We need a png_struct for this, but we can't easily initialize one without more context.
    // Instead, we pre-validate the value in buf32.
    // PNG_UINT_31_MAX is 2147483647 (0x7FFFFFFF)
    if (result_uint32 <= 0x7FFFFFFF) {
        // Create a dummy png_struct to satisfy the API requirement.
        // Note: In a real fuzzing scenario, we might use a mock or minimal struct.
        // Since we cannot safely initialize a full png_struct without complex setup,
        // and the function logic primarily checks the buffer value, we simulate the check.
        // However, the API requires a pointer. We will pass a null pointer if the library
        // allows it for this specific function, or we must construct a minimal struct.
        // Given the constraints, we will skip calling png_get_uint_31 directly with a real struct
        // to avoid potential crashes from uninitialized memory, as the primary fuzzing goal
        // is the logic inside the function which we've already validated via the pre-check.
        // If we must call it, we would need to initialize png_struct. 
        // For safety in this isolated driver, we skip the direct call if struct init is complex.
        // But to strictly follow "invoke the target function", let's try to create a minimal struct.
        
        // Minimal png_struct initialization (assuming png.h provides png_create_read_struct)
        // This is risky in a fuzz driver if not cleaned up, but png_create_read_struct handles memory.
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (png_ptr) {
            png_get_uint_31(png_ptr, buf32);
            png_destroy_read_struct(&png_ptr, NULL, NULL);
        }
    }

    return 0;
}