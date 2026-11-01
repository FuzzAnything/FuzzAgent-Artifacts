// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <vector>

static void fuzz_png_read_and_get_info(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }

    // Create a dummy file to read from
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }

    png_init_io(png_ptr, fp);

    // Read info
    png_read_info(png_ptr, info_ptr);

    // Test png_get_gAMA_fixed
    png_fixed_point gamma_val;
    png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_val);
    png_get_gAMA_fixed(png_ptr, info_ptr, nullptr);

    // Test png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);

    // Test png_get_cLLI_fixed
    png_uint_32 max_cll, max_fall;
    png_get_cLLI_fixed(png_ptr, info_ptr, &max_cll, &max_fall);
    png_get_cLLI_fixed(png_ptr, info_ptr, nullptr, nullptr);

    // Test png_set_cLLI_fixed with various values
    png_set_cLLI_fixed(png_ptr, info_ptr, 1000, 2000);
    png_set_cLLI_fixed(png_ptr, info_ptr, 0, 0);
    png_set_cLLI_fixed(png_ptr, info_ptr, 200000, 200000);
    // Try invalid large values (may trigger error)
    png_set_cLLI_fixed(png_ptr, info_ptr, 0x7FFFFFFF, 0x7FFFFFFF);

    // Test png_get_sCAL_fixed
    int unit;
    png_fixed_point width, height;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);
    png_get_sCAL_fixed(png_ptr, info_ptr, nullptr, nullptr, nullptr);

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
}

extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    fuzz_png_read_and_get_info(Data, Size);
    return 0;
}