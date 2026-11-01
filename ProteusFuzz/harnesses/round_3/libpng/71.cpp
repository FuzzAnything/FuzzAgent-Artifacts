// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_permit_mng_features at pngset.c:1622:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
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
#include <cstdint>
#include <cstring>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, dummy_warning_fn);
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

    // Extract parameters from fuzz input
    uint32_t mng_features = Data[0];
    uint32_t width_max = (Size > 1) ? (Data[1] << 24 | (Size > 2 ? Data[2] : 0) << 16 | (Size > 3 ? Data[3] : 0) << 8 | (Size > 4 ? Data[4] : 0)) : 0;
    uint32_t height_max = (Size > 5) ? (Data[5] << 24 | (Size > 6 ? Data[6] : 0) << 16 | (Size > 7 ? Data[7] : 0) << 8 | (Size > 8 ? Data[8] : 0)) : 0;
    uint32_t chunk_cache_max = (Size > 9) ? (Data[9] << 24 | (Size > 10 ? Data[10] : 0) << 16 | (Size > 11 ? Data[11] : 0) << 8 | (Size > 12 ? Data[12] : 0)) : 0;
    png_alloc_size_t chunk_malloc_max = (Size > 13) ? (static_cast<png_alloc_size_t>(Data[13]) << 24 | (Size > 14 ? Data[14] : 0) << 16 | (Size > 15 ? Data[15] : 0) << 8 | (Size > 16 ? Data[16] : 0)) : 0;

    // Call target API functions
    png_permit_mng_features(png_ptr, mng_features);
    png_set_user_limits(png_ptr, width_max, height_max);
    png_set_chunk_cache_max(png_ptr, chunk_cache_max);
    png_set_chunk_malloc_max(png_ptr, chunk_malloc_max);

    // Retrieve values to ensure state changes are observed
    png_uint_32 retrieved_height = png_get_user_height_max(png_ptr);
    void* user_chunk_ptr = png_get_user_chunk_ptr(png_ptr);

    // Suppress unused variable warnings
    (void)retrieved_height;
    (void)user_chunk_ptr;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 0;
}