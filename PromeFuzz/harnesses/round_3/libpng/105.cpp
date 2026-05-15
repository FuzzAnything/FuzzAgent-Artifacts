// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "code/png.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp message) {
    (void)png_ptr;
    (void)message;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp message) {
    (void)png_ptr;
    (void)message;
    longjmp(png_jmpbuf(png_ptr), 1);
}

extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Write data to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }

    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Set up custom read function
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);

    // Try to read PNG header
    png_set_sig_bytes(png_ptr, 0);
    png_read_info(png_ptr, info_ptr);

    // Test target API functions
    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels;

    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_uint_32 ihdr_result = png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
                                          &color_type, &interlace_method, &compression_method,
                                          &filter_method);
    (void)ihdr_result;

    png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    (void)compression_type;

    png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);
    (void)filter_type;

    png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    (void)interlace_type;

    png_byte color_type_result = png_get_color_type(png_ptr, info_ptr);
    (void)color_type_result;

    // Test with NULL parameters
    png_get_channels(nullptr, nullptr);
    png_get_channels(png_ptr, nullptr);
    png_get_channels(nullptr, info_ptr);

    png_get_IHDR(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    png_get_IHDR(png_ptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    png_get_IHDR(nullptr, info_ptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

    png_get_compression_type(nullptr, nullptr);
    png_get_compression_type(png_ptr, nullptr);
    png_get_compression_type(nullptr, info_ptr);

    png_get_filter_type(nullptr, nullptr);
    png_get_filter_type(png_ptr, nullptr);
    png_get_filter_type(nullptr, info_ptr);

    png_get_interlace_type(nullptr, nullptr);
    png_get_interlace_type(png_ptr, nullptr);
    png_get_interlace_type(nullptr, info_ptr);

    png_get_color_type(nullptr, nullptr);
    png_get_color_type(png_ptr, nullptr);
    png_get_color_type(nullptr, info_ptr);

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}