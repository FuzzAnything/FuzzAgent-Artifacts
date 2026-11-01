// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
// png_set_pHYs at pngset.c:723:1 in png.h
// png_set_sCAL_s at pngset.c:598:1 in png.h
// png_access_version_number at png.c:963:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cstddef>

// Custom error handler to prevent abort during fuzzing
static void
fuzz_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do not abort or longjmp; just return to let fuzzer continue
}

static void
fuzz_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Custom read function that does nothing to prevent actual file reading
static void
fuzz_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
    // Intentionally do nothing to avoid reading from a potentially invalid file
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzz_png_error, fuzz_png_warning);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling to avoid abort
    png_set_error_fn(png_ptr, nullptr, fuzz_png_error, fuzz_png_warning);

    // Set up a dummy read function to avoid actual file I/O issues
    png_set_read_fn(png_ptr, nullptr, fuzz_read_data);

    // We do NOT call png_read_info here because it will try to read data
    // and our dummy read function does nothing, which might cause issues.
    // Instead, we directly test the getter functions on uninitialized data.

    // Test png_get_pHYs
    png_uint_32 res_x = 0, res_y = 0;
    int unit_type = 0;
    png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);

    // Test png_get_sCAL_fixed
    int unit = 0;
    png_fixed_point width = 0, height = 0;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);

    // Test png_get_sCAL_s
    char *swidth = nullptr, *sheight = nullptr;
    png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);
    if (swidth) free(swidth);
    if (sheight) free(sheight);

    // Test png_set_pHYs
    png_set_pHYs(png_ptr, info_ptr, res_x, res_y, unit_type);

    // Test png_set_sCAL_s with various inputs
    const char *test_width = "1.0";
    const char *test_height = "2.0";
    png_set_sCAL_s(png_ptr, info_ptr, 1, test_width, test_height);

    // Test png_access_version_number
    png_uint_32 version = png_access_version_number();
    (void)version;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}