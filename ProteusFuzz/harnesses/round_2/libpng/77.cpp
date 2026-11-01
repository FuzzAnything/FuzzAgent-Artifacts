// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_get_uint_31 at pngrutil.c:41:1 in png.h
// png_save_uint_32 at pngwutil.c:41:1 in png.h
// png_save_int_32 at png.c:728:1 in png.h
// png_save_uint_16 at pngwutil.c:54:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <setjmp.h>

static void dummy_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void dummy_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_set_error_fn(png_ptr, NULL, dummy_error_handler, dummy_warning_handler);

    const png_byte *buf = Data;

    // Test png_get_uint_32
    png_uint_32 val32 = png_get_uint_32(buf);
    (void)val32;

    // Test png_get_int_32
    png_int_32 valInt32 = png_get_int_32(buf);
    (void)valInt32;

    // Test png_get_uint_31 with exception handling
    png_uint_32 val31 = 0;
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        val31 = png_get_uint_31(png_ptr, buf);
    }
    (void)val31;

    // Prepare buffers for save functions
    png_byte saveBuf32[4];
    png_byte saveBuf16[2];

    // Test png_save_uint_32
    png_save_uint_32(saveBuf32, val32);

    // Test png_save_int_32
    png_save_int_32(saveBuf32, valInt32);

    // Test png_save_uint_16
    unsigned int val16 = (Size > 2) ? (buf[0] << 8) | buf[1] : 0;
    png_save_uint_16(saveBuf16, val16);

    png_destroy_read_struct(&png_ptr, NULL, NULL);

    return 0;
}