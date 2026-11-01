// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
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
#include <fstream>
#include <vector>

static void
fuzz_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void
fuzz_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int
LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input to a dummy file
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();

    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzz_error_handler, fuzz_warning_handler);
    if (!png_ptr) {
        return 0;
    }

    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Setup error handling with longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Initialize I/O
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Try to read info
    png_read_info(png_ptr, info_ptr);
    fclose(fp);

    // Test png_get_gAMA_fixed
    png_fixed_point int_file_gamma;
    png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma);
    png_get_gAMA_fixed(png_ptr, info_ptr, nullptr);

    // Test png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    // Test png_get_sCAL_fixed
    int unit;
    png_fixed_point width, height;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);
    png_get_sCAL_fixed(png_ptr, info_ptr, nullptr, &width, &height);
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, nullptr, &height);
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, nullptr);

    // Test png_get_cLLI_fixed
    // Note: Arguments are png_uint_32*, not png_fixed_point*
    png_uint_32 max_cll, max_fall;
    png_get_cLLI_fixed(png_ptr, info_ptr, &max_cll, &max_fall);
    png_get_cLLI_fixed(png_ptr, info_ptr, nullptr, &max_fall);
    png_get_cLLI_fixed(png_ptr, info_ptr, &max_cll, nullptr);

    // Re-open file for write test (if needed for setters)
    // Since setters are for writing, we need a write context.
    // However, the task focuses on fuzzing the provided APIs which are mostly getters.
    // png_set_cLLI_fixed is a setter, so we need a write context to test it safely.
    
    // Create PNG write structure to test png_set_cLLI_fixed
    png_structp write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzz_error_handler, fuzz_warning_handler);
    if (!write_png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    png_infop write_info_ptr = png_create_info_struct(write_png_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&write_png_ptr, nullptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(write_png_ptr))) {
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Initialize write I/O to dummy file
    fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    png_init_io(write_png_ptr, fp);

    // Test png_set_cLLI_fixed with various values
    png_set_cLLI_fixed(write_png_ptr, write_info_ptr, 0, 0);
    png_set_cLLI_fixed(write_png_ptr, write_info_ptr, 10000, 20000);
    png_set_cLLI_fixed(write_png_ptr, write_info_ptr, 200000, 200000);
    // Test with potentially large values (might trigger error if > 200,000)
    png_set_cLLI_fixed(write_png_ptr, write_info_ptr, 300000, 300000);

    fclose(fp);

    // Cleanup
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}