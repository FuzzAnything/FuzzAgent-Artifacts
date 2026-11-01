// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_row at pngread.c:376:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_invert_mono at pngtrans.c:249:1 in png.h
// png_set_user_transform_info at pngtrans.c:805:1 in png.h
// png_set_add_alpha at pngtrans.c:204:1 in png.h
// png_set_filler at pngtrans.c:119:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_png_read(const uint8_t *data, size_t size) {
    if (size < 8) return;

    // Create a temporary file for the fuzzer input
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);

    // Open the file for reading
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) return;

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    // Check for valid PNG signature
    png_byte signature[8];
    if (fread(signature, 1, 8, fp) != 8 || png_sig_cmp(signature, 0, 8)) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    // Read the IHDR chunk
    png_read_info(png_ptr, info_ptr);

    // Target API: png_set_invert_mono
    png_set_invert_mono(png_ptr);

    // Target API: png_set_user_transform_info
    // Use a dummy pointer and values derived from input data if possible
    void *user_ptr = (void *)(size > 0 ? (intptr_t)data : 0);
    int depth = (size > 1) ? (data[0] % 16) : 8;
    int channels = (size > 2) ? (data[1] % 4) : 3;
    png_set_user_transform_info(png_ptr, user_ptr, depth, channels);

    // Target API: png_set_add_alpha
    png_uint_32 filler = (size > 3) ? (png_uint_32)data[2] : 255;
    int flags = (size > 4) ? (data[3] % 2) : 0;
    png_set_add_alpha(png_ptr, filler, flags);

    // Target API: png_set_filler
    png_set_filler(png_ptr, filler, flags);

    // Target API: png_set_gray_to_rgb
    png_set_gray_to_rgb(png_ptr);

    // Target API: png_get_IHDR
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // Prepare for reading image data
    png_read_update_info(png_ptr, info_ptr);

    // Allocate row buffer
    png_bytep row = (png_bytep)malloc(png_get_rowbytes(png_ptr, info_ptr));
    if (!row) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    // Read the image data
    for (png_uint_32 y = 0; y < height; ++y) {
        png_read_row(png_ptr, row, NULL);
    }

    // Cleanup
    png_read_end(png_ptr, NULL);
    free(row);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_png_read(Data, Size);

    return 0;
}