// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
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
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
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
#include <cstdio>

static void safe_png_destroy(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
}

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Create a temporary file to hold the fuzzed data
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        safe_png_destroy(png_ptr, info_ptr);
        return 0;
    }

    // Open file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        safe_png_destroy(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read info (may trigger errors if data is malformed)
    png_read_info(png_ptr, info_ptr);
    fclose(fp);

    // Fuzz target: png_get_gAMA_fixed
    png_fixed_point gamma_val = 0;
    png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_val);
    png_get_gAMA_fixed(png_ptr, info_ptr, NULL); // Test NULL pointer handling

    // Fuzz target: png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);

    // Fuzz target: png_get_cLLI_fixed
    png_uint_32 clli_flags = png_get_cLLI_fixed(png_ptr, info_ptr, NULL, NULL);
    png_uint_32 max_cll, max_fall;
    clli_flags = png_get_cLLI_fixed(png_ptr, info_ptr, &max_cll, &max_fall);

    // Fuzz target: png_set_cLLI_fixed
    // Try setting with valid and potentially invalid values
    png_set_cLLI_fixed(png_ptr, info_ptr, 10000, 20000); // Valid
    png_set_cLLI_fixed(png_ptr, info_ptr, 0x7FFFFFFF, 0x7FFFFFFF); // Max value
    png_set_cLLI_fixed(png_ptr, info_ptr, 0, 0); // Zero

    // Fuzz target: png_get_sCAL_fixed
    int unit = 0;
    png_fixed_point width = 0, height = 0;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);
    png_get_sCAL_fixed(png_ptr, info_ptr, NULL, NULL, NULL); // Test NULL handling

    // Cleanup
    safe_png_destroy(png_ptr, info_ptr);
    
    // Remove dummy file
    remove(filename);

    return 0;
}