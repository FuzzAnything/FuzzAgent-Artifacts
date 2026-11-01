// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_set_add_alpha at pngtrans.c:204:1 in png.h
// png_set_filler at pngtrans.c:119:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_user_transform_info at pngtrans.c:805:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
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
#include <cstring>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input to dummy file for file-based reading
    write_dummy_file(Data, Size);

    // Initialize libpng read structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Use setjmp/longjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Open the dummy file
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Try to read the signature (optional, but helps detect valid PNGs)
    if (png_sig_cmp(Data, 0, Size < 8 ? Size : 8) != 0) {
        // Not a valid PNG signature, but we still try to process it for fuzzing
        // We skip reading the signature to allow libpng to handle malformed data
    } else {
        png_set_sig_bytes(png_ptr, 8);
    }

    // Read IHDR chunk
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_get_IHDR
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // Fuzz png_set_add_alpha
    png_uint_32 filler = 0;
    int flags = 0;
    if (Size >= 2) {
        filler = Data[0];
        flags = Data[1];
    }
    png_set_add_alpha(png_ptr, filler, flags);

    // Fuzz png_set_filler
    png_set_filler(png_ptr, filler, flags);

    // Fuzz png_set_gray_to_rgb
    png_set_gray_to_rgb(png_ptr);

    // Fuzz png_set_user_transform_info
    void *user_transform_ptr = (void*)Data;
    int user_transform_depth = (Size > 2) ? Data[2] : 8;
    int user_transform_channels = (Size > 3) ? Data[3] : 3;
    png_set_user_transform_info(png_ptr, user_transform_ptr, user_transform_depth, user_transform_channels);

    // Fuzz png_read_png with various transform flags
    int transforms = 0;
    if (Size >= 4) {
        transforms = Data[4];
    }
    png_read_png(png_ptr, info_ptr, transforms, NULL);

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}