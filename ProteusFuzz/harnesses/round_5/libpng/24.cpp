// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_png at pngread.c:957:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_PLTE at pngget.c:1121:1 in png.h
// png_set_hIST at pngset.c:375:1 in png.h
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
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Initialize png_struct
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, error_handler, warning_handler);
    if (!png_ptr) return 0;

    // Initialize png_info
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling with longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Create a dummy file to read from
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Reopen for reading
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Check signature
    if (png_sig_cmp(Data, 0, Size < 8 ? Size : 8) == 0) {
        // Read info
        png_read_info(png_ptr, info_ptr);

        // Test png_read_update_info
        png_read_update_info(png_ptr, info_ptr);

        // Test png_get_palette_max
        int max_palette = png_get_palette_max(png_ptr, info_ptr);
        (void)max_palette;

        // Test png_set_hIST if palette exists
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_PLTE)) {
            // png_get_PLTE requires 4 arguments: png_ptr, info_ptr, &palette, &num_palette
            png_colorp palette = nullptr;
            int num_palette = 0;
            png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);
            
            if (num_palette > 0) {
                // Allocate histogram data
                png_uint_16 *hist = (png_uint_16 *)malloc(sizeof(png_uint_16) * num_palette);
                if (hist) {
                    // Fill with fuzz data
                    for (int i = 0; i < num_palette; ++i) {
                        hist[i] = (Size > (size_t)i) ? Data[i] : 0;
                    }
                    png_set_hIST(png_ptr, info_ptr, hist);
                    free(hist);
                }
            }
        }

        // Test png_read_png
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);

        // Test png_read_end
        png_read_end(png_ptr, info_ptr);
    }

    // Test png_destroy_info_struct
    png_destroy_info_struct(png_ptr, &info_ptr);

    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    fclose(fp);
    remove(dummy_file);

    return 0;
}