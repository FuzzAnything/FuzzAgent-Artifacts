// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_permit_mng_features at pngset.c:1622:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_get_user_height_max at pngget.c:1311:1 in png.h
// png_get_user_chunk_ptr at pngget.c:1272:1 in png.h
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

static void png_fuzz_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void png_fuzz_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
                                                 png_fuzz_error_callback, png_fuzz_warning_callback);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Fuzz png_permit_mng_features
    png_uint_32 mng_features = 0;
    if (Size > 0) mng_features = Data[0];
    if (Size > 1) mng_features |= ((png_uint_32)Data[1] << 8);
    if (Size > 2) mng_features |= ((png_uint_32)Data[2] << 16);
    if (Size > 3) mng_features |= ((png_uint_32)Data[3] << 24);

    png_uint_32 permitted = png_permit_mng_features(png_ptr, mng_features);
    (void)permitted;

    // Fuzz png_set_chunk_malloc_max
    png_alloc_size_t malloc_max = 0;
    if (Size > 4) malloc_max = Data[4];
    if (Size > 5) malloc_max |= ((png_alloc_size_t)Data[5] << 8);
    if (Size > 6) malloc_max |= ((png_alloc_size_t)Data[6] << 16);
    if (Size > 7) malloc_max |= ((png_alloc_size_t)Data[7] << 24);
    png_set_chunk_malloc_max(png_ptr, malloc_max);

    // Fuzz png_set_chunk_cache_max
    png_uint_32 cache_max = 0;
    if (Size > 8) cache_max = Data[8];
    if (Size > 9) cache_max |= ((png_uint_32)Data[9] << 8);
    if (Size > 10) cache_max |= ((png_uint_32)Data[10] << 16);
    if (Size > 11) cache_max |= ((png_uint_32)Data[11] << 24);
    png_set_chunk_cache_max(png_ptr, cache_max);

    // Fuzz png_set_user_limits
    png_uint_32 width_max = 0;
    if (Size > 12) width_max = Data[12];
    if (Size > 13) width_max |= ((png_uint_32)Data[13] << 8);
    if (Size > 14) width_max |= ((png_uint_32)Data[14] << 16);
    if (Size > 15) width_max |= ((png_uint_32)Data[15] << 24);

    png_uint_32 height_max = 0;
    if (Size > 16) height_max = Data[16];
    if (Size > 17) height_max |= ((png_uint_32)Data[17] << 8);
    if (Size > 18) height_max |= ((png_uint_32)Data[18] << 16);
    if (Size > 19) height_max |= ((png_uint_32)Data[19] << 24);

    png_set_user_limits(png_ptr, width_max, height_max);

    // Fuzz png_get_user_height_max
    png_uint_32 retrieved_height = png_get_user_height_max(png_ptr);
    (void)retrieved_height;

    // Fuzz png_get_user_chunk_ptr
    void *user_ptr = png_get_user_chunk_ptr(png_ptr);
    (void)user_ptr;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 0;
}