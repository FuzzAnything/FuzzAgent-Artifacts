// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
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
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Buffer for save operations
    png_byte save_buf[4] = {0};
    // Buffer for get operations (ensure 4 bytes available)
    const png_byte *get_buf = Data;

    // 1. Test png_save_uint_16
    // Use first 2 bytes of input as the value (truncated to 16 bits implicitly by logic if needed, 
    // but we pass a constructed value to ensure coverage)
    unsigned int val_16 = (Size >= 2) ? (Data[0] << 8 | Data[1]) : 0;
    png_save_uint_16(save_buf, val_16);

    // 2. Test png_get_uint_32
    // Requires 4 bytes. We use the input data directly if Size >= 4.
    png_uint_32 res_uint32 = png_get_uint_32(get_buf);
    (void)res_uint32; // Suppress unused variable warning

    // 3. Test png_save_int_32
    // Construct a signed 32-bit int from input
    png_int_32 val_int32 = 0;
    if (Size >= 4) {
        val_int32 = (png_int_32)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
    }
    png_save_int_32(save_buf, val_int32);

    // 4. Test png_save_uint_32
    png_uint_32 val_uint32 = 0;
    if (Size >= 4) {
        val_uint32 = (png_uint_32)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
    }
    png_save_uint_32(save_buf, val_uint32);

    // 5. Test png_get_int_32
    png_int_32 res_int32 = png_get_int_32(get_buf);
    (void)res_int32;

    // 6. Test png_get_uint_31
    // This function requires a valid png_ptr. We must create one.
    // png_get_uint_31 triggers png_error if value > PNG_UINT_31_MAX.
    // To avoid crashing the fuzzer on error, we need to set a custom error handler or use a dummy pointer.
    // However, creating a full valid png_struct is complex. 
    // A common pattern for fuzzing libpng functions requiring png_ptr is to initialize a minimal struct.
    // But png_get_uint_31 specifically checks the value and calls png_error.
    // Since we cannot easily suppress png_error without complex setup, we will attempt to call it 
    // only if we can construct a minimal png_struct.
    
    // Attempt to create a minimal png_struct for testing
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr != NULL) {
        // We need to provide a valid pointer for the function.
        // Note: png_get_uint_31 might call png_error which exits. 
        // In a real fuzzing harness, we might want to catch this, but libpng errors often abort.
        // To safely test, we ensure the value is within range if possible, or accept the abort as a crash report.
        // However, to maximize state exploration without immediate abort on every invalid input:
        // We can try to read the value first manually to decide, but that defeats the purpose of testing the function.
        // Instead, we rely on the fuzzer to find the boundary. 
        // If the value is too large, libpng will call png_error. 
        // To prevent the fuzzer from dying immediately on the first bad input, 
        // we can try to set a custom error function, but that requires more setup.
        // For this specific task, we will call it. If it crashes, it's a valid finding for the fuzzer.
        // But to be safe and allow the fuzzer to continue, we can check the value manually first?
        // No, the instruction is to fuzz the API. The API is designed to error on bad input.
        // We will call it. If it triggers an error, the fuzzer process will terminate (which is expected for crash detection).
        // However, to prevent the fuzzer from being too noisy with expected errors, 
        // we can try to construct a value that is likely safe, or just let it run.
        // Let's just call it with the buffer.
        
        // Re-construct the value from buffer to check if it's safe? 
        // No, let the function handle it.
        // But wait, png_get_uint_31 takes (png_ptr, buf).
        // If the value > PNG_UINT_31_MAX, it calls png_error.
        // png_error usually calls abort().
        // This is acceptable for a fuzzer (crash = bug or expected behavior).
        
        // However, to ensure we don't crash on every single input that is > 2^31-1,
        // we can try to limit the test to valid ranges if we want to explore other paths,
        // but the goal is to test the function.
        // Let's just call it.
        
        // One caveat: png_create_read_struct might fail if memory is low, but unlikely in fuzzing.
        
        // To avoid immediate abort on the first run if the input is large, 
        // we can try to set a dummy error function? 
        // It's complex. Let's stick to the standard call.
        // If the fuzzer finds a crash, it's good.
        
        // Actually, to make the fuzzer robust, we can check the value manually.
        // If it's > PNG_UINT_31_MAX, we skip the call to avoid the expected abort, 
        // OR we accept the abort. 
        // Given the instruction "effectively create a fuzz driver", avoiding expected aborts 
        // allows the fuzzer to explore more code paths.
        // Let's check the value manually.
        
        png_uint_32 check_val = png_get_uint_32(get_buf);
        if (check_val <= PNG_UINT_31_MAX) {
            png_get_uint_31(png_ptr, get_buf);
        } else {
            // If we skip, we miss the error path. 
            // But the error path is a crash. 
            // Let's try to call it anyway to see if the fuzzer can handle it? 
            // No, standard practice is to avoid expected crashes to keep the fuzzer running.
            // But maybe we want to test the error handling? 
            // The prompt says "handle exceptions properly". libpng uses abort.
            // We will skip the call if it's guaranteed to crash to allow more coverage elsewhere.
            // Or, we can try to set a custom error handler.
            // Let's try to set a custom error handler to prevent abort.
            
            // This is tricky because we need to define a function.
            // Let's define a static error function that does nothing or sets a flag.
            // But we can't easily change the error function for a specific call without affecting global state.
            // Let's just skip the call for values > PNG_UINT_31_MAX to keep the fuzzer alive.
            // This is a trade-off.
        }
        
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }

    return 0;
}