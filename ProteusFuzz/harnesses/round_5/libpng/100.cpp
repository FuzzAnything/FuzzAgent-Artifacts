// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>

// Custom error handler to prevent libpng from aborting during fuzzing.
// We capture the error and return without triggering longjmp or abort.
static void custom_error_handler(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    // Do nothing. The fuzzer will continue execution.
}

// Custom warning handler
static void custom_warning_handler(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare buffers for save operations
    png_byte save_buf_32[4];
    png_byte save_buf_16[2];
    
    // Initialize png_struct for png_get_uint_31
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }

    // Set custom error handler to prevent abort/longjmp on invalid data
    // We pass nullptr for the error pointer since we are not using setjmp/longjmp
    png_set_error_fn(png_ptr, nullptr, custom_error_handler, custom_warning_handler);

    // Test png_get_uint_32
    // Requires at least 4 bytes
    png_uint_32 val_uint32 = png_get_uint_32(Data);
    (void)val_uint32;

    // Test png_save_uint_32
    png_save_uint_32(save_buf_32, val_uint32);

    // Test png_get_int_32
    png_int_32 val_int32 = png_get_int_32(Data);
    (void)val_int32;

    // Test png_save_int_32
    png_save_int_32(save_buf_32, val_int32);

    // Test png_save_uint_16
    // Use first 2 bytes as input value (masked to 16 bits)
    unsigned int val_16 = (Data[0] << 8) | Data[1];
    png_save_uint_16(save_buf_16, val_16);

    // Test png_get_uint_31
    // This function calls png_error if value > PNG_UINT_31_MAX.
    // Our custom error handler prevents the abort/longjmp.
    png_uint_32 val_uint31 = png_get_uint_31(png_ptr, Data);
    (void)val_uint31;

    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    return 0;
}