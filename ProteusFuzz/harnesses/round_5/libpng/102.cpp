// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
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

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        png_destroy_info_struct(nullptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        fclose(fp);
        remove(filename);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read info (parses header and chunks)
    png_read_info(png_ptr, info_ptr);

    // --- Fuzzing Target 1: png_get_gAMA_fixed ---
    png_fixed_point int_file_gamma = 0;
    png_uint_32 gama_flag = png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma);
    (void)gama_flag;

    // --- Fuzzing Target 2: png_get_x_pixels_per_meter ---
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    // --- Fuzzing Target 3: png_get_cLLI_fixed ---
    // Corrected types based on error message: expects png_uint_32*
    png_uint_32 max_cll = 0;
    png_uint_32 max_fall = 0;
    #ifdef PNG_cLLI_SUPPORTED
    png_uint_32 clli_flag = png_get_cLLI_fixed(png_ptr, info_ptr, &max_cll, &max_fall);
    (void)clli_flag;
    #endif

    // --- Fuzzing Target 4: png_set_cLLI_fixed ---
    // Corrected types: expects png_uint_32 values
    if (Size >= 8) {
        png_uint_32 fuzz_cll = 0;
        png_uint_32 fuzz_fall = 0;
        
        // Safely copy data to avoid alignment issues
        if (Size >= 4) {
            memcpy(&fuzz_cll, Data, sizeof(png_uint_32));
        }
        if (Size >= 8) {
            memcpy(&fuzz_fall, Data + 4, sizeof(png_uint_32));
        }
        
        #ifdef PNG_cLLI_SUPPORTED
        png_set_cLLI_fixed(png_ptr, info_ptr, fuzz_cll, fuzz_fall);
        #endif
    }

    // --- Fuzzing Target 5: png_get_sCAL_fixed ---
    int unit = 0;
    png_fixed_point width = 0;
    png_fixed_point height = 0;
    png_uint_32 scal_flag = png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);
    (void)scal_flag;

    // Cleanup
    cleanup_png(png_ptr, info_ptr);
    fclose(fp);
    remove(filename);

    return 0;
}