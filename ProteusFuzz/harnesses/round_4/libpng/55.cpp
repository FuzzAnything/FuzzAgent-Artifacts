// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_user_width_max at pngget.c:1305:1 in png.h
// png_get_user_height_max at pngget.c:1311:1 in png.h
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
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>

static void* malloc_safe(size_t size) {
    if (size == 0) return nullptr;
    void* ptr = malloc(size);
    if (!ptr) abort();
    return ptr;
}

static void free_safe(void* ptr) {
    if (ptr) free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a dummy file with the fuzzing data
    const char* dummy_file = "./dummy_file";
    FILE* fp = fopen(dummy_file, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        remove(dummy_file);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        remove(dummy_file);
        return 0;
    }

    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        remove(dummy_file);
        return 0;
    }

    // Open the dummy file for reading
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        remove(dummy_file);
        return 0;
    }
    png_init_io(png_ptr, fp);

    // Set user limits based on fuzz input if available
    png_uint_32 user_width_max = 0;
    png_uint_32 user_height_max = 0;
    if (Size >= 8) {
        user_width_max = *(const png_uint_32*)(Data);
        user_height_max = *(const png_uint_32*)(Data + 4);
    }
    png_set_user_limits(png_ptr, user_width_max, user_height_max);

    // Read the IHDR chunk
    png_read_info(png_ptr, info_ptr);

    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);

    // Get user limits
    png_uint_32 max_width = png_get_user_width_max(png_ptr);
    png_uint_32 max_height = png_get_user_height_max(png_ptr);

    // Validate dimensions against user limits
    if (width > max_width || height > max_height) {
        // Dimension exceeds user limit
    }

    // Get full IHDR details
    png_uint_32 ihdr_width, ihdr_height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr, info_ptr, &ihdr_width, &ihdr_height,
                 &bit_depth, &color_type, &interlace_method,
                 &compression_method, &filter_method);

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    remove(dummy_file);

    return 0;
}