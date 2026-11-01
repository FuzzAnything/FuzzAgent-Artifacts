// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_row at pngread.c:376:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_set_invert_mono at pngtrans.c:249:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <cstdint>

static void write_dummy_png_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    write_dummy_png_file("./dummy_file", Data, Size);

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    FILE *fp = NULL;
    png_byte **row_pointers = NULL;
    png_byte *row = NULL;
    png_byte *display_row = NULL;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        if (row_pointers) {
            for (size_t i = 0; i < 10000; ++i) {
                if (row_pointers[i]) free(row_pointers[i]);
            }
            free(row_pointers);
        }
        if (row) free(row);
        if (display_row) free(display_row);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        if (fp) fclose(fp);
        return 0;
    }

    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    png_read_info(png_ptr, info_ptr);

    png_set_expand(png_ptr);
    png_set_invert_mono(png_ptr);
    png_set_palette_to_rgb(png_ptr);

    png_byte gray_status = png_get_rgb_to_gray_status(png_ptr);
    (void)gray_status;

    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    if (width > 10000 || height > 10000) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    row_pointers = (png_byte **)malloc(sizeof(png_byte *) * height);
    if (!row_pointers) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    for (png_uint_32 y = 0; y < height; ++y) {
        row_pointers[y] = (png_byte *)malloc(rowbytes);
        if (!row_pointers[y]) {
            for (png_uint_32 i = 0; i < y; ++i) {
                free(row_pointers[i]);
            }
            free(row_pointers);
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return 0;
        }
    }

    row = (png_byte *)malloc(rowbytes);
    display_row = (png_byte *)malloc(rowbytes);

    if (row && display_row) {
        for (png_uint_32 y = 0; y < height; ++y) {
            png_read_row(png_ptr, row, display_row);
        }
    }

    png_read_image(png_ptr, row_pointers);

    if (row) free(row);
    if (display_row) free(display_row);
    for (png_uint_32 y = 0; y < height; ++y) {
        if (row_pointers[y]) free(row_pointers[y]);
    }
    free(row_pointers);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return 0;
}