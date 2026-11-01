// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <setjmp.h>

// Custom error handler to prevent libpng from aborting the fuzzer
// We use a static jmp_buf to handle the longjmp from png_error
static jmp_buf fuzzer_jmp_buf;

static void fuzzer_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Instead of aborting, we jump back to the fuzzer
    longjmp(fuzzer_jmp_buf, 1);
}

static void fuzzer_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void fuzzer_test_save_uint_16(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    png_byte buf[2];
    unsigned int value = 0;
    if (Size >= 4) {
        value = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
    } else if (Size >= 1) {
        value = Data[0];
    }
    png_save_uint_16(buf, value);
}

static void fuzzer_test_save_int_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    png_byte buf[4];
    png_int_32 value = 0;
    if (Size >= 4) {
        value = (png_int_32)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
    }
    png_save_int_32(buf, value);
}

static void fuzzer_test_save_uint_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    png_byte buf[4];
    png_uint_32 value = 0;
    if (Size >= 4) {
        value = (png_uint_32)((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]);
    }
    png_save_uint_32(buf, value);
}

static void fuzzer_test_get_uint_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    const png_byte *buf = Data;
    png_uint_32 result = png_get_uint_32(buf);
    (void)result;
}

static void fuzzer_test_get_int_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    const png_byte *buf = Data;
    png_int_32 result = png_get_int_32(buf);
    (void)result;
}

static void fuzzer_test_get_uint_31(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    const png_byte *buf = Data;

    // Set up the jump buffer before creating the struct
    if (setjmp(fuzzer_jmp_buf) != 0) {
        // We jumped here from png_error, just return
        return;
    }

    // Create a minimal png_struct with custom error handlers
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, fuzzer_png_error, fuzzer_png_warning);
    if (!png_ptr) return;

    // Call the function which might trigger png_error if value > PNG_UINT_31_MAX
    // The custom error handler will trigger longjmp back to setjmp
    png_uint_32 result = png_get_uint_31(png_ptr, buf);
    (void)result;

    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test png_save_uint_16
    fuzzer_test_save_uint_16(Data, Size);

    // Test png_save_int_32
    fuzzer_test_save_int_32(Data, Size);

    // Test png_save_uint_32
    fuzzer_test_save_uint_32(Data, Size);

    // Test png_get_uint_32
    fuzzer_test_get_uint_32(Data, Size);

    // Test png_get_int_32
    fuzzer_test_get_int_32(Data, Size);

    // Test png_get_uint_31
    fuzzer_test_get_uint_31(Data, Size);

    return 0;
}