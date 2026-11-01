// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_set_expand_gray_1_2_4_to_8 at pngrtran.c:957:1 in png.h
// png_set_tRNS_to_alpha at pngrtran.c:969:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
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
#include <cstddef>
#include <cstdio>
#include <cstring>

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to simulate input
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Initialize I/O
    fp = fopen(filename, "rb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }
    png_init_io(png_ptr, fp);

    // Try to read the signature and info
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);

    // Now invoke the target API functions on the valid png_struct
    // 1. png_set_expand
    png_set_expand(png_ptr);

    // 2. png_set_packing
    png_set_packing(png_ptr);

    // 3. png_set_scale_16
    png_set_scale_16(png_ptr);

    // 4. png_set_expand_gray_1_2_4_to_8
    png_set_expand_gray_1_2_4_to_8(png_ptr);

    // 5. png_set_tRNS_to_alpha
    png_set_tRNS_to_alpha(png_ptr);

    // 6. png_set_palette_to_rgb
    png_set_palette_to_rgb(png_ptr);

    fclose(fp);
    cleanup_png(png_ptr, info_ptr);

    return 0;
}