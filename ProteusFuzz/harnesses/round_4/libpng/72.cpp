// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_read_user_transform_fn at pngrtran.c:1099:1 in png.h
// png_set_read_status_fn at pngread.c:945:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_get_current_pass_number at pngtrans.c:859:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
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
#include <fstream>

static void user_transform_callback(png_structp png_ptr, png_row_infop row_info, png_bytep row) {
    (void)png_ptr;
    (void)row_info;
    (void)row;
}

static void read_status_callback(png_structp png_ptr, png_uint_32 row_number, int pass) {
    (void)png_ptr;
    (void)row_number;
    (void)pass;
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    std::remove("./dummy_file");

    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file.is_open()) return 0;

    const unsigned char png_signature[] = {137, 80, 78, 71, 13, 10, 26, 10};
    file.write(reinterpret_cast<const char*>(png_signature), 8);
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();

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

    png_set_read_user_transform_fn(png_ptr, user_transform_callback);
    png_set_read_status_fn(png_ptr, read_status_callback);

    png_read_info(png_ptr, info_ptr);

    png_uint_32 width, height;
    int bit_depth, color_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

    if (width == 0 || height == 0) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    std::vector<png_bytep> row_pointers(height);
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    
    for (png_uint_32 y = 0; y < height; ++y) {
        row_pointers[y] = (png_bytep)malloc(row_bytes);
        if (!row_pointers[y]) {
            for (png_uint_32 i = 0; i < y; ++i) free(row_pointers[i]);
            fclose(fp);
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return 0;
        }
    }

    png_start_read_image(png_ptr);

    png_byte pass = png_get_current_pass_number(png_ptr);
    (void)pass;

    png_read_image(png_ptr, row_pointers.data());

    png_byte *old_row = (png_byte*)malloc(row_bytes);
    png_byte *new_row = (png_byte*)malloc(row_bytes);
    if (old_row && new_row) {
        memset(old_row, 0, row_bytes);
        memset(new_row, 0, row_bytes);
        png_progressive_combine_row(png_ptr, old_row, new_row);
    }

    for (png_uint_32 y = 0; y < height; ++y) {
        free(row_pointers[y]);
    }
    if (old_row) free(old_row);
    if (new_row) free(new_row);

    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}