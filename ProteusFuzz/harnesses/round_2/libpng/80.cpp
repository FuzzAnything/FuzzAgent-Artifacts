// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstring>
#include <cstddef>
#include <setjmp.h>

// Global buffer to hold the longjmp environment
static jmp_buf fuzz_jmp_buf;

// Custom error handler that triggers a longjmp instead of aborting
static void fuzz_error_handler(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    longjmp(fuzz_jmp_buf, 1);
}

// Custom warning handler
static void fuzz_warning_handler(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    const png_byte *read_buf = Data;
    png_byte write_buf[8];
    memset(write_buf, 0, sizeof(write_buf));

    // 1. Test png_get_uint_32
    png_uint_32 val32 = png_get_uint_32(read_buf);
    (void)val32;

    // 2. Test png_get_int_32
    png_int_32 signed_val = png_get_int_32(read_buf);
    (void)signed_val;

    // 3. Test png_save_uint_32
    png_save_uint_32(write_buf, val32);

    // 4. Test png_save_int_32
    png_save_int_32(write_buf + 4, signed_val);

    // 5. Test png_save_uint_16
    png_save_uint_16(write_buf, static_cast<unsigned int>(val32 & 0xFFFF));

    // 6. Test png_get_uint_31
    // Initialize png_struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    
    if (png_ptr) {
        // Set custom error handlers
        png_set_error_fn(png_ptr, NULL, fuzz_error_handler, fuzz_warning_handler);
        
        // Setup longjmp to catch errors
        if (setjmp(fuzz_jmp_buf) == 0) {
            // Safe to call; if it errors, it jumps back here
            png_get_uint_31(png_ptr, read_buf);
        }
        // If we return here after longjmp, we just continue or clean up
        
        // Clean up
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }

    return 0;
}