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

extern "C" int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Buffer for save operations
    png_byte save_buf[4] = {0};
    
    // Buffer for get operations (ensure we have at least 4 bytes)
    const png_byte *get_buf = Data;
    
    // 1. Test png_save_uint_16
    // Extract 2 bytes from input to form a 16-bit value
    uint16_t val16 = (Size >= 2) ? ((static_cast<uint16_t>(Data[0]) << 8) | Data[1]) : 0;
    png_save_uint_16(save_buf, static_cast<unsigned int>(val16));
    
    // 2. Test png_save_uint_32
    // Extract 4 bytes from input to form a 32-bit value
    png_uint_32 val32 = 0;
    if (Size >= 4) {
        val32 = ((static_cast<png_uint_32>(Data[0]) << 24) |
                 (static_cast<png_uint_32>(Data[1]) << 16) |
                 (static_cast<png_uint_32>(Data[2]) << 8) |
                 static_cast<png_uint_32>(Data[3]));
    }
    png_save_uint_32(save_buf, val32);
    
    // 3. Test png_get_int_32
    png_int_32 res_int32 = png_get_int_32(get_buf);
    (void)res_int32; // Suppress unused variable warning
    
    // 4. Test png_get_uint_31 with pre-validation
    // We need a dummy png_struct for this function
    // Since we cannot easily create a valid png_struct without initialization,
    // and the task warns about error handlers, we will skip direct call if we can't safely mock.
    // However, the signature requires png_const_structrp. 
    // To avoid complex setup and potential aborts, we will only call if we can validate the value first.
    // But since we don't have a valid png_ptr, we cannot call png_get_uint_31 safely in a fuzzing context
    // without risking a crash if the library checks the pointer validity.
    // The prompt says: "Do not call png_get_uint_31 directly with raw, unvalidated fuzzing input if the custom error handler is configured to abort."
    // It also says to pre-validate. But it still needs a png_ptr.
    // Given the constraints of a simple fuzz driver and lack of constructor info, 
    // we will skip calling png_get_uint_31 to avoid undefined behavior from a null/invalid png_ptr,
    // as the primary goal is to test the logic of the other functions and avoid crashing the fuzzer.
    // If a valid png_ptr is required, the test would need to initialize a png_struct, which is complex.
    // We will focus on the other functions that do not require a png_struct.
    
    // 5. Test png_get_uint_32
    png_uint_32 res_uint32 = png_get_uint_32(get_buf);
    (void)res_uint32;
    
    // 6. Test png_save_int_32
    // Use the same 32-bit value, cast to signed
    png_int_32 val_signed = static_cast<png_int_32>(val32);
    png_save_int_32(save_buf, val_signed);
    
    return 0;
}