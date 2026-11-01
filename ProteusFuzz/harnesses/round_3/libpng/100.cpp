// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
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
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare buffers for save operations
    png_byte buf16[2] = {0};
    png_byte buf32[4] = {0};

    // Extract values from input data for testing
    uint32_t val16 = 0;
    uint32_t val32 = 0;
    int32_t valSigned32 = 0;

    // Ensure we have enough data to extract values
    if (Size >= 4) {
        val16 = Data[0] | (Data[1] << 8);
        val32 = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
        valSigned32 = static_cast<int32_t>(val32);
    }

    // 1. png_save_uint_16
    png_save_uint_16(buf16, static_cast<unsigned int>(val16));

    // 2. png_get_uint_32
    // Reuse input data as buffer if size allows, otherwise use buf32
    const png_byte *getBuf32 = (Size >= 4) ? Data : buf32;
    png_uint_32 readVal32 = png_get_uint_32(getBuf32);
    (void)readVal32; // Suppress unused variable warning

    // 3. png_save_int_32
    png_save_int_32(buf32, valSigned32);

    // 4. png_save_uint_32
    png_save_uint_32(buf32, val32);

    // 5. png_get_int_32
    png_int_32 readSigned32 = png_get_int_32(getBuf32);
    (void)readSigned32; // Suppress unused variable warning

    // 6. png_get_uint_31
    // This function requires a valid png_struct pointer.
    // Since we cannot easily create a valid one without full initialization
    // and it may trigger errors on invalid data, we simulate a minimal call.
    // However, png_get_uint_31 calls png_error on invalid values, which exits.
    // To fuzz safely without crashing the fuzzer on expected errors, 
    // we must ensure the input data is valid or handle the error.
    // Given the constraint of fuzzing, we will attempt to create a minimal struct.
    // Note: png_create_read_struct is the standard way, but requires error handlers.
    // For this specific fuzz driver, we will skip direct invocation of png_get_uint_31
    // with a real struct to avoid complex setup and potential crashes from png_error,
    // as the other functions cover the core logic. 
    // Alternatively, we can try to initialize a minimal struct if headers allow,
    // but without error handlers, png_get_uint_31 might abort.
    // Let's try to initialize a minimal png_struct for testing valid paths.
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr != NULL) {
        // Try to call png_get_uint_31 with the buffer
        // We pass the buffer from input or the saved buffer
        const png_byte *getBuf31 = (Size >= 4) ? Data : buf32;
        
        // We need to handle the potential png_error call inside png_get_uint_31.
        // Since we don't have a custom error handler registered, it might abort.
        // To prevent the fuzzer from crashing on valid test cases (where value > 2^31-1),
        // we can only call this if the value is known to be safe, or accept the crash 
        // as a finding of invalid input.
        // However, for a robust fuzz driver, we should avoid crashing on expected logic.
        // Let's check the value first.
        uint32_t testVal = png_get_uint_32(getBuf31);
        if (testVal <= PNG_UINT_31_MAX) {
            png_get_uint_31(png_ptr, getBuf31);
        }
        
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }

    return 0;
}