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

static void safe_png_get_uint_31(png_const_structrp png_ptr, const png_byte *buf, png_uint_32 *out) {
    if (!png_ptr || !buf) return;
    // We cannot safely call png_get_uint_31 without a valid png_ptr that handles errors.
    // For fuzzing, we simulate the logic or skip if we cannot initialize a valid png_ptr.
    // Since we cannot easily create a valid png_ptr without complex setup and error handlers,
    // we will skip the actual call for this specific function in this driver to avoid crashes,
    // or we can try to initialize a minimal one if the library allows it without file I/O.
    // However, png_get_uint_31 calls png_error on overflow, which aborts.
    // To fuzz safely, we will manually check the value and only call if safe, 
    // or simply not call it if we can't guarantee the error handler is set.
    // Given the constraints, we will skip the actual API call for png_get_uint_31 
    // unless we can set up a dummy error handler.
    // Let's try to initialize a png_struct to allow the call, but handle the error.
    // Actually, creating a png_struct without a file is tricky for this specific function.
    // We will omit the call for png_get_uint_31 to prevent undefined behavior/crashes 
    // due to missing error handlers in a minimal fuzz harness, 
    // OR we can just not call it. 
    // Better approach: Don't call png_get_uint_31 directly as it requires a valid context 
    // that calls png_error. We will skip it to ensure stability.
    *out = 0;
}

extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Buffer for reading 32-bit integers
    const png_byte *buf32 = reinterpret_cast<const png_byte*>(Data);
    png_byte out32[4];
    png_uint_32 val32;
    png_int_32 val32s;

    // 1. png_get_uint_32
    val32 = png_get_uint_32(buf32);
    (void)val32; // Use value to avoid unused warning

    // 2. png_save_uint_32
    png_save_uint_32(out32, val32);

    // 3. png_get_int_32
    val32s = png_get_int_32(buf32);
    (void)val32s;

    // 4. png_save_int_32
    png_save_int_32(out32, val32s);

    // 5. png_get_uint_31
    // This function requires a valid png_ptr. Creating one is complex and risky for fuzzing
    // without a full error handler setup. We skip the actual call to avoid crashes.
    // If we were to call it, we would need:
    // png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    // if (!png_ptr) return 0;
    // png_infop info_ptr = png_create_info_struct(png_ptr);
    // if (!info_ptr) { png_destroy_read_struct(&png_ptr, NULL, NULL); return 0; }
    // if (setjmp(png_jmpbuf(png_ptr)) == 0) {
    //     val32 = png_get_uint_31(png_ptr, buf32);
    // }
    // png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    // Given the complexity and potential for setjmp/longjmp issues in fuzzers, we skip it.

    // 6. png_save_uint_16
    if (Size >= 2) {
        png_byte out16[2];
        unsigned int val16 = 0;
        // Construct a 16-bit value from input
        val16 = (static_cast<unsigned int>(Data[0]) << 8) | Data[1];
        png_save_uint_16(out16, val16);
        (void)out16;
    }

    return 0;
}