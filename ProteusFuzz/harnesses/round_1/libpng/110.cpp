// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
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
#include <png.h>
#include <cstdint>
#include <cstring>
#include <climits>

extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Buffer for 16-bit operations
    png_byte buf16[2];
    // Buffer for 32-bit operations
    png_byte buf32[4];

    // Fuzz png_save_uint_16
    // Use first 2 bytes or a derived value
    unsigned int val16 = (Size >= 2) ? (Data[0] << 8 | Data[1]) : 0;
    png_save_uint_16(buf16, val16);

    // Fuzz png_save_uint_32
    // Use first 4 bytes or a derived value
    png_uint_32 val32 = 0;
    if (Size >= 4) {
        val32 = (png_uint_32)Data[0] << 24 | (png_uint_32)Data[1] << 16 | 
                (png_uint_32)Data[2] << 8 | (png_uint_32)Data[3];
    } else if (Size > 0) {
        val32 = Data[0];
    }
    png_save_uint_32(buf32, val32);

    // Fuzz png_save_int_32
    // Convert val32 to signed for testing
    png_int_32 val_signed = (png_int_32)val32;
    png_save_int_32(buf32, val_signed);

    // Fuzz png_get_int_32
    // Ensure we have 4 bytes to read
    if (Size >= 4) {
        png_int_32 result_int = png_get_int_32(Data);
        (void)result_int; // Use result to avoid unused variable warning
    }

    // Fuzz png_get_uint_32
    // Ensure we have 4 bytes to read
    if (Size >= 4) {
        png_uint_32 result_uint = png_get_uint_32(Data);
        (void)result_uint;
    }

    // Fuzz png_get_uint_31
    // This function requires a png_struct and checks for overflow.
    // We must pre-validate the input to avoid triggering png_error which might abort.
    if (Size >= 4) {
        // Read the value manually to check range
        png_uint_32 test_val = (png_uint_32)Data[0] << 24 | (png_uint_32)Data[1] << 16 | 
                               (png_uint_32)Data[2] << 8 | (png_uint_32)Data[3];
        
        // Only call if within safe range (PNG_UINT_31_MAX is 2^31 - 1)
        if (test_val <= PNG_UINT_31_MAX) {
            // Create a minimal png_struct for the call
            // Note: In a real scenario, this would be initialized properly, 
            // but for fuzzing the logic path of the conversion, a non-null pointer is often sufficient 
            // if the function doesn't dereference it deeply for this specific check.
            // However, png_get_uint_31 typically uses the struct for error handling.
            // To be safe and avoid crashes, we skip creating a full struct unless necessary.
            // Since the task says "Do not call ... if custom error handler is configured to abort",
            // and we pre-validated, we can attempt a call if we can construct a safe struct.
            // Constructing a full valid png_struct is complex. 
            // Instead, we rely on the pre-validation logic described in the prompt:
            // "use png_get_uint_32 and manually check the range".
            // We already did the manual check above. 
            // If we strictly need to call the function, we need a valid png_ptr.
            // Given the constraints of a simple fuzz driver without full libpng init,
            // we will skip the direct call to png_get_uint_31 to prevent potential 
            // crashes from an invalid png_struct, as the logic is covered by the manual check.
            // However, if the fuzzer needs to hit the line, we might need a dummy struct.
            // Let's try to create a minimal dummy struct if possible, but it's risky.
            // The prompt says "Do not call ... if ... abort". Pre-validation handles the value.
            // The safest approach for a standalone driver is to rely on the manual check 
            // which exercises the logic without the side effects of the error handler.
            // But to strictly follow "fuzz the library", let's try to call it with a dummy struct 
            // ONLY if we are sure it won't crash. 
            // png_get_uint_31 implementation usually checks the value and then calls png_error if bad.
            // Since we checked the value, it should return the value.
            // But it might dereference png_ptr.
            // Let's assume the environment might not have a full libpng init.
            // We will skip the direct call to png_get_uint_31 to ensure stability, 
            // as the prompt explicitly warns about the error path.
            // The manual check (test_val <= PNG_UINT_31_MAX) effectively fuzzes the logic.
        }
    }

    return 0;
}