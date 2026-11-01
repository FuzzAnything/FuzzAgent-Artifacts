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
#include <cstddef>
#include <cstring>
#include <png.h>

extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Buffer for read operations (using input data)
    png_byte read_buf[4];
    std::memcpy(read_buf, Data, 4);

    // Buffer for write operations
    png_byte write_buf[4];
    png_byte write_buf16[2];

    // 1. Test png_get_uint_32
    // Reads 4 bytes as big-endian unsigned 32-bit integer
    png_uint_32 u32_val = png_get_uint_32(read_buf);
    (void)u32_val;

    // 2. Test png_save_uint_32
    // Writes a 32-bit value to buffer
    png_save_uint_32(write_buf, u32_val);

    // 3. Test png_get_int_32
    // Reads 4 bytes as big-endian signed 32-bit integer
    png_int_32 i32_val = png_get_int_32(read_buf);
    (void)i32_val;

    // 4. Test png_save_int_32
    // Writes a signed 32-bit value to buffer
    png_save_int_32(write_buf, i32_val);

    // 5. Test png_get_uint_31
    // Requires a valid png_struct pointer. Since we cannot easily create a valid
    // initialized png_struct without complex setup and error handling callbacks
    // that might crash in a fuzzer context if not perfectly set up, we skip this
    // specific call or mock it if we were doing full integration. 
    // However, to strictly follow the task of fuzzing the API, we attempt to
    // create a minimal context if possible, but png_get_uint_31 calls png_error
    // on failure which aborts. 
    // To avoid crashing the fuzzer on invalid data (which is expected), we 
    // typically avoid calling functions that trigger png_error unless we 
    // install a custom error handler. Since the prompt does not provide 
    // constructor methods or error handler setup instructions, and png_get_uint_31
    // is dangerous to call without a full valid PNG context, we omit this specific
    // call to ensure the fuzzer remains stable, or we would need to install a 
    // custom error handler. 
    // Given the constraints and the nature of fuzzing (avoiding crashes on 
    // expected invalid inputs), we skip png_get_uint_31 as it requires a 
    // fully initialized png_ptr which is not provided in the constructor list.
    
    // 6. Test png_save_uint_16
    // Writes a 16-bit value to buffer
    unsigned int u16_val = 0;
    if (Size >= 2) {
        u16_val = (Data[0] << 8) | Data[1];
    }
    png_save_uint_16(write_buf16, u16_val);

    // Additional exploration: vary the input buffer content for the read functions
    // by flipping bits or using different parts of the input if available
    if (Size >= 8) {
        std::memcpy(read_buf, Data + 4, 4);
        png_uint_32 u32_val2 = png_get_uint_32(read_buf);
        (void)u32_val2;
        
        png_int_32 i32_val2 = png_get_int_32(read_buf);
        (void)i32_val2;
    }

    return 0;
}