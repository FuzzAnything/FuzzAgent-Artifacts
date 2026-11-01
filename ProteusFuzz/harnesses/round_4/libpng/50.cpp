// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Silently ignore errors to prevent abort during fuzzing
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Silently ignore warnings
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to a temporary file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    if (fwrite(Data, 1, Size, fp) != Size) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    // Reopen for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }

    // Create PNG read struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    // Set error handlers to prevent abort
    png_set_error_fn(png_ptr, NULL, dummy_error, dummy_warning);

    // Create PNG info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Use setjmp/longjmp to handle errors gracefully
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    // Try to read info (IHDR and other chunks before IDAT)
    png_read_info(png_ptr, info_ptr);

    // Test png_get_gAMA_fixed
    png_fixed_point file_gamma;
    png_get_gAMA_fixed(png_ptr, info_ptr, &file_gamma);

    // Test png_get_x_pixels_per_meter
    png_uint_32 x_pixels = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_pixels;

    // Test png_get_cLLI_fixed
    png_uint_32 maxCLL, maxFALL;
    png_get_cLLI_fixed(png_ptr, info_ptr, &maxCLL, &maxFALL);

    // Test png_get_sCAL_fixed
    int unit;
    png_fixed_point width, height;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);

    // Test setting cLLI_fixed with fuzzed values (clamped to safe range)
    if (Size >= 8) {
        png_fixed_point fuzz_maxCLL = *reinterpret_cast<const png_fixed_point*>(Data);
        png_fixed_point fuzz_maxFALL = *reinterpret_cast<const png_fixed_point*>(Data + 4);
        // Clamp to safe range to avoid triggering errors that might abort
        if (fuzz_maxCLL > 200000) fuzz_maxCLL = 200000;
        if (fuzz_maxFALL > 200000) fuzz_maxFALL = 200000;
        if (fuzz_maxCLL < 0) fuzz_maxCLL = 0;
        if (fuzz_maxFALL < 0) fuzz_maxFALL = 0;
        
        if (setjmp(png_jmpbuf(png_ptr)) == 0) {
            png_set_cLLI_fixed(png_ptr, info_ptr, fuzz_maxCLL, fuzz_maxFALL);
        }
    }

    // Test setting gAMA_fixed
    if (Size >= 4) {
        png_fixed_point fuzz_gamma = *reinterpret_cast<const png_fixed_point*>(Data);
        if (fuzz_gamma > 100000) fuzz_gamma = 100000; // Clamp to reasonable range
        if (fuzz_gamma < 0) fuzz_gamma = 0;
        
        if (setjmp(png_jmpbuf(png_ptr)) == 0) {
            png_set_gAMA_fixed(png_ptr, info_ptr, fuzz_gamma);
        }
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    // Clean up dummy file
    remove(filename);

    return 0;
}