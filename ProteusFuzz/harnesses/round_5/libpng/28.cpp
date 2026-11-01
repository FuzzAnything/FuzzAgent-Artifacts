// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_y_offset_inches at pngget.c:424:1 in png.h
// png_get_pixel_aspect_ratio at pngget.c:189:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
// png_get_y_offset_pixels at pngget.c:304:1 in png.h
// png_get_x_offset_inches at pngget.c:413:1 in png.h
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

static void dummy_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

struct FuzzIO {
    const uint8_t *data;
    size_t size;
    size_t offset;
};

static void fuzz_read_callback(png_structp png_ptr, png_bytep out_bytes, png_size_t byte_count_to_read) {
    FuzzIO *state = (FuzzIO*)png_get_io_ptr(png_ptr);
    if (!state) return;

    if (state->offset >= state->size) {
        return;
    }

    png_size_t bytes_to_read = (state->size - state->offset < byte_count_to_read) 
        ? (state->size - state->offset) 
        : byte_count_to_read;
        
    memcpy(out_bytes, state->data + state->offset, bytes_to_read);
    state->offset += bytes_to_read;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_set_error_fn(png_ptr, nullptr, dummy_error_handler, dummy_warning_handler);

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    FuzzIO io_state;
    io_state.data = Data;
    io_state.size = Size;
    io_state.offset = 0;

    png_set_read_fn(png_ptr, &io_state, fuzz_read_callback);

    png_read_info(png_ptr, info_ptr);

    float y_offset_inches = png_get_y_offset_inches(png_ptr, info_ptr);
    (void)y_offset_inches;

    float pixel_aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);
    (void)pixel_aspect_ratio;

    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr, info_ptr);
    (void)x_offset_microns;

    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
    (void)y_offset_pixels;

    float x_offset_inches = png_get_x_offset_inches(png_ptr, info_ptr);
    (void)x_offset_inches;

#ifdef PNG_pHYs_SUPPORTED
    png_uint_32 res_x, res_y;
    int unit_type;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)res_x; (void)res_y; (void)unit_type;
#endif

    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    return 0;
}