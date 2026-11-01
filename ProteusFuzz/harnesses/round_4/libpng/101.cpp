// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
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
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Allocate buffers for save operations
    png_byte save_buf_32[4];
    png_byte save_buf_16[2];

    // Extract integers from input data
    png_int_32 input_int_32 = 0;
    png_uint_32 input_uint_32 = 0;
    unsigned int input_uint_16 = 0;

    if (Size >= 4) {
        std::memcpy(&input_int_32, Data, sizeof(png_int_32));
        std::memcpy(&input_uint_32, Data, sizeof(png_uint_32));
    }
    if (Size >= 2) {
        std::memcpy(&input_uint_16, Data, sizeof(unsigned int));
    }

    // Fuzz png_save_int_32
    png_save_int_32(save_buf_32, input_int_32);

    // Fuzz png_save_uint_16
    png_save_uint_16(save_buf_16, input_uint_16);

    // Fuzz png_save_uint_32
    png_save_uint_32(save_buf_32, input_uint_32);

    // Fuzz png_get_int_32
    if (Size >= 4) {
        png_int_32 result_int_32 = png_get_int_32(Data);
        (void)result_int_32; // Suppress unused variable warning
    }

    // Fuzz png_get_uint_32
    if (Size >= 4) {
        png_uint_32 result_uint_32 = png_get_uint_32(Data);
        (void)result_uint_32; // Suppress unused variable warning
    }

    // Fuzz png_get_uint_31 requires a valid png_struct
    // We create a minimal dummy png_struct to avoid crashes, 
    // but note that png_get_uint_31 may call png_error if value > PNG_UINT_31_MAX.
    // To prevent termination in fuzzing, we only call it if the value is within safe range.
    if (Size >= 4) {
        // Reconstruct uint32 from big-endian to check value safely
        png_uint_32 val = png_get_uint_32(Data);
        if (val <= PNG_UINT_31_MAX) {
            // Create a dummy png_struct to satisfy the API requirement
            // Note: In real fuzzing, a full png_create_read_struct is preferred, 
            // but for this specific API test, we pass a minimal struct.
            // However, png_get_uint_31 expects a valid struct to call png_error.
            // Since we cannot easily mock png_error without full libpng setup,
            // we skip calling png_get_uint_31 with potentially invalid values 
            // to avoid terminating the fuzzer.
            // Instead, we rely on the fact that the fuzzer will explore valid ranges.
            // For robustness, we only call it if we can ensure no error is triggered.
            // But since we can't easily control png_error, we skip this call 
            // unless we have a full png_struct initialized.
            // Given the constraints, we skip png_get_uint_31 to avoid undefined behavior.
        }
    }

    return 0;
}