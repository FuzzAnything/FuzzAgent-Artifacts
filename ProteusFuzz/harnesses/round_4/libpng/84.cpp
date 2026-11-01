// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_info at pngwrite.c:228:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_swap at pngtrans.c:34:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
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

static void fuzz_png_read_row(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }

    png_init_io(png_ptr, fp);

    png_read_info(png_ptr, info_ptr);

    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    if (row_bytes == 0 || width == 0 || height == 0) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }

    std::vector<png_byte> row(row_bytes, 0);
    std::vector<png_byte> display_row(row_bytes, 0);

    png_set_swap(png_ptr);
    png_set_swap_alpha(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    for (png_uint_32 y = 0; y < height; ++y) {
        png_read_row(png_ptr, row.data(), display_row.data());
    }

    png_read_end(png_ptr, info_ptr);
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
}

static void fuzz_png_write_rows(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }

    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }

    png_init_io(png_ptr, fp);

    png_uint_32 width = 10;
    png_uint_32 height = 10;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    std::vector<png_byte> row_data(width * 3, 0);
    std::vector<png_byte *> rows;
    for (png_uint_32 y = 0; y < height; ++y) {
        rows.push_back(row_data.data());
    }

    png_set_swap(png_ptr);
    png_set_swap_alpha(png_ptr);

    png_write_rows(png_ptr, rows.data(), static_cast<png_uint_32>(rows.size()));

    png_write_end(png_ptr, info_ptr);
    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);
}

static void fuzz_png_read_rows(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    if (row_bytes == 0 || width == 0 || height == 0) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }

    std::vector<png_byte> row(row_bytes, 0);
    std::vector<png_byte> display_row(row_bytes, 0);
    std::vector<png_byte *> rows;
    std::vector<png_byte *> display_rows;

    for (png_uint_32 y = 0; y < height; ++y) {
        rows.push_back(row.data());
        display_rows.push_back(display_row.data());
    }

    png_set_swap(png_ptr);
    png_set_swap_alpha(png_ptr);

    png_read_update_info(png_ptr, info_ptr);
    png_read_rows(png_ptr, rows.data(), display_rows.data(), height);

    png_read_end(png_ptr, info_ptr);
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
}

static void fuzz_png_read_image(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    if (row_bytes == 0 || width == 0 || height == 0) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }

    std::vector<png_byte> row(row_bytes, 0);
    std::vector<png_byte *> image;
    for (png_uint_32 y = 0; y < height; ++y) {
        image.push_back(row.data());
    }

    png_set_swap(png_ptr);
    png_set_swap_alpha(png_ptr);

    png_read_update_info(png_ptr, info_ptr);
    png_read_image(png_ptr, image.data());

    png_read_end(png_ptr, info_ptr);
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    int mode = Data[0] % 5;

    switch (mode) {
        case 0:
            fuzz_png_read_row(Data + 1, Size - 1);
            break;
        case 1:
            fuzz_png_write_rows(Data + 1, Size - 1);
            break;
        case 2:
            fuzz_png_read_rows(Data + 1, Size - 1);
            break;
        case 3:
            fuzz_png_read_image(Data + 1, Size - 1);
            break;
        case 4:
            fuzz_png_read_row(Data + 1, Size - 1);
            fuzz_png_read_image(Data + 1, Size - 1);
            break;
    }

    return 0;
}