// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_read_frame_head at pngread.c:195:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_write_frame_tail at pngwrite.c:1529:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_get_next_frame_delay_den at pngget.c:1479:1 in png.h
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
#include <setjmp.h>

static void fuzz_error_fn(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void fuzz_warning_fn(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

static void fuzz_write_data_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
    // Dummy write function
}

static void fuzz_read_data_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
    // Dummy read function
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, fuzz_error_fn, fuzz_warning_fn);
    if (!png_ptr) return nullptr;
    return png_ptr;
}

static png_infop create_png_info(png_structp png_ptr) {
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return nullptr;
    }
    return info_ptr;
}

static void cleanup_png(png_structp *png_ptr, png_infop *info_ptr) {
    if (png_ptr && *png_ptr) {
        png_destroy_read_struct(png_ptr, info_ptr, nullptr);
        *png_ptr = nullptr;
    }
    if (info_ptr) {
        *info_ptr = nullptr;
    }
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create dummy file for reading/writing if needed
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initialize PNG structures
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) return 0;
    png_infop info_ptr = create_png_info(png_ptr);
    if (!info_ptr) {
        cleanup_png(&png_ptr, &info_ptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize read/write functions
    png_set_read_fn(png_ptr, nullptr, fuzz_read_data_fn);
    png_set_write_fn(png_ptr, nullptr, fuzz_write_data_fn, nullptr);

    // Fuzz png_get_next_frame_delay_den
    png_uint_16 delay_den = png_get_next_frame_delay_den(png_ptr, info_ptr);
    (void)delay_den;

    // Fuzz png_get_next_frame_fcTL
    png_uint_32 width = 0, height = 0, x_offset = 0, y_offset = 0;
    png_uint_16 delay_num = 0, delay_den_fcTL = 0;
    png_byte dispose_op = 0, blend_op = 0;
    png_uint_32 ret_fcTL = png_get_next_frame_fcTL(png_ptr, info_ptr, &width, &height, &x_offset, &y_offset, &delay_num, &delay_den_fcTL, &dispose_op, &blend_op);
    (void)ret_fcTL;

    // Fuzz png_read_frame_head
    png_read_frame_head(png_ptr, info_ptr);

    // Fuzz png_set_next_frame_fcTL
    png_uint_32 ret_set_fcTL = png_set_next_frame_fcTL(png_ptr, info_ptr, width, height, x_offset, y_offset, delay_num, delay_den_fcTL, dispose_op, blend_op);
    (void)ret_set_fcTL;

    // Fuzz png_write_frame_tail
    png_write_frame_tail(png_ptr, info_ptr);

    // Fuzz png_write_frame_head
    png_byte **row_pointers = nullptr;
    png_write_frame_head(png_ptr, info_ptr, row_pointers, width, height, x_offset, y_offset, delay_num, delay_den_fcTL, dispose_op, blend_op);

    // Cleanup
    cleanup_png(&png_ptr, &info_ptr);

    // Remove dummy file
    remove(dummy_file);

    return 0;
}