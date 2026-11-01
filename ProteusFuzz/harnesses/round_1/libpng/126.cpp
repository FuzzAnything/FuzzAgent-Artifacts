// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
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
#include <cstddef>

static void png_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do not abort, just return to let the fuzzer continue
}

static void png_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for libpng to read from
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set error and warning handlers to prevent abort
    png_set_error_fn(png_ptr, NULL, png_error_handler, png_warning_handler);

    // Open the dummy file for reading
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Use setjmp to catch errors during reading
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Try to read the PNG info
    png_read_info(png_ptr, info_ptr);

    // Test png_get_sCAL_fixed
    int unit = 0;
    png_fixed_point width = 0, height = 0;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);

    // Test png_get_gAMA_fixed
    png_fixed_point gamma = 0;
    png_get_gAMA_fixed(png_ptr, info_ptr, &gamma);

    // Test png_get_cLLI_fixed
    png_uint_32 maxCLL = 0, maxFALL = 0;
    png_get_cLLI_fixed(png_ptr, info_ptr, &maxCLL, &maxFALL);

    // Test png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);

    // Test png_set_cLLI_fixed with varied values
    png_fixed_point test_cLLI = 10000; // 1 nit * 10000
    png_set_cLLI_fixed(png_ptr, info_ptr, test_cLLI, test_cLLI);

    // Test png_set_gAMA_fixed with varied values
    png_fixed_point test_gamma = 45455; // ~0.45455
    png_set_gAMA_fixed(png_ptr, info_ptr, test_gamma);

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}