// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_get_io_chunk_type at pngget.c:1340:1 in png.h
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
#include <cstring>
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare buffer for save operations
    png_byte save_buf[4];
    memset(save_buf, 0, sizeof(save_buf));

    // Test png_save_uint_32
    png_uint_32 val_uint = 0;
    if (Size >= 4) {
        val_uint = (png_uint_32)Data[0] | ((png_uint_32)Data[1] << 8) | 
                   ((png_uint_32)Data[2] << 16) | ((png_uint_32)Data[3] << 24);
    }
    png_save_uint_32(save_buf, val_uint);

    // Test png_save_int_32
    png_int_32 val_int = (png_int_32)val_uint;
    png_save_int_32(save_buf, val_int);

    // Test png_get_uint_32
    png_uint_32 retrieved_uint = png_get_uint_32(save_buf);

    // Test png_get_int_32
    png_int_32 retrieved_int = png_get_int_32(save_buf);

    // Test png_get_uint_31 requires a valid png_struct
    // We create a minimal png_struct for this test
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr) {
        // Test png_get_io_chunk_type
        // This function depends on internal state, so we call it to exercise the code path
        png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr);
        (void)chunk_type; // Suppress unused variable warning

        // Test png_get_uint_31
        // Ensure we have at least 4 bytes for the buffer
        if (Size >= 4) {
            // We use the input data directly as the buffer for reading
            // Note: png_get_uint_31 may call png_error if value > PNG_UINT_31_MAX
            // To prevent the fuzzer from crashing on error, we catch the error via setjmp
            // However, standard libpng error handling uses longjmp.
            // For fuzzing, we rely on the fact that if it errors, it exits.
            // To keep the fuzzer alive, we only call it if we are sure it won't error,
            // OR we accept that some inputs will crash the process (which is fine for coverage).
            // But to be safe and explore more states, we can construct a value <= PNG_UINT_31_MAX.
            
            // Let's try with the input buffer directly. If it errors, the process terminates.
            // To avoid immediate termination on every large value, we can mask the MSB.
            png_byte test_buf[4];
            memcpy(test_buf, Data, 4);
            test_buf[0] &= 0x7F; // Ensure value is <= 2^31 - 1
            
            png_uint_32 val_31 = png_get_uint_31(png_ptr, test_buf);
            (void)val_31;
        }

        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }

    return 0;
}