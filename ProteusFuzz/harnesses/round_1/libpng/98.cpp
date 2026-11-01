// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_info at pngread.c:102:1 in png.h
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
// png_set_alpha_mode at pngrtran.c:444:1 in png.h
// png_set_invert_alpha at pngtrans.c:236:1 in png.h
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
// png_set_background at pngrtran.c:155:1 in png.h
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_set_flush at pngwrite.c:966:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
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
#include <png.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Dummy read/write callbacks for libpng
static void
dummy_read(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void
dummy_write(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void
dummy_flush(png_structp png_ptr) {
    (void)png_ptr;
}

static void
dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    exit(0);
}

static void
dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const char *dummy_file = "./dummy_file";
    int fd = open(dummy_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        return 0;
    }
    write(fd, Data, Size);
    close(fd);

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        unlink(dummy_file);
        return 0;
    }

    png_set_error_fn(png_ptr, NULL, dummy_error, dummy_warning);

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        unlink(dummy_file);
        return 0;
    }

    FILE *fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        unlink(dummy_file);
        return 0;
    }

    png_init_io(png_ptr, fp);

    unsigned char sig[8];
    if (fread(sig, 1, 8, fp) != 8) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        unlink(dummy_file);
        return 0;
    }

    if (png_sig_cmp(sig, 0, 8)) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        unlink(dummy_file);
        return 0;
    }

    png_read_info(png_ptr, info_ptr);

    int mode = Data[0] % 4;
    png_fixed_point output_gamma = (png_fixed_point)(Data[1] * 100000);
    if (output_gamma == 0) output_gamma = 100000;
    png_set_alpha_mode_fixed(png_ptr, mode, output_gamma);

    double gamma_double = (double)(Data[2] % 256) / 100.0;
    if (gamma_double == 0) gamma_double = 1.0;
    png_set_alpha_mode(png_ptr, mode, gamma_double);

    png_set_invert_alpha(png_ptr);

    png_fixed_point screen_gamma = (png_fixed_point)(Data[3] * 100000);
    if (screen_gamma == 0) screen_gamma = 100000;
    png_fixed_point file_gamma = (png_fixed_point)(Data[4] * 100000);
    if (file_gamma == 0) file_gamma = 100000;
    png_set_gamma_fixed(png_ptr, screen_gamma, file_gamma);

    png_color_16 background_color;
    background_color.red = Data[5];
    background_color.green = Data[6];
    background_color.blue = Data[7];
    background_color.gray = 0;
    background_color.index = 0;
    int bg_gamma_code = Data[8] % 4;
    int need_expand = Data[9] % 2;
    double bg_gamma = (double)(Data[10] % 256) / 100.0;
    if (bg_gamma == 0) bg_gamma = 1.0;
    png_set_background(png_ptr, &background_color, bg_gamma_code, need_expand, bg_gamma);

    png_fixed_point bg_gamma_fixed = (png_fixed_point)(Data[11] * 100000);
    if (bg_gamma_fixed == 0) bg_gamma_fixed = 100000;
    png_set_background_fixed(png_ptr, &background_color, bg_gamma_code, need_expand, bg_gamma_fixed);

    png_set_scale_16(png_ptr);

    int nrows = Data[12] % 100;
    png_set_flush(png_ptr, nrows);

    png_bytep row = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
    for (int y = 0; y < 100; ++y) {
        png_read_row(png_ptr, row, NULL);
    }
    png_free(png_ptr, row);

    png_read_end(png_ptr, NULL);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    unlink(dummy_file);

    return 0;
}