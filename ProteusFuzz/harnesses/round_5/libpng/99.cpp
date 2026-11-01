// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
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
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    png_byte buf[4];
    png_byte buf16[2];
    png_uint_32 u32_val;
    png_int_32 i32_val;
    unsigned int u16_val;

    // Initialize buffers from input data
    std::memcpy(buf, Data, 4);
    std::memcpy(buf16, Data, 2);

    // Fuzz png_get_uint_32
    u32_val = png_get_uint_32(buf);
    (void)u32_val;

    // Fuzz png_save_uint_32
    u32_val = (Size > 4) ? ((png_uint_32)Data[4] << 24) | ((png_uint_32)Data[5] << 16) | ((png_uint_32)Data[6] << 8) | Data[7] : 0;
    png_save_uint_32(buf, u32_val);

    // Fuzz png_get_int_32
    i32_val = png_get_int_32(buf);
    (void)i32_val;

    // Fuzz png_save_int_32
    i32_val = (Size > 4) ? ((png_int_32)Data[4] << 24) | ((png_int_32)Data[5] << 16) | ((png_int_32)Data[6] << 8) | Data[7] : 0;
    png_save_int_32(buf, i32_val);

    // Fuzz png_save_uint_16
    u16_val = (Size > 2) ? ((unsigned int)Data[0] << 8) | Data[1] : 0;
    png_save_uint_16(buf16, u16_val);

    // Fuzz png_get_uint_31 requires a valid png_struct
    // Since we cannot easily create a valid png_struct without more complex setup and potential side effects,
    // and the task emphasizes fuzzing the provided APIs, we skip png_get_uint_31 to avoid undefined behavior
    // or complex initialization that might not be supported in a minimal fuzz driver context.
    // However, if a dummy struct is strictly required and safe to create:
    // Note: Creating a real png_struct usually requires png_create_read_struct which may call abort on errors.
    // Given the constraints and the nature of the other functions being low-level byte manipulators,
    // we focus on the ones that do not require complex state.
    
    return 0;
}