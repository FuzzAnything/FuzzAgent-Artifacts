// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
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
#include <cstddef>
#include <cstring>
#include <png.h>

// Static helper to create a minimal valid png_struct for png_get_uint_31
static png_structp create_dummy_png_struct() {
    // Create a minimal png_struct. In a real scenario, this would be initialized with
    // png_create_read_struct, but for fuzzing the internal logic of png_get_uint_31,
    // we need a valid pointer. Since png_get_uint_31 checks for errors, we need a 
    // struct that doesn't immediately crash if accessed, but ideally we just need 
    // to pass a non-null pointer to satisfy the signature.
    // However, png_get_uint_31 calls png_error if the value is too high.
    // png_error calls longjmp. To prevent the fuzzer from crashing on longjmp,
    // we can try to set up a minimal environment or just rely on the fact that
    // if the value is valid, it returns. If invalid, it crashes.
    // For fuzzing, we want to explore the valid path mostly.
    // We will use png_create_read_struct with a dummy error pointer.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    return png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Buffer for 16-bit operations (2 bytes)
    png_byte buf16[2];
    // Buffer for 32-bit operations (4 bytes)
    png_byte buf32[4];

    // Extract data from input
    // We use the input data to generate values to test
    unsigned int val16 = 0;
    png_uint_32 val32 = 0;
    png_int_32 sval32 = 0;

    if (Size >= 2) {
        val16 = (Data[0] << 8) | Data[1];
    }
    if (Size >= 4) {
        val32 = ((png_uint_32)Data[0] << 24) | ((png_uint_32)Data[1] << 16) |
                ((png_uint_32)Data[2] << 8) | ((png_uint_32)Data[3]);
        // Reinterpret as signed for png_save_int_32 and png_get_int_32
        memcpy(&sval32, Data, sizeof(png_int_32));
    }

    // 1. Test png_save_uint_16
    // Writes 16-bit unsigned int to buf16 in big-endian
    png_save_uint_16(buf16, val16);

    // 2. Test png_get_uint_32
    // Reads 32-bit unsigned int from buf32 in big-endian
    // We populate buf32 with the input data first
    if (Size >= 4) {
        memcpy(buf32, Data, 4);
        png_uint_32 read_val32 = png_get_uint_32(buf32);
        (void)read_val32; // Suppress unused variable warning
    }

    // 3. Test png_save_int_32
    // Stores signed 32-bit int as 4 bytes in big-endian
    png_save_int_32(buf32, sval32);

    // 4. Test png_save_uint_32
    // Writes 32-bit unsigned int to buf32 in big-endian
    png_save_uint_32(buf32, val32);

    // 5. Test png_get_int_32
    // Safely converts 4-byte buffer to signed 32-bit int
    // We reuse buf32 which was just written by png_save_int_32 or png_save_uint_32
    // Let's write a specific pattern to test the conversion logic
    if (Size >= 4) {
        memcpy(buf32, Data, 4);
        png_int_32 read_sval32 = png_get_int_32(buf32);
        (void)read_sval32;
    }

    // 6. Test png_get_uint_31
    // This function requires a png_struct and checks for values > 2^31-1.
    // If the value is too large, it calls png_error which triggers a longjmp.
    // We need to handle this carefully. If the fuzzer input causes a longjmp,
    // the test case is considered "crashed" by the fuzzer framework unless we catch it.
    // However, standard libpng error handling is via longjmp.
    // To fuzz this safely without crashing the fuzzer process on every invalid input,
    // we can try to set up a minimal context, but png_error is hard to intercept cleanly
    // without a full read setup.
    // Alternative: Only call this if the value is guaranteed to be < PNG_UINT_31_MAX.
    // Or, wrap in a signal handler? No, longjmp is not a signal.
    // The best approach for libpng fuzzing of error-triggering functions is to let them crash
    // if they are truly invalid, as that is a bug (or expected behavior).
    // But png_get_uint_31 is designed to error on overflow.
    // Let's create the struct and try. If it longjmps, the fuzzer sees it as a crash.
    // To avoid constant crashes on high values, we can check the value first.
    
    png_structp png_ptr = create_dummy_png_struct();
    if (png_ptr) {
        if (Size >= 4) {
            // Check if the value is within safe range to avoid immediate longjmp
            // PNG_UINT_31_MAX is 0x7FFFFFFF
            png_uint_32 test_val = ((png_uint_32)Data[0] << 24) | ((png_uint_32)Data[1] << 16) |
                                   ((png_uint_32)Data[2] << 8) | ((png_uint_32)Data[3]);
            
            // Only call if the value is safe, or let it crash if it's a test of the error path.
            // For a robust fuzzer, we usually want to avoid crashing on expected errors.
            // However, the prompt asks to fuzz the API. If the API is designed to error,
            // we should probably test the valid path primarily to exercise the logic,
            // or rely on the fuzzer to find the boundary.
            // Let's try to call it. If it errors, it's a "crash" in the fuzzer's eyes.
            // To make this more useful, we can just call it.
            // But to prevent the fuzzer from getting stuck in a loop of crashing on high values,
            // we can check the high bit.
            if ((test_val & 0x80000000) == 0) {
                // Safe to call
                png_uint_32 result = png_get_uint_31(png_ptr, Data);
                (void)result;
            } else {
                // Value is > 31 bits. This will trigger png_error.
                // We skip this to avoid constant crashes, or we could let it crash once.
                // For a general fuzzer, skipping the known error path is often better to explore other states.
                // But let's assume we want to test the boundary.
                // We'll just skip it to keep the fuzzer running smoothly on valid inputs.
            }
        }
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }

    return 0;
}