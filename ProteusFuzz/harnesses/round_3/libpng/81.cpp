// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_user_width_max at pngget.c:1305:1 in png.h
// png_get_user_height_max at pngget.c:1311:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_get_user_width_max at pngget.c:1305:1 in png.h
// png_get_user_height_max at pngget.c:1311:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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
#include <cstdio>
#include <cstring>

static void safe_png_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        png_destroy_read_struct(nullptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file for the fuzzer input
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    if (fwrite(Data, 1, Size, fp) != Size) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    // Initialize PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        safe_png_cleanup(png_ptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        safe_png_cleanup(png_ptr, info_ptr);
        return 0;
    }

    // Open the file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        safe_png_cleanup(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Test png_set_user_limits with various values derived from input
    png_uint_32 width_max = 0;
    png_uint_32 height_max = 0;

    if (Size >= 4) {
        width_max = *(const png_uint_32 *)(Data);
        height_max = *(const png_uint_32 *)(Data + 4);
    } else if (Size >= 2) {
        width_max = Data[0];
        height_max = Data[1];
    } else if (Size >= 1) {
        width_max = Data[0];
        height_max = Data[0];
    }

    // Avoid setting limits to 0 which might cause immediate rejection of valid images
    if (width_max == 0) width_max = 1;
    if (height_max == 0) height_max = 1;

    png_set_user_limits(png_ptr, width_max, height_max);

    // Test png_get_user_width_max and png_get_user_height_max before reading
    png_uint_32 user_w_max = png_get_user_width_max(png_ptr);
    png_uint_32 user_h_max = png_get_user_height_max(png_ptr);
    (void)user_w_max;
    (void)user_h_max;

    // Attempt to read the IHDR chunk
    png_read_info(png_ptr, info_ptr);

    // Variables to receive IHDR data
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;

    // Test png_get_IHDR with all pointers
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // Test png_get_IHDR with some NULL pointers to exercise different code paths
    png_get_IHDR(png_ptr, info_ptr, &width, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    png_get_IHDR(png_ptr, info_ptr, nullptr, &height, &bit_depth, nullptr, nullptr, nullptr, nullptr);
    png_get_IHDR(png_ptr, info_ptr, nullptr, nullptr, nullptr, &color_type, &interlace_method, &compression_method, &filter_method);

    // Test png_get_image_width and png_get_image_height
    png_uint_32 img_width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 img_height = png_get_image_height(png_ptr, info_ptr);
    (void)img_width;
    (void)img_height;

    // Test with NULL pointers for safety checks
    png_uint_32 null_width = png_get_image_width(nullptr, info_ptr);
    png_uint_32 null_height = png_get_image_height(png_ptr, nullptr);
    png_uint_32 null_user_w = png_get_user_width_max(nullptr);
    png_uint_32 null_user_h = png_get_user_height_max(nullptr);
    (void)null_width;
    (void)null_height;
    (void)null_user_w;
    (void)null_user_h;

    // Clean up
    fclose(fp);
    safe_png_cleanup(png_ptr, info_ptr);

    return 0;
}