// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

extern "C" {
#include "code/png.h"
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Dummy error handler
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Dummy warning handler
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    return png_ptr;
}

static png_infop create_info_struct(png_structp png_ptr) {
    if (!png_ptr) return nullptr;
    return png_create_info_struct(png_ptr);
}

extern "C" int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize PNG structures
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) {
        remove("./dummy_file");
        return 0;
    }

    png_infop info_ptr = create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        remove("./dummy_file");
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        remove("./dummy_file");
        return 0;
    }

    // Try to read PNG header from dummy file
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        remove("./dummy_file");
        return 0;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    // Call target API functions
    png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_byte channels = png_get_channels(png_ptr, info_ptr);
    png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);

    png_uint_32 width, height;
    int ihdr_bit_depth, color_type, ihdr_interlace_method, compression_method, filter_method;
    png_uint_32 get_ihdr_result = png_get_IHDR(png_ptr, info_ptr, &width, &height,
                                      &ihdr_bit_depth, &color_type, &ihdr_interlace_method,
                                      &compression_method, &filter_method);

    // Clean up
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    remove("./dummy_file");

    // Use volatile variables to prevent optimization
    volatile png_byte dummy1 = compression_type;
    volatile png_byte dummy2 = interlace_type;
    volatile png_byte dummy3 = bit_depth;
    volatile png_byte dummy4 = channels;
    volatile png_byte dummy5 = filter_type;
    volatile png_uint_32 dummy6 = get_ihdr_result;
    (void)dummy1; (void)dummy2; (void)dummy3; (void)dummy4; (void)dummy5; (void)dummy6;

    return 0;
}