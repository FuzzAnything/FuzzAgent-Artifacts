// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_add_alpha at pngtrans.c:204:1 in png.h
// png_set_filler at pngtrans.c:119:1 in png.h
// png_set_user_transform_info at pngtrans.c:805:1 in png.h
// png_set_packswap at pngtrans.c:69:1 in png.h
// png_set_tRNS_to_alpha at pngrtran.c:969:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        } else {
            // Write a minimal valid PNG header if input is empty to avoid immediate crash
            const uint8_t minimal_png[] = {
                0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A,
                0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52,
                0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
                0x08, 0x02, 0x00, 0x00, 0x00, 0x90, 0x77, 0x53,
                0xDE, 0x00, 0x00, 0x00, 0x0C, 0x49, 0x44, 0x41,
                0x54, 0x08, 0xD7, 0x63, 0xF8, 0xFF, 0xFF, 0x3F,
                0x00, 0x05, 0xFE, 0x02, 0xFE, 0xDC, 0xCC, 0x59,
                0xE7, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E,
                0x44, 0xAE, 0x42, 0x60, 0x82
            };
            fwrite(minimal_png, 1, sizeof(minimal_png), f);
        }
        fclose(f);
    }
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input to dummy file
    write_dummy_png(Data, Size);

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    FILE *fp = NULL;

    // Create read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    // Create info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        cleanup_png(png_ptr, NULL);
        return 0;
    }

    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        if (fp) fclose(fp);
        return 0;
    }

    // Open file
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read signature
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;

    // Read info up to IHDR
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        png_read_info(png_ptr, info_ptr);
    } else {
        fclose(fp);
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Get IHDR data
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // Fuzz png_set_add_alpha
    png_uint_32 filler = Data[0] | ((Data[1] << 8) & 0xFFFF);
    int flags = Data[2] & 0x0F; // Limit flags to reasonable range
    png_set_add_alpha(png_ptr, filler, flags);

    // Fuzz png_set_filler
    png_set_filler(png_ptr, filler, flags);

    // Fuzz png_set_user_transform_info
    void *user_transform_ptr = (void*)(uintptr_t)(Data[3] << 16 | Data[4] << 8 | Data[5]);
    int user_transform_depth = Data[6] & 0x0F;
    int user_transform_channels = Data[7] & 0x0F;
    png_set_user_transform_info(png_ptr, user_transform_ptr, user_transform_depth, user_transform_channels);

    // Fuzz png_set_packswap
    png_set_packswap(png_ptr);

    // Fuzz png_set_tRNS_to_alpha
    png_set_tRNS_to_alpha(png_ptr);

    // Cleanup
    fclose(fp);
    cleanup_png(png_ptr, info_ptr);

    return 0;
}