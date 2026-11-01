// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_get_user_height_max at pngget.c:1311:1 in png.h
// png_get_user_chunk_ptr at pngget.c:1272:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_permit_mng_features at pngset.c:1622:1 in png.h
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
#include <cstddef>
#include <vector>

static void dummy_error_fn(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, const char *warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void *dummy_malloc_fn(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    if (size == 0) return nullptr;
    void *ptr = malloc(size);
    if (ptr) memset(ptr, 0, size);
    return ptr;
}

static void dummy_free_fn(png_structp png_ptr, void *ptr) {
    (void)png_ptr;
    free(ptr);
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Create read structure with custom callbacks
    png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        dummy_error_fn,
        dummy_warning_fn
    );

    if (!png_ptr) return 0;

    // Set custom memory and read functions
    png_set_mem_fn(png_ptr, nullptr, dummy_malloc_fn, dummy_free_fn);
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Use input data to drive fuzzing parameters
    uint32_t mng_features = 0;
    uint32_t width_max = 0;
    uint32_t height_max = 0;
    uint32_t chunk_cache_max = 0;
    uint32_t chunk_malloc_max = 0;
    void *user_chunk_ptr = nullptr;

    if (Size >= 4) {
        mng_features = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
        if (Size >= 8) {
            width_max = Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24);
            if (Size >= 12) {
                height_max = Data[8] | (Data[9] << 8) | (Data[10] << 16) | (Data[11] << 24);
                if (Size >= 16) {
                    chunk_cache_max = Data[12] | (Data[13] << 8) | (Data[14] << 16) | (Data[15] << 24);
                    if (Size >= 20) {
                        chunk_malloc_max = Data[16] | (Data[17] << 8) | (Data[18] << 16) | (Data[19] << 24);
                    }
                }
            }
        }
    }

    // Fuzz png_permit_mng_features
    png_uint_32 permitted = png_permit_mng_features(png_ptr, mng_features);
    (void)permitted;

    // Fuzz png_set_chunk_malloc_max
    png_set_chunk_malloc_max(png_ptr, chunk_malloc_max);

    // Fuzz png_set_chunk_cache_max
    png_set_chunk_cache_max(png_ptr, chunk_cache_max);

    // Fuzz png_set_user_limits
    png_set_user_limits(png_ptr, width_max, height_max);

    // Fuzz png_get_user_height_max
    png_uint_32 height_limit = png_get_user_height_max(png_ptr);
    (void)height_limit;

    // Note: png_set_user_chunk_ptr is not available in standard libpng API for setting,
    // but png_get_user_chunk_ptr is available. We skip setting to avoid compilation error.
    // Fuzz png_get_user_chunk_ptr (will return NULL since not set)
    void *retrieved_ptr = png_get_user_chunk_ptr(png_ptr);
    (void)retrieved_ptr;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}