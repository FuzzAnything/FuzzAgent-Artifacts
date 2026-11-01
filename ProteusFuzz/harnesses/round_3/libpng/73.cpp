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
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create read struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }

    // Create info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Use setjmp to handle libpng errors safely
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Extract values from input data for fuzzing parameters
    png_uint_32 mng_features = 0;
    png_uint_32 user_width_max = 0;
    png_uint_32 user_height_max = 0;
    png_alloc_size_t chunk_malloc_max = 0;
    png_uint_32 chunk_cache_max = 0;

    if (Size >= 4) {
        mng_features = Data[0] | (static_cast<png_uint_32>(Data[1]) << 8) | 
                       (static_cast<png_uint_32>(Data[2]) << 16) | (static_cast<png_uint_32>(Data[3]) << 24);
    }
    if (Size >= 8) {
        user_width_max = Data[4] | (static_cast<png_uint_32>(Data[5]) << 8) | 
                         (static_cast<png_uint_32>(Data[6]) << 16) | (static_cast<png_uint_32>(Data[7]) << 24);
    }
    if (Size >= 12) {
        user_height_max = Data[8] | (static_cast<png_uint_32>(Data[9]) << 8) | 
                          (static_cast<png_uint_32>(Data[10]) << 16) | (static_cast<png_uint_32>(Data[11]) << 24);
    }
    if (Size >= 16) {
        chunk_malloc_max = Data[12] | (static_cast<png_alloc_size_t>(Data[13]) << 8) | 
                           (static_cast<png_alloc_size_t>(Data[14]) << 16) | (static_cast<png_alloc_size_t>(Data[15]) << 24);
    }
    if (Size >= 20) {
        chunk_cache_max = Data[16] | (static_cast<png_uint_32>(Data[17]) << 8) | 
                          (static_cast<png_uint_32>(Data[18]) << 16) | (static_cast<png_uint_32>(Data[19]) << 24);
    }

    // Call png_permit_mng_features
    png_uint_32 permitted = png_permit_mng_features(png_ptr, mng_features);
    (void)permitted; // Suppress unused variable warning

    // Call png_set_user_limits
    png_set_user_limits(png_ptr, user_width_max, user_height_max);

    // Call png_set_chunk_cache_max
    png_set_chunk_cache_max(png_ptr, chunk_cache_max);

    // Call png_set_chunk_malloc_max
    png_set_chunk_malloc_max(png_ptr, chunk_malloc_max);

    // Call png_get_user_height_max
    png_uint_32 height_max = png_get_user_height_max(png_ptr);
    (void)height_max; // Suppress unused variable warning

    // Call png_get_user_chunk_ptr
    void *user_chunk_ptr = png_get_user_chunk_ptr(png_ptr);
    (void)user_chunk_ptr; // Suppress unused variable warning

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}