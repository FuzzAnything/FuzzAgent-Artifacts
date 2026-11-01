// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_first_frame_is_hidden at pngget.c:1509:1 in png.h
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_next_frame_dispose_op at pngget.c:1489:1 in png.h
// png_get_next_frame_blend_op at pngget.c:1499:1 in png.h
#include <png.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

static void fuzz_error(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    // libpng handles longjmp internally via png_longjmp, we just return to let the caller handle it
    // or we can trigger a longjmp if we had access to the internal struct, but we don't.
    // Instead, we rely on the fact that png_set_error_fn is called before reading.
    // To make this work safely without accessing private members, we will not implement a custom
    // longjmp here but rely on the default behavior or let the program exit on error for fuzzing.
    // However, for a robust fuzzer, we usually let libpng call its own longjmp.
    // Since we cannot access jmpbuf, we will just exit or abort in a real scenario, 
    // but for fuzzing, we can just return and let the test fail gracefully.
    // Actually, the standard way is to let libpng handle the error.
    // We will just print and return, but png expects a longjmp.
    // Since we can't access the struct, we will use the standard abort for safety in this context
    // or simply not implement a custom error handler that tries to longjmp.
    // Let's use the default error handler behavior by not setting a custom one that crashes.
    // But we need to set one to avoid segfaults on invalid input if libpng tries to print.
    // A common pattern for fuzzing when you can't access the struct is to just abort.
    abort();
}

static void fuzz_warning(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

static void fuzz_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void *fuzz_malloc(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

static void fuzz_free(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_byte dispose_op, blend_op, compression_type, color_type, first_frame_hidden;
    png_uint_32 num_exif = 0;
    png_byte *exif = NULL;

    // Initialize png_struct
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, fuzz_error, fuzz_warning);
    if (!png_ptr) return 0;

    // Initialize png_info
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set custom memory functions
    png_set_mem_fn(png_ptr, NULL, fuzz_malloc, fuzz_free);

    // Set read function (dummy)
    png_set_read_fn(png_ptr, NULL, fuzz_read_data);

    // Initialize minimal PNG header info using libpng setters to avoid direct struct access
    // This simulates having read the IHDR chunk
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Test png_get_compression_type
    compression_type = png_get_compression_type(png_ptr, info_ptr);
    (void)compression_type;

    // Test png_get_color_type
    color_type = png_get_color_type(png_ptr, info_ptr);
    (void)color_type;

    // Test png_get_next_frame_dispose_op (APNG specific, likely returns 0 if not set)
    dispose_op = png_get_next_frame_dispose_op(png_ptr, info_ptr);
    (void)dispose_op;

    // Test png_get_next_frame_blend_op (APNG specific)
    blend_op = png_get_next_frame_blend_op(png_ptr, info_ptr);
    (void)blend_op;

    // Test png_get_first_frame_is_hidden (APNG specific)
    first_frame_hidden = png_get_first_frame_is_hidden(png_ptr, info_ptr);
    (void)first_frame_hidden;

    // Test png_get_eXIf_1
    png_uint_32 ret = png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &exif);
    (void)ret;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}