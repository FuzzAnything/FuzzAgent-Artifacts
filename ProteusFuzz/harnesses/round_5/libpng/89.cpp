// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_init_io at png.c:705:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_get_user_height_max at pngget.c:1311:1 in png.h
// png_permit_mng_features at pngset.c:1622:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_get_user_chunk_ptr at pngget.c:1272:1 in png.h
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
#include <cstring>
#include <fstream>

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        if (info_ptr) {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        } else {
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        }
    } else if (info_ptr) {
        // Should not happen in normal flow, but safe to ignore or handle if needed
    }
}

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    // Extract parameters from fuzz input
    png_uint_32 chunk_cache_max = 0;
    png_uint_32 width_max = 0;
    png_uint_32 height_max = 0;
    png_uint_32 mng_features = 0;
    png_alloc_size_t chunk_malloc_max = 0;

    if (Size >= 20) {
        std::memcpy(&chunk_cache_max, Data, sizeof(png_uint_32));
        std::memcpy(&width_max, Data + 4, sizeof(png_uint_32));
        std::memcpy(&height_max, Data + 8, sizeof(png_uint_32));
        std::memcpy(&mng_features, Data + 12, sizeof(png_uint_32));
        std::memcpy(&chunk_malloc_max, Data + 16, sizeof(png_alloc_size_t));
    }

    // Create a dummy file to simulate reading a PNG
    const char* dummy_file = "./dummy_file";
    std::ofstream out(dummy_file, std::ios::binary);
    if (out.is_open()) {
        // Write minimal valid PNG header (8 bytes) + some data
        // PNG signature: 8 bytes
        const uint8_t png_sig[] = {137, 80, 78, 71, 13, 10, 26, 10};
        out.write(reinterpret_cast<const char*>(png_sig), 8);
        // Write remaining data if available
        if (Size > 8) {
            out.write(reinterpret_cast<const char*>(Data + 8), Size - 8);
        }
        out.close();
    }

    // Initialize libpng read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set error handling to avoid aborts
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Test png_set_chunk_cache_max
    png_set_chunk_cache_max(png_ptr, chunk_cache_max);

    // Test png_set_user_limits
    png_set_user_limits(png_ptr, width_max, height_max);

    // Test png_get_user_height_max
    png_uint_32 retrieved_height_max = png_get_user_height_max(png_ptr);
    (void)retrieved_height_max; // Suppress unused variable warning

    // Test png_permit_mng_features
    png_uint_32 permitted_mng = png_permit_mng_features(png_ptr, mng_features);
    (void)permitted_mng;

    // Test png_set_chunk_malloc_max
    png_set_chunk_malloc_max(png_ptr, chunk_malloc_max);

    // Test png_get_user_chunk_ptr (should return NULL as we didn't set it)
    void* user_chunk_ptr = png_get_user_chunk_ptr(png_ptr);
    (void)user_chunk_ptr;

    // Attempt to initialize file reading (optional, to exercise more paths)
    FILE* fp = fopen(dummy_file, "rb");
    if (fp) {
        png_init_io(png_ptr, fp);
        // Do not read full info to avoid complex parsing; just test setup
        fclose(fp);
    }

    cleanup_png(png_ptr, info_ptr);

    return 0;
}