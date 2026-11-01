// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
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
// png_get_IHDR at pngget.c:928:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_rows at pngread.c:641:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_get_current_row_number at pngtrans.c:847:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
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

static void create_dummy_png_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    create_dummy_png_file("./dummy_file", Data, Size);

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    png_read_info(png_ptr, info_ptr);

    png_uint_32 width, height;
    int bit_depth, color_type;
    
    if (png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL) != 0) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    if (row_bytes == 0) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    std::vector<png_byte> row_buffer(row_bytes);
    std::vector<png_byte> display_buffer(row_bytes);
    
    std::vector<png_byte*> row_pointers(height);
    for (png_uint_32 i = 0; i < height; ++i) {
        row_pointers[i] = &row_buffer[0];
    }

    png_read_update_info(png_ptr, info_ptr);

    for (png_uint_32 i = 0; i < height && i < 10; ++i) {
        png_read_row(png_ptr, row_buffer.data(), display_buffer.data());
    }

    for (png_uint_32 i = 0; i < height; ++i) {
        row_pointers[i] = &row_buffer[0];
    }
    png_uint_32 rows_to_read = (height < 10) ? height : 10;
    png_read_rows(png_ptr, row_pointers.data(), nullptr, rows_to_read);

    png_read_image(png_ptr, row_pointers.data());

    png_byte pass_num = png_get_current_pass_number(png_ptr);
    (void)pass_num;

    png_uint_32 row_num = png_get_current_row_number(png_ptr);
    (void)row_num;

    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr_write) return 0;

    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_write_struct(&png_ptr_write, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }

    FILE *fp_write = fopen("./dummy_file_write", "wb");
    if (!fp_write) {
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }

    png_init_io(png_ptr_write, fp_write);

    png_set_IHDR(png_ptr_write, info_ptr_write, width, height, bit_depth, color_type, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    png_write_info(png_ptr_write, info_ptr_write);

    std::fill(row_buffer.begin(), row_buffer.end(), 0);
    
    for (png_uint_32 i = 0; i < height; ++i) {
        row_pointers[i] = &row_buffer[0];
    }

    png_uint_32 write_rows_count = (height < 5) ? height : 5;
    png_write_rows(png_ptr_write, row_pointers.data(), write_rows_count);

    png_write_end(png_ptr_write, info_ptr_write);

    fclose(fp_write);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);

    return 0;
}